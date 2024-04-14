#pragma once
#include "CBossScript.h"
class CRandomKnightBoss :
    public CBossScript
{
private:
    float   m_SkillTimer;
    float   m_SkillDelayTimer;
    bool    m_IsActivateSkill;



public:
    virtual void BossSkill() override;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    FSAVELOAD_DISABLE;
    CLONE(CRandomKnightBoss);
    CRandomKnightBoss();
    CRandomKnightBoss(const CRandomKnightBoss& _Origin);
    ~CRandomKnightBoss();
};

