#pragma once
#include "CDiceMergeScript.h"
class CDefaultMerge :
    public CDiceMergeScript
{
public:
    void DefaultMergeDiceDelegateFunc(CDiceScript* _FocusDice);
    bool DefaultIsMergeAbleDelegateFunc(CDiceScript* _FocusDice);

public:
    CLONE(CDefaultMerge);
    CDefaultMerge();
    CDefaultMerge(const CDefaultMerge& _Origin);
    ~CDefaultMerge();
};

