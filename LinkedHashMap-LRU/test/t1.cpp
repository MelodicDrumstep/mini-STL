#include <iostream>
#include "../lru/lru.hpp"

int main()
{
    sjtu::double_list<int> list;
    list.insert_head(1);
    list.insert_head(2);
    list.insert_tail(3);
    list.delete_head();
    list.delete_tail();
    list.delete_head();
    list.delete_tail();
    list.delete_head();
    list.delete_tail();
    list.delete_head();
    list.delete_tail();
    list.delete_head();
    list.delete_tail();
    // sjtu::hashmap<int, int> map(10);
    // map.insert(sjtu::pair<int, int>(1, 2));
    // map.insert(sjtu::pair<int, int>(2, 3));
    // map.insert(sjtu::pair<int, int>(3, 4));
    // std::cout << (*map.find(1)).second << std::endl;
    // std::cout << (*map.find(2)).second << std::endl;
    //std::cout << (*map.find(4) << std::endl;

    return 0;
}