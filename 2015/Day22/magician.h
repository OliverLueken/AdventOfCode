#ifndef MAGICIAN_H
#define MAGICIAN_H

#include <vector>
#include <memory>

#include "spell.h"

class Battleground;

class Magician{
public:
    Battleground* battlegroundPtr{nullptr};

    int hp   {50};
    int armor{0};
    int mana {500};

    std::vector<std::unique_ptr<Spell>> spells{};
    std::array<unsigned int, 10> battlePlan{};
    //10 is an arbitrarily choosen number of max rounds, increase if needed

    Magician(Battleground* _battlegroundPtr) : battlegroundPtr{_battlegroundPtr}{
        spells.emplace_back(std::make_unique<MissileSpell> (_battlegroundPtr));
        spells.emplace_back(std::make_unique<DrainSpell>   (_battlegroundPtr));
        spells.emplace_back(std::make_unique<ShieldSpell>  (_battlegroundPtr));
        spells.emplace_back(std::make_unique<PoisonSpell>  (_battlegroundPtr));
        spells.emplace_back(std::make_unique<RechargeSpell>(_battlegroundPtr));
    }

    int makeTurn(unsigned int);
    bool isDead() const;
    void reset();
    bool updateBattlePlan(size_t round);
    bool isValidPlan() const;
};


#endif