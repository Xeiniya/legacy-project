#include <gtest/gtest.h>
 #include "../includes/authentication.h"
 #include <string>
 
 class AuthenticationTest : public ::testing::Test {
 protected:
     Authentication* auth;
 
     void SetUp() override {
         auth = new Authentication();
     }
 
     void TearDown() override {
         delete auth;
     }
 };
 
 // Test user registration
 TEST_F(AuthenticationTest, RegisterUserTest) {
     std::string username = "testuser";
     std::string password = "password123";
     std::string role = "user";
     
     ASSERT_TRUE(auth->registerUser(username, password, role));
     
     ASSERT_TRUE(auth->login(username, password));
     ASSERT_TRUE(auth->isLoggedIn());
     ASSERT_EQ(auth->getCurrentUser(), username);
     ASSERT_EQ(auth->getCurrentRole(), role);
 }
 
 // Test user login failure
 TEST_F(AuthenticationTest, LoginFailureTest) {
     std::string username = "nonexistent";
     std::string password = "wrongpassword";
     
     ASSERT_FALSE(auth->login(username, password));
     ASSERT_FALSE(auth->isLoggedIn());
     ASSERT_TRUE(auth->getCurrentUser().empty());
     ASSERT_TRUE(auth->getCurrentRole().empty());
 }
 
 // Test user logout
 TEST_F(AuthenticationTest, LogoutTest) {
     std::string username = "testuser";
     std::string password = "password123";
     std::string role = "user";
     
     auth->registerUser(username, password, role);
     auth->login(username, password);
     ASSERT_TRUE(auth->isLoggedIn());
     
     auth->logout();
     ASSERT_FALSE(auth->isLoggedIn());
     ASSERT_TRUE(auth->getCurrentUser().empty());
     ASSERT_TRUE(auth->getCurrentRole().empty());
 }
 
 // Test password hashing
 TEST_F(AuthenticationTest, PasswordHashingTest) {
     std::string username1 = "user1";
     std::string username2 = "user2";
     std::string password = "samepassword";
     std::string role = "user";
     
     auth->registerUser(username1, password, role);
     auth->registerUser(username2, password, role);
     
     ASSERT_TRUE(auth->login(username1, password));
     auth->logout();
     ASSERT_TRUE(auth->login(username2, password));
 }
 
TEST_F(AuthenticationTest, RoleBasedAccessTest) {
    
    // Login as admin
    ASSERT_TRUE(auth->login("admin", "admin123")); 
    ASSERT_TRUE(auth->hasAccess("update_flower_price"));
    ASSERT_TRUE(auth->hasAccess("view_compositions"));
    
    auth->logout();
    
    // Login as regular user
    ASSERT_TRUE(auth->login("user", "user123"));
    ASSERT_FALSE(auth->hasAccess("update_flower_price"));
    ASSERT_TRUE(auth->hasAccess("view_compositions"));
}

 
 // Test duplicate registration
 TEST_F(AuthenticationTest, DuplicateRegistrationTest) {
     std::string username = "testuser";
     std::string password = "password123";
     std::string role = "user";
     
     ASSERT_TRUE(auth->registerUser(username, password, role));
     ASSERT_FALSE(auth->registerUser(username, "differentpassword", role));
 }
