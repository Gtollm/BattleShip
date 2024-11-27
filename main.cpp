
#include <string>
#include <iostream>

int main() {
    while(true) {
        std::string cmd;
        std::getline(std::cin, cmd);

        if(cmd == "exit") {
            return 0;
        } else if(cmd == "ping") {
            std::cout << "pong" << std::endl;
        }
    }

    return 0;
}