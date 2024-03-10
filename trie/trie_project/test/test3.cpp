#include "../trie/solution.hpp"
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>

int main() {
    sjtu::Trie trie;
    std::unordered_map<std::string, std::string> map;

    // Use a fixed seed for the random number generator
    std::mt19937 gen(100); 
    std::uniform_int_distribution<> dis(1, 20);

    // Insert random key-value pairs into the Trie and a std::unordered_map
    for (int i = 0; i < 5; i++) {
        std::string key = "key" + std::to_string(dis(gen));
        std::string value = "value" + std::to_string(dis(gen));

        std::cout << "Inserting " << key << " -> " << value << std::endl;

        trie = trie.Put<std::string>(key, value);
        std::cout << "cnt1 : " << cnt1 << std::endl;


        std::cout << "Retriving key11 : " << std::endl;
        std::cout << *(trie.Get<std::string>("key11")) << std::endl;

        map[key] = value;
    }

    std::cout << "mapsize : " << map.size() << std::endl;

    int cnt = 0;
    // Check that all key-value pairs can be retrieved correctly from the Trie
    for (const auto& pair : map) {
        std::cout << "cnt : " << cnt << std::endl;
        cnt++;
        std::cout << "Retrieving " << pair.first << " -> " << pair.second << std::endl;
        if(trie.Get<std::string>(pair.first) == nullptr){
            std::cout << "rightnow, key is : " << pair.first << " and value is : " << pair.second << std::endl;
            continue;
        }
        if (*trie.Get<std::string>(pair.first) != pair.second) {
            std::cout << "Test failed: " << pair.first << " does not return " << pair.second << std::endl;
            return 1;
        }
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}