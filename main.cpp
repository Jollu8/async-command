#include <cassert>
#include <thread>
#include "async.h"

int main()
{
//    // Example from homework_08 (another thread)
    auto task = []() {
//        std::cout << "hello" << std::endl;

        std::size_t bulk = 5;

        auto h1 = async::connect(bulk);
        auto h2 = async::connect(bulk);

        async::receive(h1, "1", 1);
        async::receive(h2, "1\n", 2);
        async::receive(h1, "\n2\n3\n4\n5\n6\n{\na\n", 15);
        async::receive(h1, "b\nc\nd\n}\n89\n", 11);

        async::disconnect(h1);
        async::disconnect(h2);
    };
    std::thread t1(task);

//    // Example from homework_06
    auto h3 = async::connect(3);
    async::receive(h3, "cmd1\ncmd2\ncmd3\nc", 16);
    auto h4 = async::connect(3);
    async::receive(h3, "md4\ncmd5\n", 9);
    async::receive(h4,"cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\ncmd5\ncmd6\n{\ncmd7\n"
                      "cmd8\n}\ncmd9\n}\n{\ncmd10\ncmd11\n", 71);
    async::disconnect(h3);
    async::disconnect(h4);

    t1.join();

    return 0;
}