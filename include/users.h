#ifndef USERS_H
#define USERS_H

#include "database.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

class Users {     
private:
    std::string generateUsername(std::string name);
    bool isValidRole(std::string role);

public:
    // Admin Only
    std::map<std::string, std::string> registerUser(std::string name, std::string role);
    bool updateRole(int id, std::string newRole);
    std::vector<std::map<std::string, std::string>> userList();
};

#endif