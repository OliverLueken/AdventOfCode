#ifndef MYCONCEPTS_HPP
#define MYCONCEPTS_HPP

#include <concepts>

template<typename T>
concept number = std::integral<T> || std::floating_point<T>;

template<typename T, typename S>
concept Addable = requires (T x, S y){ x + y; };

template<typename T, typename S>
concept Subtractable = requires (T x, S y){ x - y; };


template<typename T>
concept Container = requires (T c){c.begin();};

template<typename T>
concept Printable = requires (T x){ std::cout << x; };


#endif