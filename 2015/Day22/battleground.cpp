
#include "battleground.h"
#include "magician.h"
#include "enemy.h"
#include "spell.h"
#include "effect.h"

Battleground::Battleground(Enemy&& _tempEnemy, bool _hardDifficulty)
    : hardDifficulty{_hardDifficulty}, enemy{_tempEnemy}, magician{this}, activeEffects{}{
    enemy.battlegroundPtr = this;
}

void Battleground::reset(){
    magician.reset();
    enemy.reset();
    activeEffects.clear();
}

unsigned int Battleground::battle(unsigned int& minMana){
    auto round = 0u;
    auto manaConsumed = 0u;

    auto magicianWon = [&minMana, &manaConsumed](const auto& _enemy){
        if(_enemy.isDead()){
            if(manaConsumed < minMana){
                minMana = manaConsumed;
            }
            return true;
        }
        return false;
    };
    auto enemyWon = [](const auto& _magician){
        return _magician.isDead();
    };

    for(auto spell : magician.battlePlan){
        if(hardDifficulty){
            magician.hp--;
            if(enemyWon(magician)) break;
        }
        manaConsumed+= magician.makeTurn(spell);
        if(manaConsumed > minMana || enemyWon(magician)) break; //Magician could have lost due depletion of mana
        if(magicianWon(enemy)) break;
        enemy.makeTurn();
        if(magicianWon(enemy)) break; //check if poison killed the enemy before he could have attacked
        if(enemyWon(magician)) break;
        round++;
    }
    return round;
}