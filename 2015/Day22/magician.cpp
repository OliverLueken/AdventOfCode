
#include <algorithm>

#include "magician.h"
#include "battleground.h"
#include "effect.h"

int Magician::makeTurn(unsigned int spellNumber){
    battlegroundPtr->magician.armor=0;
    for(auto effectPtr : battlegroundPtr->activeEffects){
        effectPtr->apply();
    }
    std::erase_if(battlegroundPtr->activeEffects, [](auto effectPtr){return effectPtr->timer==0;});
    return spells[spellNumber]->castSpell();
}

bool Magician::isDead() const{
    return hp<=0 || mana < 0;
}

void Magician::reset(){
    hp = 50;
    armor = 0;
    mana = 500;
}

//this checks if the spells with an effect are not casted too early again,
//since the recharge is 5 or 6 they have to be at least 3 rounds apart
bool Magician::isValidPlan()const{
    for(unsigned int i : {2u, 3u, 4u}){
        auto left = std::ranges::find(battlePlan, i);
        if(left == std::end(battlePlan)) continue;
        auto right = std::ranges::find(left+1, std::end(battlePlan), i);
        while(right != std::end(battlePlan)){
            if(std::distance(left, right) < 3) return false;
            left = right;
            right = std::ranges::find(left+1, std::end(battlePlan), i);
        }
    }
    return true;
}

/*
this sets up all the spells the magician will cast in the battle beforehand
0 = MissileSpell
1 = DrainSpell
2 = ShieldSpell
3 = PoisonSpell
4 = RechargeSpell
*/
bool Magician::updateBattlePlan(size_t round){
    do{
        auto toChangeIt = std::ranges::find_if(std::rbegin(battlePlan)+battlePlan.size()-round-1, std::rend(battlePlan),
            [](const auto i){return i<4;});
        if(toChangeIt == std::rend(battlePlan)) return false;
        (*toChangeIt)++;
        std::ranges::fill(std::rbegin(battlePlan), toChangeIt, 0u);
    }while(!isValidPlan());
    return true;
}