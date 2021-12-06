#include "IncludesAndDefines.h"
#include "CommandLine.h"
#include "Logic/RoomManager.h"
#include "Logic/Room.h"
int main() {
    using namespace Room;
    // Create a server endpoint
    srand(time(0));
    server pong_server;
    CommandLine cmd;
    RoomManager::instance = new RoomManager();
    try
    {

        // Set logging settings
        pong_server.clear_access_channels(websocketpp::log::alevel::all);
        pong_server.set_access_channels(websocketpp::log::alevel::connect);

        // Initialize Asio
        pong_server.init_asio();

        // Register our message handler
        pong_server.set_message_handler(bind(&OnMessage, &pong_server, ::_1, ::_2));
        pong_server.set_open_handler(bind(OnOpen,&pong_server,::_1));
        pong_server.set_close_handler(bind(OnClose, &pong_server, ::_1));
        // Listen on port 9002
        pong_server.listen(9002);

        std::cout << "Server Started\n";

        // Start the server accept loop
        pong_server.start_accept();

        // Start the ASIO io_service run loop
        pong_server.run();

    }
    catch(exception ex) {
        cout << ex.what() << endl;
    }
    
    delete RoomManager::instance;
    RoomManager::instance = 0;
}
