#ifndef POSITION_HPP
#define POSITION_HPP

#include <functional>

#include "concepts.hpp"


namespace Utilities{
    template<typename T>
    class Position{
    public:
        T first{};
        T second{};

        constexpr Position() = default;

        constexpr Position(const T& _first, const T& _second)
        : first{_first}, second{_second}{}

        constexpr Position(T&& _first, T&& _second)
        : first{std::move(_first)}, second{std::move(_second)}{}

        constexpr Position(const std::pair<T, T>& pair)
        : first{pair.first}, second{pair.second}{}

        constexpr auto
        operator==(const Position<T>& rhs) const {
            return first == rhs.first && second == rhs.second;
        }

        constexpr auto
        operator<=>(const Position<T>& rhs) const {
            if (first == rhs.first){
                return second - rhs.second;
            }
            return first - rhs.first;
        }
    };
    // template<class T = size_t>
    // using Position = std::pair<T, T>;

    template<typename T, typename S>
    constexpr auto
    make_position(const T& first, const S& second){
        return Position<typename std::common_type<T, S>::type>{first, second};
    }

    template<typename T, typename S>
    constexpr auto
    make_position(T&& first, S&& second){
        return Position<typename std::common_type<T, S>::type>{std::move(first), std::move(second)};
    }
}

template<class T>
struct std::hash<Utilities::Position<T>>{
        constexpr size_t
        operator()(const Utilities::Position<T>& position) const noexcept{
        const std::size_t h1 = std::hash<T>{}(position.first);
        const std::size_t h2 = std::hash<T>{}(position.second);
        return h1^(h2<<1);
    }
};



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