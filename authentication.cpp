#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "authentication.h"


//registers given username and password into the database
bool registration(std::string username, std::string password){
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


//checks if a username and corresponding password are in the database
bool loginCheck(std::string mUsername, std::string mPassword){
    std::ifstream database;
    database.open("database.txt");
    std::string username,password;
    while(database >> username >> password/*.is_open()*/){
        /*database >> username;
        database >> password;*/
        if(username == mUsername && password == mPassword){
            return true;//(username == mUsername && password == mPassword);
        }
    }
    return false;//(username == mUsername && password == mPassword);//safety return exit
}



//prompts user to input a username and password matching one in the database
void login(bool& check){
    std::string username, password;
    //do{
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;
    if(!loginCheck(username,password)){
        std:: cout << "\n-Invalid credentials, please try again-\n";
    }else{
        std:: cout << "\nWelcome back, " << username << "\n\n";
        check = true;
    }
    //}while(!loginCheck(username,password));
}


//prompts user to register a username and password for the database
void registering(){
    std::string username, password;
    std::cout << "Enter a username: ";
    std::cin >> username;
    std::cout << "Enter a password: ";
    std::cin >> password;
    
    registration(username,password);
}