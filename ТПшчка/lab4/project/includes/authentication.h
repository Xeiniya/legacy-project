#pragma once

#include <string>
#include <vector>
#include <map>

class Authentication {
public:
    Authentication();
    ~Authentication();

    bool registerUser(const std::string& username, const std::string& password, const std::string& role);
    bool login(const std::string& username, const std::string& password);
    bool isLoggedIn() const;
    std::string getCurrentUser() const;
    std::string getCurrentRole() const;
    void logout();

    bool hasAccess(const std::string& operation) const;

private:
    bool loggedIn_;
    std::string currentUser_;
    std::string currentRole_;
    std::map<std::string, std::pair<std::string, std::string>> users_; // username -> (password, role)
    std::map<std::string, std::vector<std::string>> permissions_; // role -> operations
    
    // Password hashing
    std::string hashPassword(const std::string& password);
};
