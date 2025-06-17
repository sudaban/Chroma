#include <iostream>
#include "Log.h"
#include "EnetServer.h"
#include "ItemDatabase.h"

static void ConsoleTitle() {
#ifdef _WIN32
    system("title Chroma GTPS Console");
#else
    std::cout << "\033]0;" << "Chroma GTPS Console" << "\007";
#endif
}

int main(int argc, char **argv)
{
    ConsoleTitle();
    {
        try
        {
            ENetServer server("0.0.0.0", 17091);
            if (!server.Start())
            {
                Logger("Failed to start server!", LogType::Error);
                return -1;
            }

            ItemDatabase db;
            if (!db.Decode("items.dat"))
            {
                Logger("Failed to decode items.dat!", LogType::Error);
                return -2;
            }

            Logger("Press ENTER to stop server...", LogType::Info);
            std::cin.get();

            server.Stop();
        }
        catch (const std::exception& ex)
        {
            Logger(std::string("Exception: ") + ex.what(), LogType::Error);
            return 1;
        }

        return 0;
    }
}