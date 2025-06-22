#pragma once

#include "database.h"
#include "authentication.h"
#include <string>
#include <deque>

class UI {
private:
    std::deque<std::string> mockInputs;

public:
    UI(Database& db, Authentication& auth);
    
    virtual void start();
    virtual void showMainMenu();
    virtual void showAdminMenu();
    virtual void showUserMenu();
    
    // Authentication UI
    virtual void showLoginScreen();
    virtual void showRegisterScreen();
    
    // Flower management
    virtual void showFlowerManagement();
    virtual void displayAllFlowers();
    virtual void updateFlowerPrice();
    
    // Composition management
    virtual void showCompositionManagement();
    virtual void displayAllCompositions();
    virtual void displayCompositionDetails(int compositionId);
    virtual void displayMostPopularComposition();
    
    // Order management
    virtual void showOrderManagement();
    virtual void createNewOrder();
    virtual void displayOrdersByDate();
    virtual void displayOrdersByPeriod();
    virtual void displayOrderStatistics();
    virtual void displayFlowerUsageReport();
    virtual void displayCompositionSalesReport();
    
    // Helper methods
    virtual std::string getInput(const std::string& prompt);
    virtual int getIntInput(const std::string& prompt);
    virtual double getDoubleInput(const std::string& prompt);
    virtual void clearScreen();
    virtual void waitForKey();

private:
    Database& db_;
    Authentication& auth_;
};
