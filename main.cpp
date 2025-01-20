#include <iostream>
#include <iomanip>
#include <filesystem>
#include <string>

#include "InputArgs.h"
#include "ExpenseSheet.h"

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
    ExpenseSheet expenses;
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
            if (args.Count() == 2 || args.Count() == 3){ //label and value
                auto label = args[0];
                double value;
                if (args.Count() == 2){
                    value = atof(args[1].c_str()) * -1; //default negative expense
                } else if (args.Count() == 3){
                    value = atof(args[2].c_str());
                    if (args[1] != "+"){
                        value *= -1.0;
                    }
                }
                if (!expenses.Add(label, value)){
                    std::cout << "Failed to add." << std::endl;
                }
                //expenses.Add(label, value);
            }else{
                std::cout << "Used Incorrectly. Please folow format add <label> (+/-) <value>. If a label longer than one word is desired, please wrap label in quotation marks "". " << std::endl;
            }
           
        }else if (cmd == "del"){
            if (args.Count() == 1){
                if (!expenses.Del(args[0])){
                    std::cout << "Failed to delete." << std::endl;
                }
                //expenses.Del(args[0]);
            } else{
                std::cout << "Used Incorrectly. Please folow format del <label>." << std::endl;
            }
        }else if (cmd == "list"){
            expenses.List(std::cout);
        }else if (cmd == "eval"){
            auto value = expenses.Eval();
            std::cout << "TOTAL: " << value << std::endl;
            // std::set_precision(3)
        }else if (cmd == "exit"){
            break;
        }else{
            std::cout << "Command \"" << cmd << "\" is unknown." << std::endl;
        }
    }
}
