
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/verifySolution.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

enum precedenceOrder{leftToRight, additionBeforeMultiplication};

template<precedenceOrder>
auto innerExpression(std::string&&);

template<>
auto innerExpression<leftToRight>(std::string&& expression) {
    auto a = expression.find_first_of("+*");
    while (a != std::string::npos) {
        const auto b = expression.find_first_of("+*", a + 1);
        const auto leftValue  = stoul(expression);
        const auto rightValue = stoul(expression.substr(a + 1));
        if (expression[a] == '+') {
            expression.replace(0, b, std::to_string(leftValue+rightValue));
        } else {
            expression.replace(0, b, std::to_string(leftValue*rightValue));
        }
        a = expression.find_first_of("+*");
    }
    return expression;
}

template<>
auto innerExpression<additionBeforeMultiplication>(std::string&& expression) {
    auto b = expression.find("+");
    while (b != std::string::npos) {
        auto a = expression.find_last_of("+*", b - 1);
        if (a == std::string::npos)
            a = 0;
        else
            a = a + 2;
        const auto c = expression.find_first_of("+*", b + 1);

        const auto leftValue  = stoul(expression.substr(a));
        const auto rightValue = stoul(expression.substr(b + 1));
        expression.replace(a, c - 1 - a, std::to_string(leftValue+rightValue));

        b = expression.find("+");
    }
    return innerExpression<leftToRight>(std::move(expression));
}

template<precedenceOrder order>
auto evaluateExpression = [regex = std::regex("([(][^()]+[)])")](auto& expression) {
    //regex matches every most inner bracketed expression including the brackets
    //9 * 3 * 4 + (8 + 3) + (7 + 9 + 9 * 8 + 3 + (8 + 5 + 2 + 9))
    //eg          ^^^^^^^<-this           and    ^^^^^^^^^^^^^^^<-this
    std::smatch match;
    while (std::regex_search(expression, match, regex)) {
        const auto eval = innerExpression<order>(match.str().substr(1, match.length() - 2));
        expression.replace(match.position(), match.length(), eval);
    }
    return stoul(innerExpression<order>(std::move(expression)));
};

template<precedenceOrder order>
auto sumSolutions(auto input) {
    return Utilities::sum(input, 0ul, evaluateExpression<order>);
}

int main(){
    const auto input = readFile::vectorOfStrings();

    //Task 1
    const auto sumOfSolutionsWithLeftToRight = sumSolutions<leftToRight>(input);
    std::cout << "The sum of the solutions evaluated left to right is " << sumOfSolutionsWithLeftToRight << ".\n";

    //Task 2
    const auto sumOfSolutionsWithAdditionBeforeMultiplication = sumSolutions<additionBeforeMultiplication>(input);
    std::cout << "The sum of the solutions evaluated with the new rules is " << sumOfSolutionsWithAdditionBeforeMultiplication << ".\n";

    VerifySolution::verifySolution(sumOfSolutionsWithLeftToRight, sumOfSolutionsWithAdditionBeforeMultiplication);
}
