#ifndef __NETWORKCLIENT_H__
#define __NETWORKCLIENT_H__

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include <string>
#include <thread>
#include <map>

#include "json/json.hpp"

typedef struct sockaddr Sockaddr;
typedef struct sockaddr_in SockaddrIn;

typedef void (*NetworkCallback)(const nlohmann::json& json_cmd);

class NetworkClient
{
private:
    int client_fd;
    SockaddrIn address;
    std::string ip_address;
    int port;
    std::map<int,NetworkCallback> network_callbacks;
public:
    NetworkClient(std::string ip_address, int port, std::map<int,NetworkCallback> network_callbacks);
    ~NetworkClient();
    void OpenConnection();
private:
    void HandleConnection();
    void DispatchCmd(const std::string& json_string);
    void SendRawMessage(const nlohmann::json& data);
};

#endif // __NETWORKCLIENT_H__
