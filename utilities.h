#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

namespace Utilities{
    auto split = [](std::string s, char delim = ' '){
        std::vector<std::string> parts;

        size_t pos1=0, pos2=0;
        while(pos2 != s.npos){
            pos2 = s.find(delim, pos1);
            if(pos1 < pos2) parts.push_back(s.substr(pos1, pos2-pos1));
            pos1=pos2+1;
        }
        return parts;
    };

    //A function that searches in the range {I1, S1} for the range {I2,S2} and
    //returns a vector of subranges of every occurance it found
    //while using the usual comparator pred
    //and projectinos proj1 and proj2 that is common in the algorithms library
    struct search_all{
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


    auto printVec = [](const auto& vec, const auto delimiter=", "){
        std::ranges::for_each(vec, [&delimiter](const auto& i){std::cout << i << delimiter;});
        std::cout << '\n';
    };
}

#endif