#include <string>
#include <functional>
#include <vector>

class BloomFilter 
{
private:
    //add whatever you want here
    bool *bit_array;
    int array_size;
    std::vector<std::function<size_t(const std::string&)>> hash_functions;

public:
    BloomFilter(int size, std::function<size_t(const std::string&)> hash1, std::function<size_t(const std::string&)> hash2)
    {
        bit_array = new bool[size];
        array_size = size;
        for (int i = 0; i < size; i++)
        {
            bit_array[i] = false;
        }
        hash_functions.push_back(hash1);   
        hash_functions.push_back(hash2);
    }
    //size is the size of the bit array
    //hash1 and hash2 are the two hash functions
    //In our version of Bloom Filter, we use two hash functions
    //to reduce the possibility of false positives
    //(why can we use this trick?)

    ~BloomFilter()
    {
        delete[] bit_array;
    }

    void add(const std::string& item)
    {
        for (int i = 0; i < hash_functions.size(); i++)
        {
            bit_array[hash_functions[i](item) % array_size] = true;
        }
    }
    //add an item into the filter

    bool possibly_contains(const std::string& item)
    {
        bool result = true;
        for (int i = 0; i < hash_functions.size(); i++)
        {
            result = result && bit_array[hash_functions[i](item) % array_size];
        }
        return result;
    }
    //check if the item exists...possibly!
};

//implement the two hash functions
std::function<size_t(const std::string&)> hash1 = [](const std::string& s) -> size_t
{
    size_t hash = 0;
    for (int i = 0; i < s.length(); i++)
    {
        hash = hash * 31 + s[i];
    }
    return hash;
};
std::function<size_t(const std::string&)> hash2 = [](const std::string& s) -> size_t
{
    size_t hash = 0;
    for (int i = 0; i < s.length(); i++)
    {
        hash = hash * 33 + s[i];
    }
    return hash;
};