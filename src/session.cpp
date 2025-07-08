#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;


// Session connection structure
struct SessionConnection {
    ip::tcp::socket socket;
    std::string target_host;
    uint16_t target_port;
    int keepalive_interval;
    steady_timer keepalive_timer;
    std::atomic<bool> connected;
    int connection_id;
    bool send_keepalive_data;
    io_context* io_context_ptr;
    SessionConnection(io_context& io_ctx, const std::string& host, uint16_t port,
                     int keepalive_interval, int conn_id, bool send_keepalive = true)
        : socket(io_ctx), target_host(host), target_port(port),
          keepalive_interval(keepalive_interval), keepalive_timer(io_ctx),
          connected(false), connection_id(conn_id), send_keepalive_data(send_keepalive),
          io_context_ptr(&io_ctx) {}
};

struct SessionManager {
    io_context io_context;
    std::vector<std::shared_ptr<SessionConnection>> connections;
    std::vector<std::thread> threads;
    std::atomic<bool> running;
    SessionManager() : running(false) {}
};

// forced declarations
void session_start(std::shared_ptr<SessionConnection> conn);
void session_handle_connect(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error);
void session_schedule_keepalive(std::shared_ptr<SessionConnection> conn);
void session_send_keepalive(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error);
void session_handle_keepalive_write(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error, size_t bytes_transferred);
void session_start_read(std::shared_ptr<SessionConnection> conn);
void session_handle_read(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error, size_t bytes_transferred, std::shared_ptr<std::array<char, 1024>> buffer);
void session_reconnect(std::shared_ptr<SessionConnection> conn);
void session_force_disconnect(std::shared_ptr<SessionConnection> conn);
bool session_is_connected(std::shared_ptr<SessionConnection> conn);
void manager_start(SessionManager* manager, const std::string& target_host, uint16_t target_port,
                   int num_connections, int keepalive_interval, int num_threads = 1,
                   bool send_keepalive_data = true);
void manager_stop(SessionManager* manager);
void manager_wait(SessionManager* manager);

// Session connection functions
void session_start(std::shared_ptr<SessionConnection> conn) {
    std::cout << "Session " << conn->connection_id << ": -> "
         << conn->target_host << ":" << conn->target_port << endl;

    ip::tcp::resolver resolver(*conn->io_context_ptr);
    auto endpoints = resolver.resolve(conn->target_host, to_string(conn->target_port));

    async_connect(conn->socket, endpoints,
        [conn](const boost::system::error_code& error, const ip::tcp::endpoint&) {
            session_handle_connect(conn, error);
        });
}

void session_handle_connect(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error) {
    if (!error) {
        conn->connected = true;
        std::cout << "Session " << conn->connection_id << ": OK!" << endl;

        conn->socket.set_option(ip::tcp::socket::keep_alive(true));

        if (conn->send_keepalive_data && conn->keepalive_interval > 0) {
            session_schedule_keepalive(conn);
        }

        session_start_read(conn);
    } else {
        std::cout << "Session " << conn->connection_id << ": Connection error: "
             << error.message() << endl;
        session_reconnect(conn);
    }
}

void session_schedule_keepalive(std::shared_ptr<SessionConnection> conn) {
    if (!conn->connected) return;

    conn->keepalive_timer.expires_after(std::chrono::seconds(conn->keepalive_interval));

    conn->keepalive_timer.async_wait([conn](const boost::system::error_code& error) {
        session_send_keepalive(conn, error);
    });
}

void session_send_keepalive(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error) {
    if (error || !conn->connected) return;

    // TODO: make able to user set data or generate payload, at this moment just empty
    std::string keepalive_data = "\0";

    async_write(conn->socket, buffer(keepalive_data),
        [conn](const boost::system::error_code& error, size_t bytes_transferred) {
            session_handle_keepalive_write(conn, error, bytes_transferred);
        });
}

void session_handle_keepalive_write(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        std::cout << "Session " << conn->connection_id << ": Error keepalive: "
             << error.message() << endl;
        session_reconnect(conn);
    } else {
        std::cout << "Session " << conn->connection_id << ": Keepalive sended ("
             << bytes_transferred << " bytes)" << endl;
        session_schedule_keepalive(conn);
    }
}

void session_start_read(std::shared_ptr<SessionConnection> conn) {
    if (!conn->connected) return;

    auto buffer = std::make_shared<std::array<char, 1024>>();

    conn->socket.async_read_some(boost::asio::buffer(*buffer),
        [conn, buffer](const boost::system::error_code& error, size_t bytes_transferred) {
            session_handle_read(conn, error, bytes_transferred, buffer);
        });
}

void session_handle_read(std::shared_ptr<SessionConnection> conn, const boost::system::error_code& error, size_t bytes_transferred,
                        std::shared_ptr<std::array<char, 1024>> buffer) {
    if (error) {
        if (error == boost::asio::error::eof) {
            std::cout << "Connection " << conn->connection_id << ": Server terminated connection" << endl;
        } else {
            std::cout << "Connection " << conn->connection_id << ": Read Error: "
                 << error.message() << endl;
        }
        session_reconnect(conn);
    } else {
        std::cout << "Connection " << conn->connection_id << ": Recvd "
             << bytes_transferred << " bytes" << endl;
        session_start_read(conn);
    }
}

void session_reconnect(std::shared_ptr<SessionConnection> conn) {
    conn->connected = false;
    conn->keepalive_timer.cancel();

    if (conn->socket.is_open()) {
        boost::system::error_code ec;
        conn->socket.close(ec);
    }

    std::cout << "Connection " << conn->connection_id << ": Reconnecting..." << endl;

    auto timer = std::make_shared<steady_timer>(*conn->io_context_ptr);
    timer->expires_after(std::chrono::seconds(1));

    timer->async_wait([conn, timer](const boost::system::error_code& error) {
        if (!error) {
            session_start(conn);
        }
    });
}

bool session_is_connected(std::shared_ptr<SessionConnection> conn) {
    return conn->connected;
}

void session_force_disconnect(std::shared_ptr<SessionConnection> conn) {
    conn->connected = false;
    conn->keepalive_timer.cancel();
    if (conn->socket.is_open()) {
        boost::system::error_code ec;
        conn->socket.close(ec);
    }
}

void manager_start(SessionManager* manager, const std::string& target_host, uint16_t target_port,
                   int num_connections, int keepalive_interval, int num_threads,
                   bool send_keepalive_data) {

    manager->running = true;

    for (int i = 0; i < num_connections; ++i) {
        auto conn = std::make_shared<SessionConnection>(
            manager->io_context, target_host, target_port, keepalive_interval,
            i + 1, send_keepalive_data);
        manager->connections.push_back(conn);

        session_start(conn);
    }

    for (int i = 0; i < num_threads; ++i) {
        manager->threads.emplace_back([manager]() {
            while (manager->running) {
                try {
                    manager->io_context.run();
                    break;
                } catch (const std::exception& e) {
                    std::cout << "IO Thread error: " << e.what() << endl;
                }
            }
        });
    }
}

void manager_stop(SessionManager* manager) {
    manager->running = false;

    for (auto& conn : manager->connections) {
        session_force_disconnect(conn);
    }

    manager->io_context.stop();

    for (auto& thread : manager->threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    manager->connections.clear();
}

void manager_wait(SessionManager* manager) {
    for (auto& thread : manager->threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

int session(std::string target_ip, int port, int num_connect, int keepalive_interval, int con_number, bool send_data, int timeout) {
    SessionManager manager;

    manager_start(&manager, target_ip, port, num_connect, keepalive_interval, con_number, send_data);

    std::cout << "Running" << endl;
    if (timeout > 0) {
        sleep(timeout);
    } else {
        cin.get();
    }

    manager_stop(&manager);

    return 0;
}