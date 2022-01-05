#ifndef SPELL_H
#define SPELL_H

#include "effect.h"

class Battleground;

class Spell{
public:
    Battleground* battlegroundPtr{nullptr};
    Spell(Battleground* _battlegroundPtr) : battlegroundPtr{_battlegroundPtr}{}
    Spell(const Spell&) = default;
    Spell& operator=(const Spell&) = default;
    Spell(Spell&&) = default;
    Spell& operator=(Spell&&) = default;
    virtual ~Spell() = default;

    virtual int castSpell() = 0;
};

class MissileSpell : public Spell{
    const int manaCost{53};
public:
    MissileSpell(Battleground* _battlegroundPtr) : Spell{_battlegroundPtr}{}
    int castSpell()  override;
};
class DrainSpell : public Spell{
    const int manaCost{73};
public:
    DrainSpell(Battleground* _battlegroundPtr) : Spell{_battlegroundPtr}{}
    int castSpell()  override;
};

class ShieldSpell : public Spell{
    const int manaCost{113};
public:
    ShieldSpell(Battleground* _battlegroundPtr) : Spell{_battlegroundPtr}{}
    ShieldEffect effect{battlegroundPtr, 6};
    int castSpell() override;
};

class PoisonSpell : public Spell{
    const int manaCost{173};
public:
    PoisonSpell(Battleground* _battlegroundPtr) : Spell{_battlegroundPtr}{}
    PoisonEffect effect{battlegroundPtr, 6};
    int castSpell()  override;
};

class RechargeSpell : public Spell{
    const int manaCost{229};
public:
    RechargeSpell(Battleground* _battlegroundPtr) : Spell{_battlegroundPtr}{}
    RechargeEffect effect{battlegroundPtr, 5};
    int castSpell()  override;
};


#endif