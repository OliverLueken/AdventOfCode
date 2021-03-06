#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "concepts.hpp"

#include <ranges>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <set>

namespace Utilities{

    struct contains_{
        /*
        An algorithm that searches for the element value in the range [first, last)
        returns true if it finds it, false if not
        */
        template<std::forward_iterator I, std::sentinel_for<I> S,
                 class T,
                 class Proj = std::identity>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<I, Proj>, const T*>
        constexpr auto
        operator()(I first, S last, const T& value, Proj proj = {}) const {
            return std::ranges::find(first, last, value, proj) != last;
        }


        /*
        An algorithm that searches for the element value in the range r
        returns true if it finds it, false if not
        */
        template<std::ranges::forward_range R, class T, class Proj = std::identity>
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<std::ranges::iterator_t<R>, Proj>, const T*>
        constexpr auto
        operator()(R&& r, const T& value, Proj proj = {}) const {
            return std::ranges::find(r, value, proj) != std::end(r);
        }

        /*
        An algorithm that searches for the range [first2, last2) in the range [first1, last1)
        returns true if it finds it, false if not
        */
        template<std::forward_iterator I1, std::sentinel_for<I1> S1,
                 std::forward_iterator I2, std::sentinel_for<I2> S2,
                 class Pred = std::ranges::equal_to,
                 class Proj1 = std::identity,
                 class Proj2 = std::identity>
        requires std::indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
        constexpr auto
        operator()(I1 first1, S1 last1, I2 first2, S2 last2,
                   Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
            return std::ranges::search(first1, last1, first2, last2, pred, proj1, proj2).begin() != last1;
        }


        /*
        An algorithm that searches for the range r2 in the range r1
        returns true if it finds it, false if not
        */
        template<std::ranges::forward_range R1, std::ranges::forward_range R2,
             class Pred = std::ranges::equal_to,
             class Proj1 = std::identity,
             class Proj2 = std::identity>
        requires std::indirectly_comparable<std::ranges::iterator_t<R1>, std::ranges::iterator_t<R2>,
                                            Pred, Proj1, Proj2>
        constexpr auto
        operator()(R1&& r1, R2&& r2, Pred pred = {},
                   Proj1 proj1 = {}, Proj2 proj2 = {}) const {
            return std::ranges::search(r1, r2, pred, proj1, proj2).begin() != std::end(r1);
        }
    };
    inline constexpr contains_ contains;


    struct split_{
        /*
        A function that splits a string [first, last) by delimiter and
        returns a std::vector<std::string> containing the parts
        */
        auto operator()(auto first, const auto last, const char delimiter = ' ', const bool keepEmpty = false) const {
            std::vector<std::string> parts{};
            while(first < last){
                const auto second = std::ranges::find(first, last, delimiter);
                parts.emplace_back(first, second);
                first = second+1;
            }
            if(!keepEmpty){
                std::erase(parts, "");
            }
            return parts;
        }

        /*
        A function that splits a string r by delimiter and
        returns a std::vector<std::string> containing the parts
        */
        auto operator()(const std::string& r, const char delimiter = ' ', const bool keepEmpty = false) const {
            return (*this)(std::begin(r), std::end(r), delimiter, keepEmpty);
        }
    };
    inline constexpr split_ split{};



    struct splitOnEach_{
        /*
        A function that splits a string [first, last) on each character contained in delimiter and
        returns a std::vector<std::string> containing parts
        */
        template<std::forward_iterator I, std::sentinel_for<I> S>
        auto operator()(I first, const S last, const std::string& delimiter = " ", const bool keepEmpty = false) const {
            std::vector<std::string> parts{};
            while(first < last){
                const auto second = std::ranges::find_first_of(first, last, std::begin(delimiter), std::end(delimiter));
                parts.emplace_back(first, second);
                first = second+1;
            }
            if(!keepEmpty){
                std::erase(parts, "");
            }
            return parts;
        }

        /*
        A function that splits a string r on each character contained in delimiter and
        returns a std::vector<std::string> containing parts
        */
        auto operator()(const std::string& r, const std::string& delimiter = " ", const bool keepEmpty = false) const {
            return (*this)(std::begin(r), std::end(r), delimiter, keepEmpty);
        }
    };
    inline constexpr splitOnEach_ splitOnEach{};



    struct searchAll_{
        /*
        A function that searches in the range [I1, S1) for the range [I2,S2) and
        returns a vector of subranges of every occurance it found
        while using the usual comparator pred
        and projectinos proj1 and proj2 that is common in the algorithms library
        */
        template<std::forward_iterator I1, std::sentinel_for<I1> S1,
                 std::forward_iterator I2, std::sentinel_for<I2> S2,
                 class Pred = std::ranges::equal_to,
                 class Proj1 = std::identity,
                 class Proj2 = std::identity>
        requires std::indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
        constexpr auto
        operator()(I1 first1, S1 last1, I2 first2, S2 last2,
                   Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
            std::vector<std::ranges::subrange<I1>> vectorOfFoundSubranges{};
            auto start = first1;
            while(true){
                auto subrange = std::ranges::search(start, last1, first2, last2, pred, proj1, proj2);
                start = subrange.begin()+1;
                if(start > last1) break;
                vectorOfFoundSubranges.emplace_back(std::move(subrange));
            }
            return vectorOfFoundSubranges;
        }

        /*
        A function that searches in the range r1 for the range r2 and
        returns a vector of subranges of every occurance it found
        while using the usual comparator pred
        and projectinos proj1 and proj2 that is common in the algorithms library
        */
        template<std::ranges::forward_range R1, std::ranges::forward_range R2,
             class Pred = std::ranges::equal_to,
             class Proj1 = std::identity,
             class Proj2 = std::identity>
        requires std::indirectly_comparable<std::ranges::iterator_t<R1>, std::ranges::iterator_t<R2>,
                                            Pred, Proj1, Proj2>
        constexpr auto
        operator()(R1&& r1, R2&& r2, Pred pred = {},
                   Proj1 proj1 = {}, Proj2 proj2 = {}) const {
          return (*this)(std::ranges::begin(r1), std::ranges::end(r1),
                         std::ranges::begin(r2), std::ranges::end(r2),
                         std::move(pred), std::move(proj1), std::move(proj2));
        }
    };
    inline constexpr searchAll_ searchAll{};



    /*
    An std::ranges::rotation interface that takes the number of left shifts to be performed
    instead of an iterator pointing at the first element of the rotated range of a range by shift elements
    */
    struct rotate_{
        /*
        A rotate function that takes a range [first, last) and performs a left rotation by shift elements
        */
        template<std::forward_iterator I, std::sentinel_for<I> S, std::integral T>
        constexpr auto
        operator()(I first, S last, const T shift) const {
            const auto dis = std::distance(first, last);
            return std::ranges::rotate(first, first+(dis+shift)%dis, last );
        }

        /*
        A rotate function that takes a range r and performs a left rotation by shift elements
        */
        template<std::ranges::forward_range R, std::integral T>
        constexpr auto
        operator()(R&& r, const T shift) const {
            return std::ranges::rotate(r, std::begin(r)+(r.size()+shift)%r.size());
        }

    };
    inline constexpr rotate_ rotate;

    struct sum_{
        /*
        Sums up all the elements in the range [first, last) and the value init.
        */
        template<std::forward_iterator I, std::sentinel_for<I> S, number T = unsigned int, class Proj = std::identity >
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<I, Proj>, const T*>
        constexpr auto
        operator()(I first, S last, T init = 0u, Proj proj = {}) const {
            return std::transform_reduce(first, last, init, std::plus<>(), proj);
        }

        /*
        Sums up all the elements in the range r and the value init.
        */
        template<std::ranges::forward_range R, number T = unsigned int, class Proj = std::identity >
        requires std::indirect_binary_predicate<std::ranges::equal_to, std::projected<std::ranges::iterator_t<R>, Proj>, const T*>
        constexpr auto
        operator()(R&& r, T init = 0u, Proj proj = {} ) const {
            return std::transform_reduce(std::begin(r), std::end(r), init, std::plus<>(), proj);
        }

    };
    inline constexpr sum_ sum;



    struct setDifference_{
        /*
        Returns a set of the elements from the sorted input range [first1, last1) which are not found in the sorted input range [first2, last2)
        */
        template<
            std::forward_iterator I1, std::sentinel_for<I1> S1,
            std::forward_iterator I2, std::sentinel_for<I2> S2,
            class Comp = std::ranges::less,
            class Proj1 = std::identity,
            class Proj2 = std::identity
        >
        constexpr auto
        operator()(I1 first1, S1 last1, I2 first2, S2 last2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
            using T = std::iter_value_t<I1>;
            auto difference = std::set<T>{};
            std::ranges::set_difference(first1, last1, first2, last2, std::inserter(difference, std::begin(difference)), comp, proj1, proj2);
            return difference;
        }

        /*
        Returns a set of the elements from the sorted input range r1 which are not found in the sorted input range r2
        */
        template<
            std::ranges::forward_range R1,
            std::ranges::forward_range R2,
            class Comp = std::ranges::less,
            class Proj1 = std::identity,
            class Proj2 = std::identity
        >
        constexpr auto
        operator()(R1&& r1, R2&& r2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
            return (*this)(
                std::ranges::begin(r1), std::ranges::end(r1),
                std::ranges::begin(r2), std::ranges::end(r2),
                std::move(comp), std::move(proj1), std::move(proj2)
            );
        }

    };
    inline constexpr setDifference_ setDifference;
};

#endif