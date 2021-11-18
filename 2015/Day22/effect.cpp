
#include "effect.h"
#include "battleground.h"
#include "magician.h"
#include "enemy.h"

void ShieldEffect::apply() {
    timer--;
    battlegroundPtr->magician.armor=7;
}

void PoisonEffect::apply() {
    timer--;
    battlegroundPtr->enemy.hp-=3;
}

void RechargeEffect::apply() {
    timer--;
    battlegroundPtr->magician.mana+=101;
}


