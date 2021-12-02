#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <numeric>

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

    /*
    A function that splits a string [first, last) by delimiter and
    returns a std::vector<std::string> containing parts
    */
    struct split_{
        auto operator()(auto first, const auto last, const char delimiter = ' ') const{
            std::vector<std::string> parts{};
            std::string part{};
            while(first != last){
                if(*first == delimiter){
                    if(!part.empty()){
                        parts.emplace_back(std::move(part));
                        part = std::string{};
                    }
                }
                else{
                    part.push_back(*first);
                }
                first++;
            }
            if(!part.empty()){
                parts.emplace_back(std::move(part));
            }
            return parts;
        }

        auto operator()(const std::string& r, const char delimiter = ' ') const {
            std::vector<std::string> parts;

            size_t pos1=0, pos2=0;
            while(pos2 != r.npos){
                pos2 = r.find(delimiter, pos1);
                if(pos1 < pos2) parts.emplace_back(r.substr(pos1, pos2-pos1));
                pos1=pos2+1;
            }
            return parts;
        }
    };
    inline constexpr split_ split{};


    /*
    A function that splits a string [first, last) on each character contained in delimiter and
    returns a std::vector<std::string> containing parts
    */
    struct split_on_each{
        template<std::forward_iterator I, std::sentinel_for<I> S>
        auto operator()(I first, const S last, const std::string& delimiter = " ") const{
            std::vector<std::string> parts{};
            std::string part{};
            while(first != last){
                if(contains(delimiter, *first)){
                    if(!part.empty()){
                        parts.emplace_back(std::move(part));
                        part = std::string{};
                    }
                }
                else{
                    part.push_back(*first);
                }
                first++;
            }
            if(!part.empty()){
                parts.emplace_back(std::move(part));
            }
            return parts;
        }

        /*
        A function that splits a string r on each character contained in delimiter and
        returns a std::vector<std::string> containing parts
        */
        auto operator()(const std::string& r, const std::string& delimiter = " ") const {
            return (*this)(std::begin(r), std::end(r), delimiter);
        }
    };
    inline constexpr split_on_each splitOnEach{};



    struct search_all{
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
    inline constexpr search_all searchAll{};


    auto printVec = [](const auto& vec, const std::string delimiter=", "){
        std::ranges::for_each(vec, [&delimiter](const auto& i){std::cout << i << delimiter;});
        std::cout << '\n';
    };

    /*
    left rotation of a range by shift elements
    */
    struct rotate_{
        template<std::forward_iterator I, std::sentinel_for<I> S, std::integral T>
        constexpr auto
        operator()(I first, S last, const T shift) const {
            const auto dis = std::distance(first, last);
            return std::rotate(first, first+(dis+shift)%dis, last );
        }

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
        template<std::forward_iterator I, std::sentinel_for<I> S, std::integral T = unsigned int>
        constexpr auto
        operator()(I first, S last, T init) const {
            return std::accumulate(first, last, init = 0u );
        }

        /*
        Sums up all the elements in the range r and the value init.
        */
        template<std::ranges::forward_range R, std::integral T = unsigned int>
        constexpr auto
        operator()(R&& r, T init = 0u ) const {
            return std::accumulate(std::begin(r), std::end(r), init);
        }

    };
    inline constexpr sum_ sum;
}

#endif