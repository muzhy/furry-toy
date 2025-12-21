#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "LocalFileReader.h"

namespace fs = std::filesystem;

class LocalFileReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建临时文件
        tempFile = fs::temp_directory_path() / "test_file.txt";
        std::ofstream out(tempFile);
        out << "Line 1\nLine 2\nLine 3\n";
        out.close();
    }

    void TearDown() override {
        // 删除临时文件
        fs::remove(tempFile);
    }

    fs::path tempFile;
};

TEST_F(LocalFileReaderTest, OpenAndClose) {
    furry_toy::LocalFileReader reader;
    EXPECT_FALSE(reader.is_open());

    reader.open(tempFile.string());
    EXPECT_TRUE(reader.is_open());

    reader.close();
    EXPECT_FALSE(reader.is_open());
}

TEST_F(LocalFileReaderTest, ReadData) {
    furry_toy::LocalFileReader reader;
    reader.open(tempFile.string());

    std::vector<char> buffer(10, 0);
    std::streamsize bytesRead = reader.read(buffer.data(), 6);
    EXPECT_EQ(bytesRead, 6);
    EXPECT_EQ(std::string(buffer.data(), 6), "Line 1");

    reader.close();
}

TEST_F(LocalFileReaderTest, GetLine) {
    furry_toy::LocalFileReader reader;
    reader.open(tempFile.string());

    auto line1 = reader.getLine();
    ASSERT_TRUE(line1.has_value());
    EXPECT_EQ(*line1, "Line 1");

    auto line2 = reader.getLine();
    ASSERT_TRUE(line2.has_value());
    EXPECT_EQ(*line2, "Line 2");

    auto line3 = reader.getLine();
    ASSERT_TRUE(line3.has_value());
    EXPECT_EQ(*line3, "Line 3");

    auto end = reader.getLine();
    EXPECT_FALSE(end.has_value());

    reader.close();
}

TEST_F(LocalFileReaderTest, Getline) {
    furry_toy::LocalFileReader reader;
    reader.open(tempFile.string());

    std::string line;
    reader.getline(line);
    EXPECT_EQ(line, "Line 1");

    reader.getline(line);
    EXPECT_EQ(line, "Line 2");

    reader.close();
}

TEST_F(LocalFileReaderTest, SeekAndTell) {
    furry_toy::LocalFileReader reader;
    reader.open(tempFile.string());

    reader.seekg(2);
    EXPECT_EQ(reader.tellg(), 2);

    char ch;
    reader.read(&ch, 1);
    EXPECT_EQ(ch, 'n');

    reader.close();
}

TEST_F(LocalFileReaderTest, PeekAndGet) {
    furry_toy::LocalFileReader reader;
    reader.open(tempFile.string());

    int peeked = reader.peek();
    EXPECT_EQ(peeked, 'L');

    int gotten = reader.get();
    EXPECT_EQ(gotten, 'L');

    reader.close();
}

TEST_F(LocalFileReaderTest, OperatorBool) {
    furry_toy::LocalFileReader reader;
    EXPECT_FALSE(reader);

    reader.open(tempFile.string());
    EXPECT_TRUE(reader);

    reader.close();
    EXPECT_FALSE(reader);
}