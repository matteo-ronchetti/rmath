#include <iostream>
#include <bitset>
#include <sstream>

template <typename T>
std::string bits(T x)
{
    return bits(*(int *)(&x));
}

template <>
std::string bits(int x)
{
    std::string tmp = std::bitset<32>(x).to_string();
    std::stringstream ss;
    ss << tmp[0] << " " << tmp.substr(1, 8) << " " << tmp.substr(8);
    return ss.str();
}
