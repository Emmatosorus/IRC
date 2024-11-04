#include "../include/Server.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    (void)argc;
    try
    {
        std::string password;
        std::string port = "6667";
        if (argc == 2)
            password = argv[1];
        else if (argc == 3)
        {
            password = argv[2];
            port = argv[1];
        }
        else
        {
            std::cerr << "ircserv must be called with : ./ircserv <port> <password>\n";
            return EXIT_FAILURE;
        }
        if (password.find_first_of("#%:,*?!@.\t\r\n ") != std::string::npos)
        {
            std::cerr << "Forbidden characters in password: #%:,*?!@. \\t\\r\\n\n";
            return EXIT_FAILURE;
        }
        if (password.size() > 16)
        {
            std::cerr << "Password must be less than 16 characters\n";
            return EXIT_FAILURE;
        }

        Server s(password.c_str(), port.c_str());
        s.start();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
