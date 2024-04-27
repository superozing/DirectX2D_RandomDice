#pragma once
#include "CBossScript.h"

class CSnakeBoss :
    public CBossScript
{
private:
    float   m_SkillTimer;
    float   m_SkillDelayTimer;
    bool    m_IsActivateSkill;



public:
    virtual void BossSkill() override;
    virtual void EndBossSkill() override;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    FSAVELOAD_DISABLE;
    CLONE(CSnakeBoss);
    CSnakeBoss();
    CSnakeBoss(const CSnakeBoss& _Origin);
    ~CSnakeBoss();
};

