#include "copy-on-write_trie.hpp"
#include <string_view>
#include <iostream>

int main()
{
    Trie t;
    t = t.Put("a", 5);
    t = t.Put("ab", 10);
    t = t.Put("abc", 15);
    t = t.Put("ac", 15);
    std::cout << "a is " << *t.Get<int>("a") << std::endl;
    std::cout << "ab is " << *t.Get<int>("ab") << std::endl;
    std::cout << "abc is " << *t.Get<int>("abc") << std::endl;
    std::cout << "ac is " << *t.Get<int>("ac") << std::endl;
    t = t.Remove("ab");
    t = t.Remove("ac");
    std::cout << "a is " << *t.Get<int>("a") << std::endl;
    std::cout << "ab is " << t.Get<int>("ab") << std::endl;
    std::cout << "abc is " << *t.Get<int>("abc") << std::endl;
    std::cout << "ac is " << t.Get<int>("ac") << std::endl;
    return 0;
}