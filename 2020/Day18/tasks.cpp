
#include "../../lib/matrix.hpp"
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <cmath>
// #include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

enum precedenceOrder{leftToRight, additionBeforeMultiplication};

template<precedenceOrder>
auto innerExpression(std::string&&);

template<>
auto innerExpression<leftToRight>(std::string&& ex) {
    // size_t a, b;

    auto a = ex.find_first_of("+*");
    while (a != std::string::npos) {
        const auto b = ex.find_first_of("+*", a + 1);
        // long x, y;
        auto x = stoul(ex);
        auto y = stoul(ex.substr(a + 1));
        if (ex[a] == '+') {
            x = x + y;
        } else {
            x = x * y;
        }
        ex.replace(0, b, std::to_string(x));
        a = ex.find_first_of("+*");
    }
    return ex;
}

template<>
auto innerExpression<additionBeforeMultiplication>(std::string&& ex) {
    auto b = ex.find("+");
    while (b != std::string::npos) {
        auto a = ex.find_last_of("+*", b - 1);
        if (a == std::string::npos)
            a = 0;
        else
            a = a + 2;
        const auto c = ex.find_first_of("+*", b + 1);

        const auto y = stoul(ex.substr(b + 1));
        const auto x = stoul(ex.substr(a)) + y;
        ex.replace(a, c - 1 - a, std::to_string(x));

        b = ex.find("+");
    }
    return innerExpression<leftToRight>(std::move(ex));
}

template<precedenceOrder order>
auto outerExpression = [regex = std::regex("([(][^()]+[)])")](auto& ex) {
    //regex matches every most inner bracketed expression including the brackets
    //9 * 3 * 4 + (8 + 3) + (7 + 9 + 9 * 8 + 3 + (8 + 5 + 2 + 9))
    //eg          ^^^^^^^<-this           and    ^^^^^^^^^^^^^^^<-this
    std::smatch match;
    while (std::regex_search(ex, match, regex)) {
        const auto eval = innerExpression<order>(match.str().substr(1, match.length() - 2));
        ex.replace(match.position(), match.length(), eval);
    }
    return stoul(innerExpression<order>(std::move(ex)));
};

template<precedenceOrder order>
auto evalAll(auto input) {
    return Utilities::sum(input, 0ul, outerExpression<order>);
}

int main(){
    const auto input = readFile::vectorOfStrings();

    //Task 1
    const auto result1 = evalAll<leftToRight>(input);
    std::cout << " " << result1 << " .\n";

    //Task 2
    const auto result2 = evalAll<additionBeforeMultiplication>(input);
    std::cout << " " << result2 << " .\n";

    VerifySolution::verifySolution(result1, result2);
}
