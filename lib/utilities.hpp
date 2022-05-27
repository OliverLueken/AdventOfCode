#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <complex>
#include <bitset>
#include <concepts>
#include <sstream>

#include "algorithms.hpp"
#include "concepts.hpp"
#include "position.hpp"

namespace Utilities{

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


    struct hexToBin_{
        std::string operator()(const char c) const {
            std::stringstream ss{};
            ss << std::hex <<c;
            unsigned n;
            ss >> n;
            std::bitset<4> binary(n);
            return binary.to_string();
        }

        std::string operator()(const std::string& s) const {
            auto bin = std::stringstream{};
            for(const auto c : s){
                bin << (*this)(c);
            }
            return bin.str();
        }

        std::string operator()(const std::integral auto hex) const {
            return (*this)(std::to_string(hex));
        }
    };
    inline hexToBin_ hexToBin;
}


template<class T>
struct std::hash<std::complex<T>>{
    std::size_t operator()(const std::complex<T>& z) const noexcept{
        const std::size_t h1 = std::hash<T>{}(z.real());
        const std::size_t h2 = std::hash<T>{}(z.imag());
        return h1^(h2<<1);
    }
};



#endif