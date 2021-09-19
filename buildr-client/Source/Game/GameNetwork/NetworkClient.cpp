#include <cstdio>
#include <cstring>

#include "NetworkClient.h"
#include "Logger.h"


#define MAX_PACKET 1024

NetworkClient::NetworkClient(std::string ip_address, int port, std::map<int,NetworkCallback> network_callbacks)
    : ip_address(ip_address), port(port), network_callbacks(network_callbacks)
{

}

NetworkClient::~NetworkClient()
{

}

void
NetworkClient::OpenConnection()
{
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Logger::ErrorDie("Cannot create the socket");
   
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address.c_str(), &address.sin_addr) <= 0) 
        Logger::ErrorDie("Invalid IP Address");
   
    if (connect(client_fd, (Sockaddr*)&address, sizeof(address)) < 0)
        Logger::ErrorDie("Connection failed");

    std::thread worker(&NetworkClient::HandleConnection, this);

    nlohmann::json data = {
        {"cmd", 1},
        {"position", {
            {"x", 1},
            {"y", 2}
        }}
    };
    SendRawMessage(data);

    worker.join();
}

void
NetworkClient::HandleConnection()
{
    char recv_msg[MAX_PACKET];
    int read_size;

    Logger::Network("Connection to server established");
    while (true) {
        memset(recv_msg, 0, MAX_PACKET);
        if ((read_size = read(client_fd, recv_msg, MAX_PACKET)) <= 0)
            continue;

        Logger::Debug(std::string(recv_msg));
        DispatchCmd(recv_msg);
    }
}

void
NetworkClient::DispatchCmd(const std::string& json_string)
{
    try {
        nlohmann::json data = nlohmann::json::parse(json_string);
        int cmd = data["cmd"];

        for (const auto &[nc_cmd, nc_callback] : network_callbacks) {
            if (cmd != nc_cmd) continue;
            
            nc_callback(cmd);
        }

    } catch (nlohmann::json::parse_error& e) {
        Logger::Warning("Malformed json string");
    }
}

void
NetworkClient::SendRawMessage(const nlohmann::json& data)
{
    std::string serialized = data.dump();
    const char* send_data = serialized.c_str();
    int data_len = strlen(send_data);

    if (data_len > MAX_PACKET)
        Logger::Warning("Attempting to send a packet exceeding max size");

    send(client_fd, send_data, data_len, 0);
    Logger::Debug("Sent message");
}

