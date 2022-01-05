#ifndef EFFECT_H
#define EFFECT_H

class Battleground;

class Effect{
public:
    Battleground* battlegroundPtr{nullptr};
    int timer{0};

    Effect(Battleground* _battlegroundPtr, int _timer) : battlegroundPtr{_battlegroundPtr}, timer{_timer}{}
    Effect(const Effect&) = default;
    Effect& operator=(const Effect&) = default;
    Effect(Effect&&) = default;
    Effect& operator=(Effect&&) = default;
    virtual ~Effect() = default;

    virtual void apply() = 0;
};

class ShieldEffect : public Effect{
public:
    ShieldEffect(Battleground* _battlegroundPtr, int _timer)
        : Effect{_battlegroundPtr, _timer}{}
    void apply() override;
};

class PoisonEffect : public Effect{
public:
    PoisonEffect(Battleground* _battlegroundPtr, int _timer)
        : Effect{_battlegroundPtr, _timer}{}
    void apply() override;
};

class RechargeEffect : public Effect{
public:
    RechargeEffect(Battleground* _battlegroundPtr, int _timer) :
        Effect{_battlegroundPtr, _timer}{}
    void apply() override;
};


#endif