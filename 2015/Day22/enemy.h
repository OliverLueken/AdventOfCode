#ifndef ENEMY_H
#define ENEMY_H

class Battleground;

class Enemy{
public:
    Battleground* battlegroundPtr{nullptr};

    const int maxHP{0};
    int hp{0};
    const int damage{0};

    Enemy(int hp, int damage) : maxHP{hp}, hp{hp}, damage{damage}{}
    void makeTurn();
    bool isDead() const;
    void reset();
};

#endif