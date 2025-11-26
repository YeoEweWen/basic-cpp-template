//#include "database.h"
#include "auth.h"
#include "users.h"
#include "functions.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    cout << "##### Basic C++ Template #####\n" << endl;

    string username, password;

    cout << "----- Try Username: ali1787, Password: ali1787 -----" << endl;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    cout << "\n---- Result ----" << endl;
    Auth::LoginStatus loginStatus = Auth::login(username, password);

    cout << "Success: " << (loginStatus.success ? "Yes" : "No") << endl;
    cout << "Error Code: " << loginStatus.errorCode << endl;
    cout << "Description: " << loginStatus.description << endl;
    cout << "Using Preset Password: " << (loginStatus.usingPresetPassword ? "Yes" : "No") << endl;

    /*
    // Example
    Users users;

    // Register new user
    users.registerUser("Haikal bin Zul", "Admin");

    // Retrieve list (using vector)
    vector<map<string, string>> list = users.userList();
    printVectorValues(list);
    */
    

    system("pause"); // waits for a keypress

    return 0;
}
