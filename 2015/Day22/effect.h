#ifndef EFFECT_H
#define EFFECT_H

class Battleground;

class Effect{
public:
    Battleground* battlegroundPtr{nullptr};
    int timer{0};

    Effect(Battleground* battlegroundPtr, int timer) : battlegroundPtr{battlegroundPtr}, timer{timer}{}

    virtual void apply() = 0;
};

class ShieldEffect : public Effect{
public:
    ShieldEffect(Battleground* battlegroundPtr, int timer)
        : Effect{battlegroundPtr, timer}{}
    void apply() override;
};

class PoisonEffect : public Effect{
public:
    PoisonEffect(Battleground* battlegroundPtr, int timer)
        : Effect{battlegroundPtr, timer}{}
    void apply() override;
};

class RechargeEffect : public Effect{
public:
    RechargeEffect(Battleground* battlegroundPtr, int timer) :
        Effect{battlegroundPtr, timer}{}
    void apply() override;
};


#endif