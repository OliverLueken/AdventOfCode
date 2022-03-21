#ifndef MYCONCEPTS_HPP
#define MYCONCEPTS_HPP


template<typename T>
concept number = std::integral<T> || std::floating_point<T>;

template<typename T, typename S>
concept Addable = requires (T x, S y){ x + y; };

template<typename T, typename S>
concept Subtractable = requires (T x, S y){ x - y; };

#endif