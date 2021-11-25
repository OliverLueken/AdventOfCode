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
    std::vector<unsigned int> battlePlan{std::vector<unsigned int>(10,0)};
                                                //10 is an arbitrarily choosen number of max rounds, increase if needed

    Magician(Battleground* battlegroundPtr) : battlegroundPtr{battlegroundPtr}{
        spells.emplace_back(std::make_unique<MissileSpell> (battlegroundPtr));
        spells.emplace_back(std::make_unique<DrainSpell>   (battlegroundPtr));
        spells.emplace_back(std::make_unique<ShieldSpell>  (battlegroundPtr));
        spells.emplace_back(std::make_unique<PoisonSpell>  (battlegroundPtr));
        spells.emplace_back(std::make_unique<RechargeSpell>(battlegroundPtr));
    }

    int makeTurn(unsigned int);
    bool isDead() const;
    void reset();
    bool updateBattlePlan(size_t round);
    bool isValidPlan() const;
};


#endif