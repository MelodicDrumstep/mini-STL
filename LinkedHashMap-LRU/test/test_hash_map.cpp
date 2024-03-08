#include <iostream>
#include "../lru/lru.hpp"

using namespace sjtu;

int main() {
    {
        std::cerr << "!!" << std::endl;
        hashmap<int, std::string> map;
        puts("!!");
        map.insert({1, "one"});
        puts("!!!");
        map.insert({2, "two"});
        map.insert({3, "three"});

        if((map.find(1)) -> second != "one" || map.find(2) -> second != "two" || map.find(3) -> second != "three" || map.find(4) != map.end()) {
            std::cout << "InsertAndFind test failed\n";
        }
    }

    // {
    //     hashmap<int, std::string> map;
    //     map.insert({1, "one"});
    //     map.insert({2, "two"});
    //     map.insert({3, "three"});

    //     map.remove(1);
    //     if(map.find(1) != map.end() || map.remove(4)) {
    //         std::cout << "Remove test failed\n";
    //     }
    // }

    // {
    //     hashmap<int, std::string> map;
    //     map.insert({1, "one"});
    //     map.insert({2, "two"});
    //     map.insert({3, "three"});

    //     map.clear();
    //     if(map.find(1) != map.end() || map.find(2) != map.end() || map.find(3) != map.end()) {
    //         std::cout << "Clear test failed\n";
    //     }
    // }

    // {
    //     hashmap<int, std::string> map;
    //     map.insert({1, "one"});
    //     map.insert({2, "two"});
    //     map.insert({3, "three"});

    //     for(auto it = map.begin(); it != map.end(); ++it) {
    //         if(it->first < 1 || it->first > 3) {
    //             std::cout << "Iterator test failed\n";
    //             break;
    //         }
    //     }
    //}

    return 0;
}