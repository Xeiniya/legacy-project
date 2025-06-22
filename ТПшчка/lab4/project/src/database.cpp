#include "../includes/database.h"
#include <iostream>
#include <ctime>

// Callback function for SQLite
static int callbackWrapper(void* data, int argc, char** argv, char** azColName) {
    auto* rows = static_cast<std::vector<std::vector<std::string>>*>(data);
    std::vector<std::string> row;
    
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    
    rows->push_back(row);
    return 0;
}

Database::Database(const std::string& dbPath) : dbPath_(dbPath), db_(nullptr), connected_(false) {}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    if (connected_) {
        return true;
    }
    
    int rc = sqlite3_open(dbPath_.c_str(), &db_);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    connected_ = true;
    return true;
}

void Database::disconnect() {
    if (connected_ && db_) {
        sqlite3_close(db_);
        db_ = nullptr;
        connected_ = false;
    }
}

bool Database::isConnected() const {
    return connected_;
}

bool Database::authenticateUser(const std::string& username, const std::string& password) {    
    std::string sql = "SELECT CustomerID FROM Customers WHERE CustomerName = '" + username + "'";
    std::vector<std::vector<std::string>> results;
    
    bool success = executeSQLWithCallback(sql, callbackWrapper, &results);
    
    return success && !results.empty();
}

std::vector<Database::Flower> Database::getAllFlowers() {
    std::vector<Flower> flowers;
    std::string sql = "SELECT FlowerID, FlowerName, Variety, Price FROM Flowers";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 4) {
                Flower flower;
                flower.id = std::stoi(row[0]);
                flower.name = row[1];
                flower.variety = row[2];
                flower.price = std::stod(row[3]);
                flowers.push_back(flower);
            }
        }
    }
    
    return flowers;
}

bool Database::updateFlowerPrice(int flowerId, double newPrice) {
    // Get current price
    std::string checkSql = "SELECT Price FROM Flowers WHERE FlowerID = " + std::to_string(flowerId);
    std::vector<std::vector<std::string>> results;
    
    if (!executeSQLWithCallback(checkSql, callbackWrapper, &results) || results.empty()) {
        return false;
    }
    
    double currentPrice = std::stod(results[0][0]);
   
    if (newPrice > currentPrice * 1.1) {
        std::cout << "Price increase cannot exceed 10%" << std::endl;
        return false;
    }
    
    std::string sql = "UPDATE Flowers SET Price = " + std::to_string(newPrice) + 
                      " WHERE FlowerID = " + std::to_string(flowerId);
    
    return executeSQL(sql);
}

std::vector<Database::Composition> Database::getAllCompositions() {
    std::vector<Composition> compositions;
    std::string sql = "SELECT CompositionID, CompositionName, Description FROM Compositions";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 3) {
                Composition comp;
                comp.id = std::stoi(row[0]);
                comp.name = row[1];
                comp.description = row[2];
                compositions.push_back(comp);
            }
        }
    }
    
    return compositions;
}

std::map<int, int> Database::getCompositionFlowers(int compositionId) {
    std::map<int, int> flowerQuantities;
    std::string sql = "SELECT FlowerID, Quantity FROM CompositionFlowers WHERE CompositionID = " + 
                      std::to_string(compositionId);
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 2) {
                int flowerId = std::stoi(row[0]);
                int quantity = std::stoi(row[1]);
                flowerQuantities[flowerId] = quantity;
            }
        }
    }
    
    return flowerQuantities;
}

Database::Composition Database::getMostPopularComposition() {
    Composition mostPopular;
    std::string sql = "SELECT c.CompositionID, c.CompositionName, c.Description, COUNT(o.OrderID) as OrderCount "
                      "FROM Compositions c "
                      "JOIN Orders o ON c.CompositionID = o.CompositionID "
                      "GROUP BY c.CompositionID "
                      "ORDER BY OrderCount DESC LIMIT 1";
    
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results) && !results.empty()) {
        mostPopular.id = std::stoi(results[0][0]);
        mostPopular.name = results[0][1];
        mostPopular.description = results[0][2];
    }
    
    return mostPopular;
}

std::vector<Database::Customer> Database::getAllCustomers() {
    std::vector<Customer> customers;
    std::string sql = "SELECT CustomerID, CustomerName, PhoneNumber, Email FROM Customers";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 4) {
                Customer customer;
                customer.id = std::stoi(row[0]);
                customer.name = row[1];
                customer.phone = row[2];
                customer.email = row[3];
                customers.push_back(customer);
            }
        }
    }
    
    return customers;
}

Database::Customer Database::getCustomerById(int customerId) {
    Customer customer;
    std::string sql = "SELECT CustomerID, CustomerName, PhoneNumber, Email FROM Customers WHERE CustomerID = " + 
                      std::to_string(customerId);
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results) && !results.empty()) {
        customer.id = std::stoi(results[0][0]);
        customer.name = results[0][1];
        customer.phone = results[0][2];
        customer.email = results[0][3];
    }
    
    return customer;
}

bool Database::createOrder(int customerId, int compositionId, const std::string& orderDate, 
                          const std::string& fulfillmentDate, int quantity) {
    std::string sql = "INSERT INTO Orders (CustomerID, CompositionID, OrderDate, FulfillmentDate, Quantity, UrgencyRate) "
                      "VALUES (" + std::to_string(customerId) + ", " +
                                   std::to_string(compositionId) + ", '" +
                                   orderDate + "', '" +
                                   fulfillmentDate + "', " +
                                   std::to_string(quantity) + ", 0)";
    
    return executeSQL(sql);
}

std::vector<Database::Order> Database::getOrdersByDate(const std::string& date) {
    std::vector<Order> orders;
    std::string sql = "SELECT OrderID, CustomerID, CompositionID, OrderDate, FulfillmentDate, Quantity, UrgencyRate "
                      "FROM Orders WHERE OrderDate = '" + date + "'";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 7) {
                Order order;
                order.id = std::stoi(row[0]);
                order.customerId = std::stoi(row[1]);
                order.compositionId = std::stoi(row[2]);
                order.orderDate = row[3];
                order.fulfillmentDate = row[4];
                order.quantity = std::stoi(row[5]);
                order.urgencyRate = std::stod(row[6]);
                orders.push_back(order);
            }
        }
    }
    
    return orders;
}

std::vector<Database::Order> Database::getOrdersByDateRange(const std::string& startDate, const std::string& endDate) {
    std::vector<Order> orders;
    std::string sql = "SELECT OrderID, CustomerID, CompositionID, OrderDate, FulfillmentDate, Quantity, UrgencyRate "
                      "FROM Orders WHERE OrderDate BETWEEN '" + startDate + "' AND '" + endDate + "'";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 7) {
                Order order;
                order.id = std::stoi(row[0]);
                order.customerId = std::stoi(row[1]);
                order.compositionId = std::stoi(row[2]);
                order.orderDate = row[3];
                order.fulfillmentDate = row[4];
                order.quantity = std::stoi(row[5]);
                order.urgencyRate = std::stod(row[6]);
                orders.push_back(order);
            }
        }
    }
    
    return orders;
}

Database::OrderSummary Database::getOrderSummary(int orderId) {
    OrderSummary summary;
    std::string sql = "SELECT OrderID, BasePrice, UrgencyFee, TotalPrice FROM OrderSummary WHERE OrderID = " + 
                      std::to_string(orderId);
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results) && !results.empty()) {
        summary.orderId = std::stoi(results[0][0]);
        summary.basePrice = std::stod(results[0][1]);
        summary.urgencyFee = std::stod(results[0][2]);
        summary.totalPrice = std::stod(results[0][3]);
    }
    
    return summary;
}

double Database::getTotalRevenue(const std::string& startDate, const std::string& endDate) {
    double total = 0.0;
    std::string sql = "SELECT SUM(os.TotalPrice) "
                      "FROM OrderSummary os "
                      "JOIN Orders o ON os.OrderID = o.OrderID "
                      "WHERE o.OrderDate BETWEEN '" + startDate + "' AND '" + endDate + "'";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results) && !results.empty() && results[0][0] != "NULL") {
        total = std::stod(results[0][0]);
    }
    
    return total;
}

std::vector<std::pair<int, int>> Database::getOrdersByUrgency() {
    std::vector<std::pair<int, int>> urgencyStats;
    std::string sql = "SELECT UrgencyRate, COUNT(OrderID) "
                      "FROM Orders "
                      "GROUP BY UrgencyRate";
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 2) {
                double urgencyRate = std::stod(row[0]);
                int count = std::stoi(row[1]);
                
                // Convert urgency rate to a percentage
                int urgencyPercent = static_cast<int>(urgencyRate * 100);
                urgencyStats.push_back({urgencyPercent, count});
            }
        }
    }
    
    return urgencyStats;
}

std::map<std::string, std::map<std::string, int>> Database::getFlowerUsageByPeriod(
    const std::string& startDate, const std::string& endDate) {
    
    std::map<std::string, std::map<std::string, int>> flowerUsage;
    std::string sql = "SELECT f.FlowerName, f.Variety, SUM(cf.Quantity * o.Quantity) as TotalUsed "
                      "FROM Orders o "
                      "JOIN CompositionFlowers cf ON o.CompositionID = cf.CompositionID "
                      "JOIN Flowers f ON cf.FlowerID = f.FlowerID "
                      "WHERE o.OrderDate BETWEEN '" + startDate + "' AND '" + endDate + "' "
                      "GROUP BY f.FlowerName, f.Variety";
    
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 3) {
                std::string flowerName = row[0];
                std::string variety = row[1];
                int quantity = std::stoi(row[2]);
                
                flowerUsage[flowerName][variety] = quantity;
            }
        }
    }
    
    return flowerUsage;
}

std::map<std::string, std::pair<int, double>> Database::getCompositionSalesSummary() {
    std::map<std::string, std::pair<int, double>> salesSummary;
    std::string sql = "SELECT c.CompositionName, COUNT(o.OrderID) as OrderCount, SUM(os.TotalPrice) as TotalRevenue "
                      "FROM Compositions c "
                      "JOIN Orders o ON c.CompositionID = o.CompositionID "
                      "JOIN OrderSummary os ON o.OrderID = os.OrderID "
                      "GROUP BY c.CompositionName";
    
    std::vector<std::vector<std::string>> results;
    
    if (executeSQLWithCallback(sql, callbackWrapper, &results)) {
        for (const auto& row : results) {
            if (row.size() >= 3) {
                std::string compositionName = row[0];
                int orderCount = std::stoi(row[1]);
                double totalRevenue = std::stod(row[2]);
                
                salesSummary[compositionName] = {orderCount, totalRevenue};
            }
        }
    }
    
    return salesSummary;
}

bool Database::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool Database::executeSQLWithCallback(const std::string& sql, sqlite3_callback callback, void* data) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), callback, data, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}
