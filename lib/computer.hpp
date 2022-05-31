#ifndef COMPUTER_HPP
#define COMPUTER_HPP


#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ranges>
#include <functional>
#include <algorithm>


namespace Computer{
    struct Logger{
        Logger() = default;
        Logger(const Logger&) = default;
        Logger& operator=(const Logger&) = default;
        Logger(Logger&&) = default;
        Logger& operator=(Logger&&) = default;
        virtual ~Logger() = default;

        virtual void update() = 0;
    };

    struct Subject{
        Subject() = default;
        Subject(const Subject&) = default;
        Subject& operator=(const Subject&) = default;
        Subject(Subject&&) = default;
        Subject& operator=(Subject&&) = default;
        virtual ~Subject() = default;

        std::vector<Logger*> logger{};
        void addLogger(Logger* log){
            logger.push_back(log);
        }
        void removeLogger(const Logger* log){
            std::erase(logger, log);
        }
        void notifyLogger() const {
            std::ranges::for_each(logger, [](Logger* log){log->update();});
        }
    };


    struct Instruction{
        Instruction() = default;
        Instruction(const Instruction&) = default;
        Instruction& operator=(const Instruction&) = default;
        Instruction(Instruction&&) = default;
        Instruction& operator=(Instruction&&) = default;
        virtual ~Instruction() = default;

        virtual void execute() = 0;
    };

    template<class Register>
    class Computer : public Subject {
    public:

        template<typename Lambda>
        struct Wrapper : public Instruction, public Lambda{
            Wrapper(Lambda&& lambda): Lambda{std::forward<Lambda>(lambda)} {}
            void execute() override { Lambda::operator()(); }
        };

        using Instructions = std::vector<std::unique_ptr<Instruction>>;

        int currentInstructionPosition{0};
        Instructions instructions{};
        std::unique_ptr<Register> reg{};

        template<typename Lambda>
        auto add(Lambda&& l){
            instructions.emplace_back(std::make_unique<Wrapper<Lambda>>(std::forward<Lambda>(l)));
        }


        bool currentInstructionPositionIsValid() const {
            return 0<=currentInstructionPosition && std::less{}(currentInstructionPosition, instructions.size());
        }

        Computer()
        : currentInstructionPosition{0}, instructions{}, reg{std::make_unique<Register>()} {}
        Computer(const Computer&) = default;
        Computer& operator=(const Computer&) = default;
        Computer(Computer&&) = default;
        Computer& operator=(Computer&&) = default;
        virtual ~Computer() = default;

        auto execute(){
            for(currentInstructionPosition = 0; currentInstructionPositionIsValid();){
                instructions[currentInstructionPosition]->execute();
                notifyLogger();
            }
        }

        auto getRegisterView() const {
            return std::views::all(*reg);
        }

        /*
        reset()

        virtual resetCurrentPos
        virtual resetInstructions
        virtual resetData
        */
    };

    //ComputerFactory

} //end namespace Computer

#endif