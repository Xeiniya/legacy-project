#include "../includes/database.h"
#include "../includes/authentication.h"
#include "../includes/ui.h"
#include <iostream>

int main() {
    // Initialize the database with the path to the SQLite file
    Database db("flower.db");
    
    // Initialize authentication system
    Authentication auth;
    
    // Initialize the UI with the database and authentication objects
    UI ui(db, auth);
    
    // Start the UI
    ui.start();
    
    return 0;
}
