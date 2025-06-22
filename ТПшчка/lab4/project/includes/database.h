#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class Database {
public:
    Database(const std::string& dbPath);
    ~Database();

    // Connection management
    bool connect();
    void disconnect();
    bool isConnected() const;

    // User authentication
    bool authenticateUser(const std::string& username, const std::string& password);

    // Flower operations
    struct Flower {
        int id;
        std::string name;
        std::string variety;
        double price;
    };

    std::vector<Flower> getAllFlowers();
    bool updateFlowerPrice(int flowerId, double newPrice);
    
    // Composition operations
    struct Composition {
        int id;
        std::string name;
        std::string description;
    };

    std::vector<Composition> getAllCompositions();
    std::map<int, int> getCompositionFlowers(int compositionId);
    Composition getMostPopularComposition();
    
    // Customer operations
    struct Customer {
        int id;
        std::string name;
        std::string phone;
        std::string email;
    };

    std::vector<Customer> getAllCustomers();
    Customer getCustomerById(int customerId);
    
    // Order operations
    struct Order {
        int id;
        int customerId;
        int compositionId;
        std::string orderDate;
        std::string fulfillmentDate;
        int quantity;
        double urgencyRate;
    };

    struct OrderSummary {
        int orderId;
        double basePrice;
        double urgencyFee;
        double totalPrice;
    };

    bool createOrder(int customerId, int compositionId, const std::string& orderDate, 
                    const std::string& fulfillmentDate, int quantity);
    std::vector<Order> getOrdersByDate(const std::string& date);
    std::vector<Order> getOrdersByDateRange(const std::string& startDate, const std::string& endDate);
    OrderSummary getOrderSummary(int orderId);
    double getTotalRevenue(const std::string& startDate, const std::string& endDate);
    std::vector<std::pair<int, int>> getOrdersByUrgency();
    std::map<std::string, std::map<std::string, int>> getFlowerUsageByPeriod(
        const std::string& startDate, const std::string& endDate);
    std::map<std::string, std::pair<int, double>> getCompositionSalesSummary();

private:
    std::string dbPath_;
    sqlite3* db_;
    bool connected_;

    // Helper methods for executing SQL
    bool executeSQL(const std::string& sql);
    bool executeSQLWithCallback(const std::string& sql, sqlite3_callback callback, void* data);
};
