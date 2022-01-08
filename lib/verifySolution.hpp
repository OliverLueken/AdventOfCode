#ifndef VERIFYSOLUTION_HPP
#define VERIFYSOLUTION_HPP

#include "readFile.hpp"
#include "utilities.hpp"

#include <string>
#include <iostream>
#include <iomanip>

namespace VerifySolution{
    struct verifySolution_{

        template<typename T>
        auto operator()(const T& answer) const {
            const auto solutions = readFile::vectorOfStrings("solutions.txt");
            verify(answer, solutions[0]);
        }

        template<typename T1, typename T2>
        auto operator()(const T1& answer1, const T2& answer2) const {
            const auto solutions = readFile::vectorOfStrings("solutions.txt");
            verify(answer1, solutions[0]);
            verify(answer2, solutions[1]);
        }

    private:
        template<Utilities::isNumber T>
        auto verify(const T& answer, const std::string& solution) const {
            verify(std::to_string(answer), solution);
        }

        auto verify(const std::string& answer, const std::string& solution) const {
            const auto upperAnswer = Utilities::toupper(answer);
            const auto upperSolution = Utilities::toupper(solution);
            std::cout << "Submitted answer: "  << std::quoted(upperAnswer)
                      << ", expected answer: " << std::quoted(upperSolution) << ": ";
            std::cout << (upperAnswer == upperSolution ? "PASSED\n" : "FAILED\n");
        }
    };
    inline verifySolution_ verifySolution;
}

#endif