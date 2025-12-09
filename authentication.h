#ifndef AUTHENTICATION
#define AUTHENTICATION

#include <string>



bool registration(std::string, std::string);
bool loginCheck(std::string, std::string);
void login(bool& check);
void registering();

#endif