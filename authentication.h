#ifndef AUTH_H
#define AUTH_H

#include <string>

bool registerUser(std::string, std::string);
bool checkLogin(std::string, std::string);
void login(bool& check);
void makeAccount();

#endif
