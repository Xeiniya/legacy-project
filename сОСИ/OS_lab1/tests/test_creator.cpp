#include <gtest/gtest.h>
#include <fstream>
#include <string>

class Creator {
private:
    std::string binname;
    int record;
public:
    Creator() : binname(""), record(0) {}
    Creator(const std::string& bn, int r) : binname(bn), record(r) {}
    Creator(const Creator& other) : binname(other.binname), record(other.record) {}

    std::string getBinname() const { return binname; }
    int getRecord() const { return record; }

    bool CreateFile() {
        std::ofstream outFile(binname, std::ios::binary);
        return outFile.is_open();
    }
};

TEST(CreatorTest, DefaultConstructor) {
    Creator creator;
    EXPECT_EQ(creator.getBinname(), "");
    EXPECT_EQ(creator.getRecord(), 0);
}

TEST(CreatorTest, ParameterizedConstructor) {
    Creator creator("test.bin", 10);
    EXPECT_EQ(creator.getBinname(), "test.bin");
    EXPECT_EQ(creator.getRecord(), 10);
}

TEST(CreatorTest, CopyConstructor) {
    Creator original("test.bin", 10);
    Creator copy(original);
    EXPECT_EQ(copy.getBinname(), "test.bin");
    EXPECT_EQ(copy.getRecord(), 10);
}

TEST(CreatorTest, CreateFile) {
    Creator creator("test_output.bin", 2);
    EXPECT_TRUE(creator.CreateFile());

    // Check file exists
    std::ifstream testFile("test_output.bin");
    EXPECT_TRUE(testFile.good());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}