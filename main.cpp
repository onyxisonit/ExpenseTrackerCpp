#include <iostream>
#include "InputArgs.h"

/*
* CMD Args
* -add  Label (+/- default:-) 0.00
* -del  Label
* -list
* -eval
* -exit
*/
int main(){
    std::string line;
    while (true){
        //get user input
        std::cout << "> ";
        std::getline(std::cin, line); //read in entire line to parse later
        // std::cout << cmd << std::endl;
        InputArgs inArgs(line);
        std::string cmd = inArgs[0];
        
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower); //make case insensitive by making input arguments lowercase

        auto args = inArgs.Subset(1); //extract arguments using class (skip the command)

        // for (size_t i = 0; i < args.Count(); ++i) {
        //     std::cout << "Arg[" << i << "]: " << args[i] << std::endl;
        // }

        //check command
        if (cmd == "add"){
            std::cout << cmd << std::endl;
            // std::cout << "Parsed Arguments:\n";
            // for (size_t i = 0; i < inArgs.Count(); ++i) {
            //     std::cout << "Arg[" << i << "]: " << inArgs[i] << "\n";
            // }
        }else if (cmd == "del"){
            std::cout << cmd << std::endl;
        }else if (cmd == "list"){
            std::cout << cmd << std::endl;
        }else if (cmd == "eval"){
            std::cout << cmd << std::endl;
        }else if (cmd == "exit"){
            break;
        }else{
            std::cout << "Command \"" << cmd << "\" is unknown." << std::endl;
        }
    }
}
