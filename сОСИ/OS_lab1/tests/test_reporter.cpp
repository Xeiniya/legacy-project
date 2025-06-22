#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "Structure.h"

class Reporter {
private:
    std::string binname;
    std::string tname;
    double price;

public:
    Reporter() : binname(""), tname(""), price(0.0) {}
    Reporter(const std::string& bin, const std::string& tn, double p)
        : binname(bin), tname(tn), price(p) {
    }

    std::string getBinname() const { return binname; }
    std::string getTname() const { return tname; }
    double getPrice() const { return price; }

    bool generateReport() {
        // Check if input file exists
        std::ifstream inFile(binname, std::ios::binary);
        if (!inFile) return false;

        // Create output file
        std::string txtname = tname + ".txt";
        std::ofstream outFile(txtname);
        return outFile.is_open();
    }
};

void createTestBinaryFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile) {
        employee e = { 1, "TestName", 40.0 };
        outFile.write(reinterpret_cast<char*>(&e), sizeof(e));
        outFile.close();
    }
}

TEST(ReporterTest, DefaultConstructor) {
    Reporter reporter;
    EXPECT_EQ(reporter.getBinname(), "");
    EXPECT_EQ(reporter.getTname(), "");
    EXPECT_DOUBLE_EQ(reporter.getPrice(), 0.0);
}

TEST(ReporterTest, ParameterizedConstructor) {
    Reporter reporter("data.bin", "report", 10.5);
    EXPECT_EQ(reporter.getBinname(), "data.bin");
    EXPECT_EQ(reporter.getTname(), "report");
    EXPECT_DOUBLE_EQ(reporter.getPrice(), 10.5);
}

TEST(ReporterTest, GenerateReport) {
    // Create a test binary file
    createTestBinaryFile("test_data.bin");

    Reporter reporter("test_data.bin", "test_report", 20.0);
    EXPECT_TRUE(reporter.generateReport());

    // Check if output file exists
    std::ifstream reportFile("test_report.txt");
    EXPECT_TRUE(reportFile.good());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}