
#include "../../lib/readFile.hpp"
#include "../../lib/verifySolution.hpp"
#include "../../lib/utilities.hpp"
#include "../../lib/matrix.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <memory>
#include <ranges>


struct Expression{

    Expression() = default;
    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;
    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;
    virtual ~Expression() = default;

    virtual unsigned long long evaluate() = 0;
    virtual void print(int) = 0;
};

struct Value : public Expression{
    unsigned long long val{0};

    Value(auto val_) : val{val_}{}

    unsigned long long evaluate(){
        return val;
    }
    void print(int n){
        for(auto i=0; i<n; ++i){
            std::cerr << '|';
        }
        std::cerr << '-' << val << '\n';
    }
};

struct Addition : public Expression {
    std::unique_ptr<Expression> left{};
    std::unique_ptr<Expression> right{};

    Addition(auto&& left_, auto&& right_) : left{std::move(left_)}, right{std::move(right_)} {}

    unsigned long long evaluate(){
        return left->evaluate() + right->evaluate();
    }
    void print(int n){
        for(auto i=0; i<n; ++i){
            std::cerr << '|';
        }
        std::cerr << '+' << '\n';
        left->print(n+1);
        right->print(n+1);
    }
};

struct Multiplication : public Expression {
    std::unique_ptr<Expression> left{};
    std::unique_ptr<Expression> right{};

    Multiplication(auto&& left_, auto&& right_) : left{std::move(left_)}, right{std::move(right_)} {}

    unsigned long long evaluate(){
        return left->evaluate() * right->evaluate();
    }
    void print(int n){
        for(auto i=0; i<n; ++i){
            std::cerr << '|';
        }
        std::cerr << '*' << '\n';
        left->print(n+1);
        right->print(n+1);
    }
};

auto findClosing = [](auto begin, auto end){
    auto bracketCount = 0l;
    auto it = begin;
    while(it!=end){
        bracketCount-=std::ranges::count(*it, '(');
        bracketCount+=std::ranges::count(*it, ')');
        if(bracketCount <= 0){
            return it;
        }
        it++;
    }
    return it;
};

auto buildExpressionTree(auto begin, auto end) -> std::unique_ptr<Expression> {
    if( std::distance(begin, end) == 1){
        return std::make_unique<Value>( std::stoul(*begin)  );
    }
    auto n = 1l;
    do{
        n = 1l;
        if( begin->back() == ')'){
            auto closingBracketAt = findClosing(begin, end);
            n = std::distance(begin, closingBracketAt)+1;
            // std::cout << "Bracket until " << n << '\n';
            begin->pop_back();
            closingBracketAt->erase(0,1);
        }
    }while( end == begin+n );
    auto left  = buildExpressionTree( begin,     begin+n ) ;
    auto right = buildExpressionTree( begin+n+1, end    ) ;
    if( (*(begin+n))[0]=='+'){
        return std::make_unique<Addition>( std::move(left), std::move(right) );
    }
    return std::make_unique<Multiplication>( std::move(left), std::move(right) );
}

auto parseInput(const auto& input){

    auto buildExpression = [](const auto& str){
        // std::cout << str << '\n';
        // char c; std::cin >> c;
        auto v_str = Utilities::split(str, ' ');
        return buildExpressionTree(std::rbegin(v_str), std::rend(v_str));
    };

    std::vector<std::unique_ptr<Expression>> expressions{};
    expressions.reserve(input.size());
    std::ranges::transform(input, std::back_inserter(expressions), buildExpression);
    return expressions;
}

int main(){
    const auto a = parseInput(readFile::vectorOfStrings());

    //Task 1
    const auto result1 = Utilities::sum(a, 0ull, [](const auto& ptr){
        return ptr->evaluate();
    });
    std::cout << " " << result1 << " .\n";
    
    // //Task 2
    // const auto result2 = evalAll<2>(input);
    // std::cout << " " << result2 << " .\n";
    //
    // VerifySolution::verifySolution(result1, result2);
}
