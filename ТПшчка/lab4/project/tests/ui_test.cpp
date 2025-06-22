#include <gtest/gtest.h>
#include "../includes/ui.h"
#include "../includes/database.h"
#include "../includes/authentication.h"
#include <string>
#include <sstream>
#include <iostream>

class MockUI : public UI {
public:
    MockUI(Database& db, Authentication& auth) : UI(db, auth) {}
    
    // Override getInput to provide mock input
    std::string getInput(const std::string& prompt) override {
        if (mockInputs.empty()) {
            return "";
        }
        
        std::string input = mockInputs.front();
        mockInputs.pop_front();
        return input;
    }
    
    // Override getIntInput for mock integer input
    int getIntInput(const std::string& prompt) override {
        if (mockInputs.empty()) {
            return 0;
        }
        
        std::string input = mockInputs.front();
        mockInputs.pop_front();
        try {
            return std::stoi(input);
        } catch (...) {
            return 0;
        }
    }
    
    // Override getDoubleInput for mock double input
    double getDoubleInput(const std::string& prompt) override {
        if (mockInputs.empty()) {
            return 0.0;
        }
        
        std::string input = mockInputs.front();
        mockInputs.pop_front();
        try {
            return std::stod(input);
        } catch (...) {
            return 0.0;
        }
    }
    
    // Override clearScreen to do nothing for tests
    void clearScreen() override {}
    
    // Override waitForKey to do nothing for tests
    void waitForKey() override {}
    
    // Add mock inputs to the queue
    void addMockInput(const std::string& input) {
        mockInputs.push_back(input);
    }
    
    // Add multiple mock inputs
    void addMockInputs(const std::vector<std::string>& inputs) {
        for (const auto& input : inputs) {
            mockInputs.push_back(input);
        }
    }
    
private:
    std::deque<std::string> mockInputs;
};

class UITest : public ::testing::Test {
protected:
    Database* db;
    Authentication* auth;
    MockUI* ui;
    
    std::stringstream buffer;
    std::streambuf* oldCoutBuffer;

    void SetUp() override {
        db = new Database("flower.db");
        db->connect();
        
        auth = new Authentication();
        auth->registerUser("admin", "adminpass", "admin");
        auth->registerUser("user", "userpass", "user");
        
        ui = new MockUI(*db, *auth);

        oldCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCoutBuffer);
        
        delete ui;
        delete auth;
        delete db;
    }
    
    std::string getOutput() {
        std::string output = buffer.str();
        buffer.str("");  
        return output;
    }
};

// Test login screen
TEST_F(UITest, LoginScreenTest) {
    ui->addMockInputs({"admin", "adminpass"});
    ui->showLoginScreen();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("Login") != std::string::npos);
    ASSERT_TRUE(auth->isLoggedIn());
    ASSERT_EQ(auth->getCurrentUser(), "admin");
}

// Test flower management display
TEST_F(UITest, DisplayAllFlowersTest) {
    auth->login("admin", "adminpass");
    ui->displayAllFlowers();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("List of Flowers") != std::string::npos);
    ASSERT_TRUE(output.find("ID") != std::string::npos);
    ASSERT_TRUE(output.find("Name") != std::string::npos);
    ASSERT_TRUE(output.find("Variety") != std::string::npos);
    ASSERT_TRUE(output.find("Price") != std::string::npos);
}

// Test composition management display
TEST_F(UITest, DisplayAllCompositionsTest) {
    auth->login("admin", "adminpass");
    ui->displayAllCompositions();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("List of Compositions") != std::string::npos);
    ASSERT_TRUE(output.find("ID") != std::string::npos);
    ASSERT_TRUE(output.find("Name") != std::string::npos);
    ASSERT_TRUE(output.find("Description") != std::string::npos);
}

// Test order creation
TEST_F(UITest, CreateOrderTest) {
    auth->login("user", "userpass");
    ui->addMockInputs({"1", "1", "2025-04-21", "2025-04-22", "1"});
    ui->createNewOrder();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("Order created successfully") != std::string::npos);
}

// Test order statistics display
TEST_F(UITest, DisplayOrderStatisticsTest) {
    auth->login("admin", "adminpass");
    ui->displayOrderStatistics();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("Order Statistics") != std::string::npos);
    ASSERT_TRUE(output.find("Urgency") != std::string::npos);
}

// Test main menu display for admin
TEST_F(UITest, ShowAdminMenuTest) {
    auth->login("admin", "adminpass");
    ui->addMockInput("0");
    ui->showAdminMenu();
    std::string output = getOutput();
    ASSERT_TRUE(output.find("Admin Menu") != std::string::npos);
    ASSERT_TRUE(output.find("Flower Management") != std::string::npos);
    ASSERT_TRUE(output.find("Composition Management") != std::string::npos);
    ASSERT_TRUE(output.find("Order Management") != std::string::npos);
}
