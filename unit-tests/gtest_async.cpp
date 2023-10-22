#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <list>
#include <iostream>
#include <fstream>
#include <thread>
#include "async.h"

namespace fs = boost::filesystem;

TEST(Async, LogFiles)
{
fs::remove_all("./log");
std::list<std::string> testSet = {
        "bulk: 1, 2, 3, 4, 5",
        "bulk: 6",
        "bulk: a, b, c, d",
        "bulk: 89",
        "bulk: 1"
};

EXPECT_TRUE(testSet.size() == 5);

// Test from task
std::size_t bulk = 5;

auto h1 = async::connect(bulk);
auto h2 = async::connect(bulk);

async::receive(h1, "1", 1);
async::receive(h2, "1\n", 2);
async::receive(h1, "\n2\n3\n4\n5\n6\n{\na\n", 15);
async::receive(h1, "b\nc\nd\n}\n89\n", 11);


async::disconnect(h1);
async::disconnect(h2);



// Wait for finished all threads
std::this_thread::sleep_for(std::chrono::milliseconds(10));

// Parse all generated files
for (auto &entry : fs::directory_iterator("./log")) {
std::ifstream infile(entry.path().native());
std::string line;

while (std::getline(infile, line)) {

testSet.remove(line);
}

fs::remove(entry);

}


std::cout << "Size after deleted " << testSet.size() <<std::endl;
EXPECT_TRUE(testSet.size() == 0);

}