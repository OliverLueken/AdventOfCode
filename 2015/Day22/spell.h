#ifndef SPELL_H
#define SPELL_H

#include "effect.h"

class Battleground;

class Spell{
public:
    Battleground* battlegroundPtr{nullptr};
    Spell(Battleground* battlegroundPtr) : battlegroundPtr{battlegroundPtr}{}

    virtual int castSpell() = 0;
};

class MissileSpell : public Spell{
    const int manaCost{53};
public:
    MissileSpell(Battleground* battlegroundPtr) : Spell{battlegroundPtr}{}
    int castSpell()  override;
};
class DrainSpell : public Spell{
    const int manaCost{73};
public:
    DrainSpell(Battleground* battlegroundPtr) : Spell{battlegroundPtr}{}
    int castSpell()  override;
};

class ShieldSpell : public Spell{
    const int manaCost{113};
public:
    ShieldEffect effect{battlegroundPtr, 6};
    ShieldSpell(Battleground* battlegroundPtr) : Spell{battlegroundPtr}{}
    int castSpell() override;
};

class PoisonSpell : public Spell{
    const int manaCost{173};
public:
    PoisonEffect effect{battlegroundPtr, 6};
    PoisonSpell(Battleground* battlegroundPtr) : Spell{battlegroundPtr}{}
    int castSpell()  override;
};

class RechargeSpell : public Spell{
    const int manaCost{229};
public:
    RechargeEffect effect{battlegroundPtr, 5};
    RechargeSpell(Battleground* battlegroundPtr) : Spell{battlegroundPtr}{}
    int castSpell()  override;
};


#endif