#include "../includes/authentication.h"
#include <iostream>
#include <functional>

Authentication::Authentication() : loggedIn_(false), currentUser_(""), currentRole_("") {
    // Add default users for testing - store HASHED passwords
    users_["admin"] = {hashPassword("admin123"), "admin"};
    users_["user"] = {hashPassword("user123"), "user"};
    
    // Set up permissions
    permissions_["admin"] = {"view_flowers", "update_flower_price", "view_compositions", 
                            "view_orders", "create_order", "view_reports"};
    permissions_["user"] = {"view_flowers", "view_compositions", "view_own_orders", "create_order"};
}

Authentication::~Authentication() {}

bool Authentication::registerUser(const std::string& username, const std::string& password, const std::string& role) {
    if (users_.find(username) != users_.end()) {
        std::cout << "User already exists" << std::endl;
        return false;
    }
    
    // Store hashed password
    users_[username] = {hashPassword(password), role};
    return true;
}

bool Authentication::login(const std::string& username, const std::string& password) {
    auto it = users_.find(username);
    if (it == users_.end()) {
        return false;
    }
    
    if (it->second.first == hashPassword(password)) {
        loggedIn_ = true;
        currentUser_ = username;
        currentRole_ = it->second.second;
        return true;
    }
    
    return false;
}

bool Authentication::isLoggedIn() const {
    return loggedIn_;
}

std::string Authentication::getCurrentUser() const {
    return currentUser_;
}

std::string Authentication::getCurrentRole() const {
    return currentRole_;
}

void Authentication::logout() {
    loggedIn_ = false;
    currentUser_ = "";
    currentRole_ = "";
}

bool Authentication::hasAccess(const std::string& operation) const {
    if (!loggedIn_) {
        return false;
    }
    
    auto it = permissions_.find(currentRole_);
    if (it == permissions_.end()) {
        return false;
    }
    
    const auto& rolePerm = it->second;
    return std::find(rolePerm.begin(), rolePerm.end(), operation) != rolePerm.end();
}

std::string Authentication::hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}
