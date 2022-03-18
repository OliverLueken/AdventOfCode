#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <complex>

#include "algorithms.hpp"

namespace Utilities{

    template<class T = size_t>
    using Position = std::pair<T, T>;

    /*
    Takes a string s and returns a new string containing every character of s converted to uppercase
    */
    struct toupper_{
        auto
        operator()(const std::string& s) const {
            std::string upper{};
            std::ranges::transform(s, std::back_inserter(upper), [](const auto c){return std::toupper(c);});
            return upper;
        }
    };
    inline constexpr toupper_ toupper;

    /*
    Takes a string s and returns true if every character of s is a digit, false otherwise
    */
    constexpr bool
    isNumber(const std::string& s){
        auto isDigit = [](const auto c){ return std::isdigit(c); };
        return std::ranges::all_of(s, isDigit);
    }

    /*
    Takes a string s and returns true if every character of s is a hexadecimal digit, false otherwise
    */
    constexpr bool
    isHexNumber(const std::string& s){
        auto isHex = [](const auto c){ return std::isxdigit(c); };
        return std::ranges::all_of(s, isHex);
    }

    /*
    Takes a value val and returns true if val is in the range [left, right), false otherwise
    */
    template<typename T>
    requires std::totally_ordered<T>
    constexpr bool
    isBetween(const T& val, const T& left, const T& right ){
        return left <= val && val < right;
    }

    template <typename T>
    T sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    struct solveCRM_{
        auto operator()(const std::vector<std::pair<int,int>>& n_and_b_pairs) const {
            const auto getXi = [](const auto& N, const auto& n){
                for(auto x=0u; std::cmp_less(x,n) ; ++x){
                    if((x*N)%n==1) return x;
                }
                return 0u;
            };
            const auto& n_range = n_and_b_pairs | std::views::elements<0>;
            const auto N = std::reduce(std::begin(n_range), std::end(n_range), 1ul, std::multiplies<>());
            auto x = 0ul;
            for(const auto& p : n_and_b_pairs){
                const auto N_i = N/p.first;
                const auto x_i = getXi(N_i, p.first);
                x+=x_i*N_i*p.second;
            }
            return x%N;
        }
    };
    inline constexpr solveCRM_ solveCRM;
}

template<class T>
struct std::hash<Utilities::Position<T>>{
    size_t operator()(const Utilities::Position<T>& position) const noexcept{
        const std::size_t h1 = std::hash<T>{}(position.first);
        const std::size_t h2 = std::hash<T>{}(position.second);
        return h1^(h2<<1);
    }
};

template<class T>
struct std::hash<std::complex<T>>{
    std::size_t operator()(const std::complex<T>& z) const noexcept{
        const std::size_t h1 = std::hash<T>{}(z.real());
        const std::size_t h2 = std::hash<T>{}(z.imag());
        return h1^(h2<<1);
    }
};


template<typename T, typename S>
concept Addable = requires (T x, S y){ x + y; };

template<typename T, typename S>
concept Subtractable = requires (T x, S y){ x - y; };

template<typename T, typename S>
requires Addable<T, S>
constexpr Utilities::Position<T>
operator+(const Utilities::Position<T>& lhs, const Utilities::Position<S>& rhs){
    return Utilities::Position<T>{lhs.first+rhs.first, lhs.second+rhs.second};
}

template<typename T, typename S>
requires Subtractable<T, S>
constexpr Utilities::Position<T>
 operator-(const Utilities::Position<T>& lhs, const Utilities::Position<S>& rhs){
    return Utilities::Position<T>{lhs.first-rhs.first, lhs.second-rhs.second};
}

template<typename T>
constexpr Utilities::Position<T>
operator/(const Utilities::Position<T>& numerator, const T& divisor){
    return Utilities::Position<T>{numerator.first/divisor, numerator.second/divisor};
}

template<typename T>
requires Addable<T, T>
constexpr T
abs(const Utilities::Position<T>& position){
    return std::abs(position.first)+std::abs(position.second);
}

#endif