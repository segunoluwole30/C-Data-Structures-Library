#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    /* TODO */
    size_t _hash = 0;
    size_t p = 1;
    for (size_t i = 0; i < str.length();i++)
    {
        _hash += str[i] * p;
        p = (p * 19) % 3298534883309ul;
    }
    return _hash;
}

size_t fnv1a_hash::operator() (std::string const & str) const {
    /* TODO */
    size_t _hash = 0xCBF29CE484222325;
    for (size_t i = 0; i < str.length();i++)
    {
        _hash = _hash ^ str[i];
        _hash = _hash * 0x00000100000001B3;
    }
    return _hash;
}
