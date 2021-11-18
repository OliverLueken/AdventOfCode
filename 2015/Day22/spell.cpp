
struct Enemy;
struct Magician;
//#include <vector>

#include "spell.h"
#include "battleground.h"
#include "enemy.h"
#include "magician.h"
#include "effect.h"


int MissileSpell::castSpell()  {
    battlegroundPtr->enemy.hp-=4;
    battlegroundPtr->magician.mana-=manaCost;
    return manaCost;
}

int DrainSpell::castSpell()  {
    battlegroundPtr->enemy.hp-=2;
    battlegroundPtr->magician.mana-=manaCost;
    battlegroundPtr->magician.hp+=2;
    return manaCost;
}

int ShieldSpell::castSpell() {
    battlegroundPtr->magician.mana-=manaCost;
    battlegroundPtr->activeEffects.push_back(&effect);
    effect.timer=6;
    return manaCost;
}

int PoisonSpell::castSpell()  {
    battlegroundPtr->magician.mana-=manaCost;
    effect.timer=6;
    battlegroundPtr->activeEffects.push_back(&effect);
    return manaCost;
}

int RechargeSpell::castSpell()  {
    battlegroundPtr->magician.mana-=manaCost;
    effect.timer=5;
    battlegroundPtr->activeEffects.push_back(&effect);
    return manaCost;
}
