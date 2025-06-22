#include "../includes/ui.h"
#include <iostream>
#include <iomanip>
#include <limits>

UI::UI(Database& db, Authentication& auth) : db_(db), auth_(auth) {}

void UI::start() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "    FLOWER GREENHOUSE MANAGEMENT    \n";
    std::cout << "====================================\n\n";
    
    // Trying to connect to the database
    if (!db_.isConnected() && !db_.connect()) {
        std::cout << "Failed to connect to the database. Exiting...\n";
        return;
    }
    
    // Show login screen
    showLoginScreen();
    
    // Main menu based on user role
    if (auth_.isLoggedIn()) {
        if (auth_.getCurrentRole() == "admin") {
            showAdminMenu();
        } else {
            showUserMenu();
        }
    }
}

void UI::showMainMenu() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "    FLOWER GREENHOUSE MANAGEMENT    \n";
    std::cout << "====================================\n";
    std::cout << "Logged in as: " << auth_.getCurrentUser() << " (" << auth_.getCurrentRole() << ")\n\n";
    
    std::cout << "1. Flower Management\n";
    std::cout << "2. Composition Management\n";
    std::cout << "3. Order Management\n";
    std::cout << "4. Logout\n";
    std::cout << "0. Exit\n\n";
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1:
            showFlowerManagement();
            break;
        case 2:
            showCompositionManagement();
            break;
        case 3:
            showOrderManagement();
            break;
        case 4:
            auth_.logout();
            start();
            break;
        case 0:
            std::cout << "Goodbye!\n";
            db_.disconnect();
            exit(0);
        default:
            std::cout << "Invalid choice. Please try again.\n";
            waitForKey();
            showMainMenu();
    }
}

void UI::showAdminMenu() {
    while (auth_.isLoggedIn() && auth_.getCurrentRole() == "admin") {
        clearScreen();
        std::cout << "====================================\n";
        std::cout << "           ADMIN MENU              \n";
        std::cout << "====================================\n";
        std::cout << "Logged in as: " << auth_.getCurrentUser() << " (Administrator)\n\n";
        
        std::cout << "1. View All Flowers\n";
        std::cout << "2. Update Flower Price\n";
        std::cout << "3. View All Compositions\n";
        std::cout << "4. View Most Popular Composition\n";
        std::cout << "5. Create New Order\n";
        std::cout << "6. View Orders by Date\n";
        std::cout << "7. View Total Revenue Report\n";
        std::cout << "8. View Orders by Urgency Report\n";
        std::cout << "9. View Flower Usage Report\n";
        std::cout << "10. View Composition Sales Report\n";
        std::cout << "11. Logout\n";
        std::cout << "0. Exit\n\n";
        
        int choice = getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                displayAllFlowers();
                break;
            case 2:
                updateFlowerPrice();
                break;
            case 3:
                displayAllCompositions();
                break;
            case 4:
                displayMostPopularComposition();
                break;
            case 5:
                createNewOrder();
                break;
            case 6:
                displayOrdersByDate();
                break;
            case 7:
                displayOrdersByPeriod();
                break;
            case 8:
                displayOrderStatistics();
                break;
            case 9:
                displayFlowerUsageReport();
                break;
            case 10:
                displayCompositionSalesReport();
                break;
            case 11:
                auth_.logout();
                start();
                break;
            case 0:
                std::cout << "Goodbye!\n";
                db_.disconnect();
                exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
                waitForKey();
        }
    }
}

void UI::showUserMenu() {
    while (auth_.isLoggedIn() && auth_.getCurrentRole() == "user") {
        clearScreen();
        std::cout << "====================================\n";
        std::cout << "           USER MENU               \n";
        std::cout << "====================================\n";
        std::cout << "Logged in as: " << auth_.getCurrentUser() << "\n\n";
        
        std::cout << "1. View All Flowers\n";
        std::cout << "2. View All Compositions\n";
        std::cout << "3. Create New Order\n";
        std::cout << "4. View My Orders\n";
        std::cout << "5. Logout\n";
        std::cout << "0. Exit\n\n";
        
        int choice = getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                displayAllFlowers();
                break;
            case 2:
                displayAllCompositions();
                break;
            case 3:
                createNewOrder();
                break;
            case 4:
                std::cout << "Feature not implemented yet.\n";
                waitForKey();
                break;
            case 5:
                auth_.logout();
                start();
                break;
            case 0:
                std::cout << "Goodbye!\n";
                db_.disconnect();
                exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
                waitForKey();
        }
    }
}

void UI::showLoginScreen() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "              LOGIN                \n";
    std::cout << "====================================\n\n";
    
    std::string username = getInput("Username: ");
    std::string password = getInput("Password: ");
    
    if (auth_.login(username, password)) {
        std::cout << "Login successful!\n";
    } else {
        std::cout << "Invalid username or password.\n";
        waitForKey();
        return;
    }
}

void UI::showRegisterScreen() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "           REGISTRATION             \n";
    std::cout << "====================================\n\n";
    
    std::string username = getInput("Choose a username: ");
    std::string password = getInput("Choose a password: ");
    std::string role = getInput("Role (user/admin): ");
    
    if (auth_.registerUser(username, password, role)) {
        std::cout << "Registration successful!\n";
    } else {
        std::cout << "Registration failed. Username may already exist.\n";
    }
    waitForKey();
}

void UI::showFlowerManagement() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "        FLOWER MANAGEMENT           \n";
    std::cout << "====================================\n\n";
    
    std::cout << "1. View All Flowers\n";
    std::cout << "2. Update Flower Price\n";
    std::cout << "3. Back to Main Menu\n\n";
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1:
            displayAllFlowers();
            break;
        case 2:
            updateFlowerPrice();
            break;
        case 3:
            if (auth_.getCurrentRole() == "admin") {
                showAdminMenu();
            } else {
                showUserMenu();
            }
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            waitForKey();
            showFlowerManagement();
    }
}

void UI::displayAllFlowers() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "          ALL FLOWERS              \n";
    std::cout << "====================================\n\n";
    
    auto flowers = db_.getAllFlowers();
    
    if (flowers.empty()) {
        std::cout << "No flowers found in the database.\n";
    } else {
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Name" 
                  << std::setw(20) << "Variety" << std::setw(10) << "Price" << std::endl;
        std::cout << std::string(55, '-') << std::endl;
        
        for (const auto& flower : flowers) {
            std::cout << std::left << std::setw(5) << flower.id << std::setw(20) << flower.name 
                      << std::setw(20) << flower.variety << std::setw(10) << flower.price << std::endl;
        }
    }
    
    waitForKey();
    showFlowerManagement();
}

void UI::updateFlowerPrice() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "       UPDATE FLOWER PRICE         \n";
    std::cout << "====================================\n\n";
    
    if (!auth_.hasAccess("update_flower_price")) {
        std::cout << "You don't have permission to update flower prices.\n";
        waitForKey();
        showFlowerManagement();
        return;
    }
    
    displayAllFlowers();
    
    int flowerId = getIntInput("\nEnter Flower ID to update: ");
    double newPrice = getDoubleInput("Enter new price: ");
    
    if (db_.updateFlowerPrice(flowerId, newPrice)) {
        std::cout << "Price updated successfully!\n";
    } else {
        std::cout << "Failed to update price. Please check if the ID is valid or if the price increase is too high.\n";
    }
    
    waitForKey();
    showFlowerManagement();
}

void UI::showCompositionManagement() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "      COMPOSITION MANAGEMENT        \n";
    std::cout << "====================================\n\n";
    
    std::cout << "1. View All Compositions\n";
    std::cout << "2. View Composition Details\n";
    std::cout << "3. View Most Popular Composition\n";
    std::cout << "4. Back to Main Menu\n\n";
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1:
            displayAllCompositions();
            break;
        case 2:
            int compId;
            compId = getIntInput("Enter Composition ID: ");
            displayCompositionDetails(compId);
            break;
        case 3:
            displayMostPopularComposition();
            break;
        case 4:
            if (auth_.getCurrentRole() == "admin") {
                showAdminMenu();
            } else {
                showUserMenu();
            }
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            waitForKey();
            showCompositionManagement();
    }
}

void UI::displayAllCompositions() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "        ALL COMPOSITIONS           \n";
    std::cout << "====================================\n\n";
    
    auto compositions = db_.getAllCompositions();
    
    if (compositions.empty()) {
        std::cout << "No compositions found in the database.\n";
    } else {
        std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" 
                  << std::setw(40) << "Description" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        for (const auto& comp : compositions) {
            std::cout << std::left << std::setw(5) << comp.id << std::setw(25) << comp.name 
                      << std::setw(40) << comp.description << std::endl;
        }
    }
    
    waitForKey();
    showCompositionManagement();
}

void UI::displayCompositionDetails(int compositionId) {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "      COMPOSITION DETAILS          \n";
    std::cout << "====================================\n\n";
    
    auto compositions = db_.getAllCompositions();
    Database::Composition selectedComp;
    bool found = false;
    
    for (const auto& comp : compositions) {
        if (comp.id == compositionId) {
            selectedComp = comp;
            found = true;
            break;
        }
    }
    
    if (!found) {
        std::cout << "Composition not found.\n";
        waitForKey();
        showCompositionManagement();
        return;
    }
    
    std::cout << "ID: " << selectedComp.id << "\n";
    std::cout << "Name: " << selectedComp.name << "\n";
    std::cout << "Description: " << selectedComp.description << "\n\n";
    
    std::cout << "Flowers in this composition:\n";
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "Flower" << std::setw(20) << "Variety" 
              << std::setw(10) << "Quantity" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    auto flowerQuantities = db_.getCompositionFlowers(compositionId);
    auto allFlowers = db_.getAllFlowers();
    
    for (const auto& [flowerId, quantity] : flowerQuantities) {
        for (const auto& flower : allFlowers) {
            if (flower.id == flowerId) {
                std::cout << std::left << std::setw(20) << flower.name << std::setw(20) << flower.variety
                          << std::setw(10) << quantity << std::endl;
                break;
            }
        }
    }
    
    waitForKey();
    showCompositionManagement();
}

void UI::displayMostPopularComposition() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "    MOST POPULAR COMPOSITION       \n";
    std::cout << "====================================\n\n";
    
    auto mostPopular = db_.getMostPopularComposition();
    
    if (mostPopular.id == 0) {
        std::cout << "No compositions or orders found in the database.\n";
    } else {
        std::cout << "ID: " << mostPopular.id << "\n";
        std::cout << "Name: " << mostPopular.name << "\n";
        std::cout << "Description: " << mostPopular.description << "\n\n";
    }
    
    waitForKey();
    showCompositionManagement();
}

void UI::showOrderManagement() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "        ORDER MANAGEMENT           \n";
    std::cout << "====================================\n\n";
    
    std::cout << "1. Create New Order\n";
    std::cout << "2. View Orders by Date\n";
    std::cout << "3. View Orders by Period\n";
    std::cout << "4. View Order Statistics\n";
    std::cout << "5. View Flower Usage Report\n";
    std::cout << "6. View Composition Sales Report\n";
    std::cout << "7. Back to Main Menu\n\n";
    
    int choice = getIntInput("Enter your choice: ");
    
    switch (choice) {
        case 1:
            createNewOrder();
            break;
        case 2:
            displayOrdersByDate();
            break;
        case 3:
            displayOrdersByPeriod();
            break;
        case 4:
            displayOrderStatistics();
            break;
        case 5:
            displayFlowerUsageReport();
            break;
        case 6:
            displayCompositionSalesReport();
            break;
        case 7:
            if (auth_.getCurrentRole() == "admin") {
                showAdminMenu();
            } else {
                showUserMenu();
            }
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            waitForKey();
            showOrderManagement();
    }
}

void UI::createNewOrder() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "          CREATE ORDER             \n";
    std::cout << "====================================\n\n";
    
    // Display customers for selection
    auto customers = db_.getAllCustomers();
    std::cout << "Available Customers:\n";
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Name" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    for (const auto& customer : customers) {
        std::cout << std::left << std::setw(5) << customer.id << std::setw(20) << customer.name << std::endl;
    }
    
    int customerId = getIntInput("\nEnter Customer ID: ");
    
    // Display compositions for selection
    auto compositions = db_.getAllCompositions();
    std::cout << "\nAvailable Compositions:\n";
    std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Name" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    for (const auto& comp : compositions) {
        std::cout << std::left << std::setw(5) << comp.id << std::setw(25) << comp.name << std::endl;
    }
    
    int compositionId = getIntInput("\nEnter Composition ID: ");
    std::string orderDate = getInput("Enter Order Date (YYYY-MM-DD): ");
    std::string fulfillmentDate = getInput("Enter Fulfillment Date (YYYY-MM-DD): ");
    int quantity = getIntInput("Enter Quantity: ");
    
    if (db_.createOrder(customerId, compositionId, orderDate, fulfillmentDate, quantity)) {
        std::cout << "Order created successfully!\n";
    } else {
        std::cout << "Failed to create order. Please check the provided information.\n";
    }
    
    waitForKey();
    showOrderManagement();
}

void UI::displayOrdersByDate() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "         ORDERS BY DATE            \n";
    std::cout << "====================================\n\n";
    
    std::string date = getInput("Enter Date (YYYY-MM-DD): ");
    auto orders = db_.getOrdersByDate(date);
    
    if (orders.empty()) {
        std::cout << "No orders found for the specified date.\n";
    } else {
        std::cout << std::left << std::setw(5) << "ID" << std::setw(12) << "Customer" 
                  << std::setw(12) << "Composition" << std::setw(12) << "Order Date"
                  << std::setw(15) << "Delivery Date" << std::setw(8) << "Quantity" 
                  << std::setw(10) << "Urgency %" << std::endl;
        std::cout << std::string(74, '-') << std::endl;
        
        for (const auto& order : orders) {
            auto customer = db_.getCustomerById(order.customerId);
            auto compositions = db_.getAllCompositions();
            std::string compName = "Unknown";
            
            for (const auto& comp : compositions) {
                if (comp.id == order.compositionId) {
                    compName = comp.name;
                    break;
                }
            }
            
            std::cout << std::left << std::setw(5) << order.id << std::setw(12) << customer.name 
                      << std::setw(12) << compName << std::setw(12) << order.orderDate
                      << std::setw(15) << order.fulfillmentDate << std::setw(8) << order.quantity 
                      << std::setw(10) << (order.urgencyRate * 100) << "%" << std::endl;
        }
    }
    
    waitForKey();
    showOrderManagement();
}

void UI::displayOrdersByPeriod() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "         REVENUE REPORT            \n";
    std::cout << "====================================\n\n";
    
    std::string startDate = getInput("Enter Start Date (YYYY-MM-DD): ");
    std::string endDate = getInput("Enter End Date (YYYY-MM-DD): ");
    
    double totalRevenue = db_.getTotalRevenue(startDate, endDate);
    
    std::cout << "\nTotal Revenue for period " << startDate << " to " << endDate << ": $" 
              << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
    
    waitForKey();
    showOrderManagement();
}

void UI::displayOrderStatistics() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "        URGENCY STATISTICS         \n";
    std::cout << "====================================\n\n";
    
    auto urgencyStats = db_.getOrdersByUrgency();
    
    if (urgencyStats.empty()) {
        std::cout << "No order statistics available.\n";
    } else {
        std::cout << std::left << std::setw(15) << "Urgency Rate" << std::setw(15) << "Order Count" << std::endl;
        std::cout << std::string(30, '-') << std::endl;
        
        for (const auto& [urgencyRate, count] : urgencyStats) {
            std::cout << std::left << std::setw(15) << urgencyRate << "%" << std::setw(15) << count << std::endl;
        }
    }
    
    waitForKey();
    showOrderManagement();
}

void UI::displayFlowerUsageReport() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "       FLOWER USAGE REPORT         \n";
    std::cout << "====================================\n\n";
    
    std::string startDate = getInput("Enter Start Date (YYYY-MM-DD): ");
    std::string endDate = getInput("Enter End Date (YYYY-MM-DD): ");
    
    auto flowerUsage = db_.getFlowerUsageByPeriod(startDate, endDate);
    
    if (flowerUsage.empty()) {
        std::cout << "No flower usage data for the specified period.\n";
    } else {
        std::cout << std::left << std::setw(20) << "Flower" << std::setw(20) << "Variety" 
                  << std::setw(10) << "Quantity" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (const auto& [flowerName, varieties] : flowerUsage) {
            for (const auto& [variety, quantity] : varieties) {
                std::cout << std::left << std::setw(20) << flowerName << std::setw(20) << variety 
                          << std::setw(10) << quantity << std::endl;
            }
        }
    }
    
    waitForKey();
    showOrderManagement();
}

void UI::displayCompositionSalesReport() {
    clearScreen();
    std::cout << "====================================\n";
    std::cout << "    COMPOSITION SALES REPORT       \n";
    std::cout << "====================================\n\n";
    
    auto salesSummary = db_.getCompositionSalesSummary();
    
    if (salesSummary.empty()) {
        std::cout << "No composition sales data available.\n";
    } else {
        std::cout << std::left << std::setw(25) << "Composition" << std::setw(15) << "Orders" 
                  << std::setw(15) << "Revenue" << std::endl;
        std::cout << std::string(55, '-') << std::endl;
        
        for (const auto& [composition, data] : salesSummary) {
            int orderCount = data.first;
            double revenue = data.second;
            
            std::cout << std::left << std::setw(25) << composition << std::setw(15) << orderCount 
                      << "$" << std::fixed << std::setprecision(2) << revenue << std::endl;
        }
    }
    
    waitForKey();
    showOrderManagement();
}

std::string UI::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int UI::getIntInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            return std::stoi(input);
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
}

double UI::getDoubleInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            return std::stod(input);
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    }
}

void UI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void UI::waitForKey() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
