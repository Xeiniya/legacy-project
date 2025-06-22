#include <gtest/gtest.h>
#include "../includes/database.h"
#include <string>
#include <vector>
#include <cstdio>
#include <fstream>

// Test database file
const std::string TEST_DB_PATH = "test_flower.db";

// Helper function to create a test database
void createTestDatabase() {
    std::ifstream src("flower.db", std::ios::binary);
    std::ofstream dst(TEST_DB_PATH, std::ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

class DatabaseTest : public ::testing::Test {
protected:
    Database* db;

    void SetUp() override {
        createTestDatabase();
        db = new Database(TEST_DB_PATH);
        db->connect();
    }

    void TearDown() override {
        db->disconnect();
        delete db;
        std::remove(TEST_DB_PATH.c_str());
    }
};

// Test database connection
TEST_F(DatabaseTest, ConnectionTest) {
    ASSERT_TRUE(db->isConnected());
    db->disconnect();
    ASSERT_FALSE(db->isConnected());
    ASSERT_TRUE(db->connect());
}

// Test fetching all flowers
TEST_F(DatabaseTest, GetAllFlowersTest) {
    std::vector<Database::Flower> flowers = db->getAllFlowers();
    ASSERT_GT(flowers.size(), 0);
    
    ASSERT_GT(flowers[0].id, 0);
    ASSERT_FALSE(flowers[0].name.empty());
    ASSERT_FALSE(flowers[0].variety.empty());
    ASSERT_GT(flowers[0].price, 0.0);
}

// Test updating flower price
TEST_F(DatabaseTest, UpdateFlowerPriceTest) {
    std::vector<Database::Flower> flowers = db->getAllFlowers();
    ASSERT_GT(flowers.size(), 0);
    
    int flowerId = flowers[0].id;
    double originalPrice = flowers[0].price;
    double newPrice = originalPrice * 1.05;
    
    ASSERT_TRUE(db->updateFlowerPrice(flowerId, newPrice));
    
    flowers = db->getAllFlowers();
    for (const auto& flower : flowers) {
        if (flower.id == flowerId) {
            ASSERT_NEAR(flower.price, newPrice, 0.01);
            break;
        }
    }
}

// Test getting all compositions
TEST_F(DatabaseTest, GetAllCompositionsTest) {
    std::vector<Database::Composition> compositions = db->getAllCompositions();
    ASSERT_GT(compositions.size(), 0);
    
    ASSERT_GT(compositions[0].id, 0);
    ASSERT_FALSE(compositions[0].name.empty());
}

// Test getting composition flowers
TEST_F(DatabaseTest, GetCompositionFlowersTest) {
    std::vector<Database::Composition> compositions = db->getAllCompositions();
    ASSERT_GT(compositions.size(), 0);
    
    int compositionId = compositions[0].id;
    std::map<int, int> flowerQuantities = db->getCompositionFlowers(compositionId);
    
    ASSERT_GT(flowerQuantities.size(), 0);
}

// Test getting most popular composition
TEST_F(DatabaseTest, GetMostPopularCompositionTest) {
    Database::Composition popularComposition = db->getMostPopularComposition();
    ASSERT_GT(popularComposition.id, 0);
    ASSERT_FALSE(popularComposition.name.empty());
}

// Test creating a new order
TEST_F(DatabaseTest, CreateOrderTest) {
    std::vector<Database::Customer> customers = db->getAllCustomers();
    std::vector<Database::Composition> compositions = db->getAllCompositions();
    
    ASSERT_GT(customers.size(), 0);
    ASSERT_GT(compositions.size(), 0);
    
    int customerId = customers[0].id;
    int compositionId = compositions[0].id;
    
    std::string orderDate = "2025-04-21";
    std::string fulfillmentDate = "2025-04-22";
    int quantity = 1;
    
    ASSERT_TRUE(db->createOrder(customerId, compositionId, orderDate, fulfillmentDate, quantity));
    
    std::vector<Database::Order> orders = db->getOrdersByDate(orderDate);
    bool found = false;
    for (const auto& order : orders) {
        if (order.customerId == customerId && order.compositionId == compositionId) {
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found);
}

// Test getting orders by date range
TEST_F(DatabaseTest, GetOrdersByDateRangeTest) {
    std::string startDate = "2025-04-01";
    std::string endDate = "2025-04-20";
    
    std::vector<Database::Order> orders = db->getOrdersByDateRange(startDate, endDate);
    ASSERT_GT(orders.size(), 0);
}

// Test getting order summary
TEST_F(DatabaseTest, GetOrderSummaryTest) {
    std::vector<Database::Order> orders = db->getOrdersByDateRange("2025-04-01", "2025-04-30");
    ASSERT_GT(orders.size(), 0);
    
    int orderId = orders[0].id;
    Database::OrderSummary summary = db->getOrderSummary(orderId);
    
    ASSERT_EQ(summary.orderId, orderId);
    ASSERT_GT(summary.totalPrice, 0.0);
}
