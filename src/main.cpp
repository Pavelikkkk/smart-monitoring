#include <iostream>

void print_usage(){
    std::cout << "Dorm Energy Simulator - usage:\n";
    std::cout << "  sim --version, -v     show version\n";
    std::cout << "  sim --help, -h        show help menu\n";
    std::cout << "  sim simulate          soon\n";
}


int main(int argc, char* argv[]){

    std::cout << "Dorm Energy Simulator\n";
    std::cout << "iteration 1 - Skeleton\n";

    if(argc < 2){
        print_usage();
        return 1;
    }

    std::string arg = argv[1];

    if(arg == "--version" || arg == "-v"){
        std::cout << "version 0.0.1-dev";
        return 0;  
    }
    else if(arg == "--help" || arg == "-h"){
        print_usage();
        return 0;
    }
    else if(arg == "simulate"){
        std::cout << "Soon";
        return 0;
    }
    else{
        std::cout << "Wrong command: \n" << arg;
        print_usage();
        return 1;
    }

    std::cout << "Enter..";
    std::cin.get();
    
    return 0;
}