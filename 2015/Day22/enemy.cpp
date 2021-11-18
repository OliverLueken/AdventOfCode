
#include "enemy.h"
#include "magician.h"
#include "effect.h"
#include "battleground.h"

void Enemy::makeTurn(){
    battlegroundPtr->magician.armor=0;
    for(auto effectPtr : battlegroundPtr->activeEffects){
        effectPtr->apply();
    }
    std::erase_if(battlegroundPtr->activeEffects, [](auto effectPtr){return effectPtr->timer==0;});
    battlegroundPtr->magician.hp-=std::max(1, damage-battlegroundPtr->magician.armor);
}

bool Enemy::isDead() const{
    return hp<=0;
}

void Enemy::reset(){
    hp = maxHP;
}