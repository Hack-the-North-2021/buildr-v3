#include "Game/GameNetwork/NetworkClient.h"

int
main(int argc, char** argv)
{
    std::map<int,NetworkCallback> network_callbacks;
    NetworkClient client("127.0.0.1", 8080, network_callbacks); 
    client.OpenConnection();

    return 0;
}
