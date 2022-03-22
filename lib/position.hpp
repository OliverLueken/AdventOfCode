#ifndef POSITION_HPP
#define POSITION_HPP

#include <functional>

#include "concepts.hpp"


namespace Utilities{
    template<typename T>
    class Position{
    public:

        T x{};
        T y{};


        constexpr Position() = default;

        constexpr Position(const T& _x, const T& _y)
        : x{_x}, y{_y}{}

        constexpr Position(T&& _x, T&& _y)
        : x{std::move(_x)}, y{std::move(_y)}{}

        constexpr Position(const std::pair<T, T>& pair)
        : x{pair.first}, y{pair.second}{}

        constexpr auto
        operator==(const Position<T>& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        constexpr auto
        operator<=>(const Position<T>& rhs) const {
            if (x == rhs.x){
                return y - rhs.y;
            }
            return x - rhs.x;
        }
    };

    template<typename T, typename S>
    constexpr auto
    make_position(const T& x, const S& y){
        return Position<typename std::common_type<T, S>::type>{x, y};
    }

    template<typename T, typename S>
    constexpr auto
    make_position(T&& x, S&& y){
        return Position<typename std::common_type<T, S>::type>{std::move(x), std::move(y)};
    }
}

template<class T>
struct std::hash<Utilities::Position<T>>{
        constexpr size_t
        operator()(const Utilities::Position<T>& position) const noexcept{
        const std::size_t h1 = std::hash<T>{}(position.x);
        const std::size_t h2 = std::hash<T>{}(position.y);
        return h1^(h2<<1);
    }
};



template<typename T, typename S>
requires Addable<T, S>
constexpr Utilities::Position<T>
operator+(const Utilities::Position<T>& lhs, const Utilities::Position<S>& rhs){
    return Utilities::Position<T>{lhs.x+rhs.x, lhs.y+rhs.y};
}

template<typename T, typename S>
requires Subtractable<T, S>
constexpr Utilities::Position<T>
 operator-(const Utilities::Position<T>& lhs, const Utilities::Position<S>& rhs){
    return Utilities::Position<T>{lhs.x-rhs.x, lhs.y-rhs.y};
}

template<typename T>
constexpr Utilities::Position<T>
operator/(const Utilities::Position<T>& numerator, const T& divisor){
    return Utilities::Position<T>{numerator.x/divisor, numerator.y/divisor};
}

template<typename T>
requires Addable<T, T>
constexpr T
abs(const Utilities::Position<T>& position){
    return std::abs(position.x)+std::abs(position.y);
}


#endif