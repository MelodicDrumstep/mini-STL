#include <iostream>
#include "../trie/solution.hpp"
#include <vector>
#include <string>

void print_trie(std::shared_ptr<sjtu::TrieNode> trienode, std::vector<std::string> path)
{
    if(trienode == nullptr)
    {
        return;
    }
    if(trienode -> is_value_node_)
    {
        std::cout << "value: " << *(trienode -> value_) << std::endl;
        std::cout << "path with size : " << path.size() << std::endl;
        for(auto i : path)
        {
            std::cout << i << " ";
        }
    }

    for(auto & child : root -> children_)
    {
        print_trie(child.second, path);
    }
}