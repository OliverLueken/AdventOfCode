#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <vector>

#include "magician.h"
#include "enemy.h"
#include "effect.h"

struct Battleground{
    bool hardDifficulty{false};
    Enemy enemy;
    Magician magician{this};
    std::vector<Effect*> activeEffects{};

    Battleground(Enemy&& tempEnemy, bool hardDifficulty);
    unsigned int battle(unsigned int& minMana);
    void reset();
};


#endif