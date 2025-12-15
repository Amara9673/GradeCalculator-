#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "authentication.h"


// adds a username + password to the database
bool registerUser(std::string username, std::string password){
    std::stringstream curdata;

    std::ifstream data;
    data.open("database.txt");
    curdata << data.rdbuf();
    data.close();

    std::ofstream database;
    database.open("database.txt");

    database << curdata.str() << "\n" << username << " " << password;
    return 1;
}


// checks if username/password exists in the database
bool checkLogin(std::string mUsername, std::string mPassword){
    std::ifstream database;
    database.open("database.txt");

    std::string username, password;
    while(database >> username >> password){
        if(username == mUsername && password == mPassword){
            return true;
        }
    }
    return false;
}


// console login (not used in GTK, but keeping it)
void login(bool& check){
    std::string username, password;

    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;

    if(!checkLogin(username,password)){
        std::cout << "\n-Invalid credentials, please try again-\n";
    }else{
        std::cout << "\nWelcome back, " << username << "\n\n";
        check = true;
    }
}


// console register (not used in GTK, but keeping it)
void makeAccount(){
    std::string username, password;

    std::cout << "Enter a username: ";
    std::cin >> username;
    std::cout << "Enter a password: ";
    std::cin >> password;

    registerUser(username,password);
}
