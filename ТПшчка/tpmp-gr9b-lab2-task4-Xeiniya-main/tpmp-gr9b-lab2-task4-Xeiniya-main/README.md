# Car Owners Project

## Overview
This project processes text files containing car owner data and extracts information about vehicles of a specific brand ("Ваз").

## Usage
1. Ensure you have `make` installed.
2. Run `make` to compile the project.
3. Execute `./bin/car_owners` to run the program.
4. The program will:
   - Create a sample data file (`data.txt`).
   - Display its content.
   - Filter owners of "Ваз" cars.
   - Save the filtered data to `output.txt`.
   - Display the result on the screen.

## Building
- Compile the project using:
  ```sh
  make
  ```
- Clean build artifacts using:
  ```sh
  make clean
  ```


## Additional Notes
The program is structured according to the KIS model, with clear separation between:
- Interface (structure definitions)
- Server (data processing functions)
- Client (user interaction)

Project structure follows the required format:
```
car_owners_project/
├── bin/          # Executable files
├── doc/          # Documentation
├── include/      # Header files
├── obj/          # Object files
├── src/          # Source code
├── data.txt         # Test data
└── Makefile      # Build script
```

Links to related repositories:
- [Repository from Task 1](https://github.com/Xeiniya/lab3-task1-gr9-nikolayeva)
- [Repository from Task 2](https://github.com/Xeiniya/tpmp-lab2-task2)
- [Repository from Task 3](https://github.com/fpmi-tp2025/tpmp-gr9b-lab2-task3-Xeiniya)
