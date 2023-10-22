#include <gtest/gtest.h>

#include <iostream>
#include "handler.h"

TEST(HandlerTest, Empty)
{
    std::ostringstream oss;
    bulk::Handler handler(3);
    handler.RegisterLogger(logger::LogPtr{new logger::Console(oss)});
    handler.ReceiveEof();
    EXPECT_TRUE(oss.str() == "");
}

TEST(HandlerTest, Simple)
{
    std::ostringstream oss;
    bulk::Handler handler(3);
    handler.RegisterLogger(logger::LogPtr{new logger::Console(oss)});
    handler.Receive("cmd1\n", 5);
    handler.ReceiveEof();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Cheat
    EXPECT_TRUE(oss.str() == "bulk: cmd1\n");
}

TEST(HandlerTest, TaskCase1)
{
    std::ostringstream oss;
    bulk::Handler handler(3);
    handler.RegisterLogger(logger::LogPtr{new logger::Console(oss)});
    handler.Receive("cmd1\ncmd2\ncmd3\nc", 16);
    handler.Receive("md4\ncmd5\n", 9);
    handler.ReceiveEof();

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Cheat
//    std::cout << oss.str() << " ****" << std::endl;

    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2, cmd3\n"
                             "bulk: cmd4, cmd5\n");
}

TEST(HandlerTest, TaskCase2)
{
    std::string buffer = {
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd3\n"
            "cmd4\n"
            "}\n"
            "{\n"
            "cmd5\n"
            "cmd6\n"
            "{\n"
            "cmd7\n"
            "cmd8\n"
            "}\n"
            "cmd9\n"
            "}\n"
            "{\n"
            "cmd10\n"
            "cmd11\n"
    };

    std::ostringstream oss;
    bulk::Handler handler(3);
    handler.RegisterLogger(logger::LogPtr{new logger::Console(oss)});
    handler.Receive(buffer.c_str(), buffer.size());
    handler.ReceiveEof();

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Cheat
//    std::cout << oss.str() << " ****" << std::endl;
// ======================================
    std::cout << "\n\n ==========  Begin print ==============\n";
    for(auto i : oss.str()) {
        std::cout << i;
    }

    std::cout << std::endl;
    std::cout << "============= End print==============\n\n";
    // ============================
    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2\n"
                             "bulk: cmd3, cmd4\n"
                             "bulk: cmd5, cmd6, cmd7, cmd8, cmd9\n");
}