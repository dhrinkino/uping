#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;
class SessionConnection : public std::enable_shared_from_this<SessionConnection> {
private:
    ip::tcp::socket socket_;
    io_context& io_context_;
    std::string target_host_;
    uint16_t target_port_;
    int keepalive_interval_;
    steady_timer keepalive_timer_;
    std::atomic<bool> connected_{false};
    int connection_id_;
    bool send_keepalive_data_;

public:
    SessionConnection(io_context& io_ctx, const std::string& host, uint16_t port,
                  int keepalive_interval, int conn_id, bool send_keepalive = true)
        : socket_(io_ctx), io_context_(io_ctx), target_host_(host),
          target_port_(port), keepalive_interval_(keepalive_interval),
          keepalive_timer_(io_ctx), connection_id_(conn_id),
          send_keepalive_data_(send_keepalive) {}

    void start() {
        cout << "Session " << connection_id_ << ": -> "
             << target_host_ << ":" << target_port_ << endl;

        ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(target_host_, to_string(target_port_));

        auto self = shared_from_this();
        async_connect(socket_, endpoints,
            [self](const boost::system::error_code& error, const ip::tcp::endpoint&) {
                self->handle_connect(error);
            });
    }

private:
    void handle_connect(const boost::system::error_code& error) {
        if (!error) {
            connected_ = true;
            cout << "Session " << connection_id_ << ": OK!" << endl;

            socket_.set_option(ip::tcp::socket::keep_alive(true));

            if (send_keepalive_data_ && keepalive_interval_ > 0) {
                schedule_keepalive();
            }

            start_read();
        } else {
            cout << "Session " << connection_id_ << ": Connection error: "
                 << error.message() << endl;
            reconnect();
        }
    }

    void schedule_keepalive() {
        if (!connected_) return;

        keepalive_timer_.expires_after(std::chrono::seconds(keepalive_interval_));

        auto self = shared_from_this();
        keepalive_timer_.async_wait([self](const boost::system::error_code& error) {
            self->send_keepalive(error);
        });
    }

    void send_keepalive(const boost::system::error_code& error) {
        if (error || !connected_) return;
        std::string keepalive_data = "\0";

        auto self = shared_from_this();
        async_write(socket_, buffer(keepalive_data),
            [self](const boost::system::error_code& error, size_t bytes_transferred) {
                self->handle_keepalive_write(error, bytes_transferred);
            });
    }

    void handle_keepalive_write(const boost::system::error_code& error, size_t bytes_transferred) {
        if (error) {
            cout << "Session " << connection_id_ << ": Error keepalive: "
                 << error.message() << endl;
            reconnect();
        } else {
            cout << "Session " << connection_id_ << ": Keepalive sended ("
                 << bytes_transferred << " bytes)" << endl;
            schedule_keepalive();
        }
    }

    void start_read() {
        if (!connected_) return;

        auto buffer = std::make_shared<std::array<char, 1024>>();
        auto self = shared_from_this();

        socket_.async_read_some(boost::asio::buffer(*buffer),
            [self, buffer](const boost::system::error_code& error, size_t bytes_transferred) {
                self->handle_read(error, bytes_transferred, buffer);
            });
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred,
                     std::shared_ptr<std::array<char, 1024>> buffer) {
        if (error) {
            if (error == boost::asio::error::eof) {
                cout << "Connection " << connection_id_ << ": Server terminated connection" << endl;
            } else {
                cout << "Connection " << connection_id_ << ": Read Error: "
                     << error.message() << endl;
            }
            reconnect();
        } else {
            cout << "Connection " << connection_id_ << ": Recvd "
                 << bytes_transferred << " bytes" << endl;
            start_read();
        }
    }

    void reconnect() {
        connected_ = false;
        keepalive_timer_.cancel();

        if (socket_.is_open()) {
            boost::system::error_code ec;
            socket_.close(ec);
        }

        cout << "Connection " << connection_id_ << ": Reconnecting..." << endl;

        auto timer = std::make_shared<steady_timer>(io_context_);
        timer->expires_after(std::chrono::seconds(1));

        auto self = shared_from_this();
        timer->async_wait([self, timer](const boost::system::error_code& error) {
            if (!error) {
                self->start();
            }
        });
    }

public:
    bool is_connected() const {
        return connected_;
    }

    void force_disconnect() {
        connected_ = false;
        keepalive_timer_.cancel();
        if (socket_.is_open()) {
            boost::system::error_code ec;
            socket_.close(ec);
        }
    }
};

class SessionManager {
private:
    io_context io_context_;
    std::vector<std::shared_ptr<SessionConnection>> connections_;
    std::vector<std::thread> threads_;
    std::atomic<bool> running_{false};

public:

    void start(const std::string& target_host, uint16_t target_port,
               int num_connections, int keepalive_interval, int num_threads = 4,
               bool send_keepalive_data = true) {

        running_ = true;

        for (int i = 0; i < num_connections; ++i) {
            auto conn = std::make_shared<SessionConnection>(
                io_context_, target_host, target_port, keepalive_interval,
                i + 1, send_keepalive_data);
            connections_.push_back(conn);

            conn->start();
        }

        for (int i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this]() {
                while (running_) {
                    try {
                        io_context_.run();
                        break;
                    } catch (const std::exception& e) {
                        cout << "IO Thread error: " << e.what() << endl;
                    }
                }
            });
        }
    }

    void stop() {
        running_ = false;

        for (auto& conn : connections_) {
            conn->force_disconnect();
        }

        io_context_.stop();

        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        connections_.clear();
    }

    void wait() {
        for (auto& thread : threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

};

int session(std::string target_ip,int port, int num_connect, int keepalive_interval, int con_number, bool send_data, int timeout) {

    SessionManager manager;
        manager.start(target_ip, port, num_connect,
                     keepalive_interval, con_number, send_data);

        cout << "Running" << endl;
        if (timeout > 0) {
            sleep(timeout);
        } else {
            cin.get();
        }

        manager.stop();


    return 0;
}