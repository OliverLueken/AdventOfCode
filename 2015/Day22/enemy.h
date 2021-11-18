#ifndef ENEMY_H
#define ENEMY_H

struct Battleground;

struct Enemy{
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