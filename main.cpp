#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <fstream>
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
int main(int argc, char** argv) {
    std::string line;
    ExpenseSheet expenses;

    //check if file will be opened
    if (argc == 2) {
        std::filesystem::path path = (argv[1]);
        expenses.Load(path);
    }

    //command loop
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
                std::cout << "Used Incorrectly. Please folow format add <label> (+/-) <value>. Label must be unique. Value must not contain commas. If a label longer than one word is desired, please wrap label in quotation marks "". " << std::endl;
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
        }else if (cmd == "save"){
            if (args.Count() == 1){
                std::filesystem::path p = args[0];
                if (!expenses.Save(p)){
                    std::cout << "Failed to save." << std::endl;
                }
            } else if (args.Count() == 0){
                if (!expenses.Save()){
                    std::cout << "Failed to save. Try the format save <filepath>." << std::endl;
                }
            }else{
                std::cout << "Used Incorrectly. Please folow format save <filepath>." << std::endl;
            }

        }else if (cmd == "load"){
            if (args.Count() == 1){
                std::filesystem::path p = args[0];
                if (!expenses.Load(p)){
                    std::cout << "Failed to load." << std::endl;
                }
            } else{
                std::cout << "Used Incorrectly. Please folow format load <filepath>." << std::endl;
            }
        }else if (cmd == "clear"){
            expenses.Clear();     
        }else if (cmd =="export"){
            if (args.Count() == 2){
                if (args[0] == "csv"){
                    expenses.ExportCSV(args[1]);
                }else if (args[0] == "html"){
                    expenses.ExportHTML(args[1]);
                }else{
                    std::cout << "Used Incorrectly. Please folow format export <csv/html> <file>" << std::endl;
                }
            }else{
                    std::cout << "Used Incorrectly. Please folow format export <csv/html> <file>." << std::endl;
            }           
        }else{
            std::cout << "Command \"" << cmd << "\" is unknown." << std::endl;
        }
    }
}
