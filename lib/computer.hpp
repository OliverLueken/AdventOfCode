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

    template<class Data>
    class Computer : public Subject {

        struct Instruction{
            Instruction() = default;
            Instruction(const Instruction&) = default;
            Instruction& operator=(const Instruction&) = default;
            Instruction(Instruction&&) = default;
            Instruction& operator=(Instruction&&) = default;
            virtual ~Instruction() = default;

            virtual void execute() = 0;
        };


        template<typename Lambda>
        struct Wrapper : public Instruction, public Lambda{
            Computer* computerPtr{nullptr};
            Wrapper(Lambda&& lambda, Computer* _computerPtr): Lambda{std::forward<Lambda>(lambda)}, computerPtr{_computerPtr} {}
            void execute() override { Lambda::operator()(computerPtr); }
        };

        using Instructions = std::vector<std::unique_ptr<Instruction>>;

        int m_currentInstructionPosition{0};
        Instructions m_instructions{};
        std::unique_ptr<Data> m_data{};

    public:

        int getCurrentPosition() const {
            return m_currentInstructionPosition;
        }

        void advanceCurrentPosition(const int offset = 1){
            m_currentInstructionPosition+=offset;
        }

        Data& getData(){
            return *m_data.get();
        }

        const Data& getData() const {
            return *m_data.get();
        }

        template<typename Lambda>
        auto add(Lambda&& l){
            m_instructions.emplace_back(std::make_unique<Wrapper<Lambda>>(std::forward<Lambda>(l), this));
        }


        bool currentInstructionPositionIsValid() const {
            return 0<=m_currentInstructionPosition && std::less{}(m_currentInstructionPosition, m_instructions.size());
        }

        Computer()
        : m_currentInstructionPosition{0}, m_instructions{}, m_data{std::make_unique<Data>()} {}

        template<typename... Args>
        Computer(Args&&... args)
        : m_currentInstructionPosition{0}, m_instructions{}, m_data{std::make_unique<Data>(std::forward<Args>(args)...)} {}

        Computer(Computer&&) = default;
        Computer& operator=(Computer&&) = default;
        virtual ~Computer() = default;

        auto execute(){
            while( currentInstructionPositionIsValid() ){
                m_instructions[m_currentInstructionPosition]->execute();
                notifyLogger();
            }
        }

        auto getDataView() const {
            return std::views::all(*m_data);
        }

        void reset(){
            resetCurrentPos();
            resetInstructions();
            resetData();
        }

        virtual void resetCurrentPos(){
            m_currentInstructionPosition = 0;
        }
        virtual void resetInstructions(){}
        virtual void resetData(){
            auto newData = std::make_unique<Data>();
            std::swap(m_data, newData);
        }
    };


    template<typename Register>
    class ComputerFactory{
        Computer<Register>* computerPtr{nullptr};

    protected:
        template<typename Command>
        void addCommand(Command&& command){
            return computerPtr->add(
                std::forward<Command>(command)
            );
        }

    public:
        auto make(const auto& input){
            auto computer = Computer<Register>{};
            computerPtr = &computer;

            auto parse = [this](const auto& string){
                makeCommand(string);
            };

            std::ranges::for_each(input, parse);
            return computer;
        }

        virtual void makeCommand(const std::string&) = 0;
    };


} //end namespace Computer

#endif