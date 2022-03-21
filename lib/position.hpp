#ifndef POSITION_HPP
#define POSITION_HPP


namespace Utilities{

    template<class T = size_t>
    using Position = std::pair<T, T>;

}

template<class T>
struct std::hash<Utilities::Position<T>>{
    size_t operator()(const Utilities::Position<T>& position) const noexcept{
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