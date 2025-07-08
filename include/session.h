//
// Created by Dominik on 07/07/2025.
//

#ifndef SESSION_H

#define SESSION_H
#include <string>

int session(std::string target_ip,int port, int num_connect, int keepalive, int con_number, bool send_data, int timeout);

#endif //SESSION_H
