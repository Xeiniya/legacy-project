#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include"Structure.h"


class Reporter {
private:
    std::string binname;    //бин файл
    std::string tname;      //файл отчета
    double price;    //зп/час

public:

    Reporter() : binname(""), tname(""), price(0.0) {}

    Reporter(const std::string& binname,const std::string& tname, double price):binname(binname),tname(tname),price(price) { }
    

    void generateReport() {
        std::ifstream inFile(binname, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error opening input file\n";
            return;
        } 
        std::string txttname = tname + ".txt";
        std::ofstream outFile(txttname);
        if (!outFile) {
            std::cerr << "Error opening report file for writing\n";
            return;
        }

        outFile << "Report of file \"" << binname << "\"\n";
        outFile << "Number of employee,Name of employee,Worked hours,Salary\n";

        std::cout<< "Report file \"" << binname << "\"\n";
        std::cout<< "Number of employee,Name of employee,Worked hours,Salary\n";

        employee e;
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(e))) {
            double salary = e.hours * price;
            outFile << e.num << ". " << e.name << ", " << e.hours << ", " << salary << "\n";
            std::cout<< e.num << "." << e.name << ", " << e.hours << ", " << salary << "\n";
        }

        inFile.close();
        outFile.close();
    }

};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input binary file> <output report file> <hourly rate>\n";
        return 1;
    }

    std::string binname= argv[1];
    std::string tname = argv[2];
    double price = std::stod(argv[3]);

    Reporter reporter(binname,tname, price);
    reporter.generateReport();

    return 0;
}
