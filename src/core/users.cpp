#include "users.h"
#include "auth.h"
#include "database.h"
#include "functions.h"

using namespace std;


string Users::generateUsername(string name){
    Database db;

    string firstName;
    vector<int> usedUniqueNumbers, availableUniqueNumbers;
    int uniqueNumber;
    string paddedUniqueNumber; // Fill 0 at the front to make it 4 digits

    // Extract the first name and convert it into lower case
    stringstream ss(name);
    ss >> firstName;
    transform(firstName.begin(), firstName.end(), firstName.begin(), ::tolower);

    // Retrieve all the username with same initial first name, and store all the used unique number
    string query = "SELECT username FROM user WHERE username LIKE :username;";
    map<string, string> params = {
        {"username", firstName + "%"}
    };

    vector<map<string, string>> usernames = db.fetchData(query, params);

    for (const auto& row : usernames) {
        uniqueNumber = stoi(row.at("username").substr(firstName.length()));
        usedUniqueNumbers.push_back(uniqueNumber);
    }

    // Generate all available options for the unique number
    for (int i=0; i<10000; i++){
        if (std::find(usedUniqueNumbers.begin(), usedUniqueNumbers.end(), i) != usedUniqueNumbers.end()) {
            continue; // skip this number
        }
        
        availableUniqueNumbers.push_back(i);
    }

    // Pick one unique number
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, availableUniqueNumbers.size() - 1); // Distribution from 0 to vector.size() - 1
    uniqueNumber = availableUniqueNumbers[dis(gen)];

    // Fill the 0 at the front to make it 4 digits.
    paddedUniqueNumber = to_string(uniqueNumber);
    paddedUniqueNumber = string(4 - paddedUniqueNumber.length(), '0') + paddedUniqueNumber;

    return firstName + paddedUniqueNumber;
}

bool Users::isValidRole(string role){
    transform(role.begin(), role.end(), role.begin(), ::tolower); // convert to lowercase
    return ((role == "admin") || (role == "employee"));
}

/*---------- ADMIN ONLY ----------*/
map<string, string> Users::registerUser(string name, string role){
    Database db;

    string username = generateUsername(name);
    string salt = Auth::generateSalt();
    string password = Auth::hashPassword(username, salt); // Default password is using username

    string query = "INSERT INTO user (name, username, role, password, salt) VALUES (:name, :username, :role, :password, :salt);";
    map<string, string> params = {
        {"name", name},
        {"username", username},
        {"role", role},
        {"password", password},
        {"salt", salt},
    };

    if (!db.runQuery(query, params)){
        logError("Failed to register new user.");
        return {
            {"status", "fail"}
        };
    }

    return {
        {"name", name},
        {"username", username},
        {"role", role},
        {"password", password},
        {"status", "success"}
    };
}

bool Users::updateRole(int id, string newRole){
    if (!isValidRole(newRole)){
        logError(newRole + " is not a valid role.");
        return false;
    }

    newRole = toProperCase(newRole);

    Database db;

    string query = "UPDATE user SET role = :role WHERE id = :id AND status = 'Active';";
    map<string, string> params = {
        {"id", to_string(id)},
        {"role", newRole}
    };

    return db.runQuery(query, params);
}

vector<map<string, string>> Users::userList(){
    Database db;

    string query = "SELECT * FROM user;";

    return db.fetchData(query);
}
