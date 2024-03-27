#pragma once
#include "UI.h"

// 현재 레벨에 있는 모든 오브젝트들 나열

class TreeUI;
class TreeNode;

class Outliner :
    public UI
{
private:
    TreeUI*     m_Tree;
    bool    m_bRightClick;

public:
    virtual void render_update() override;

public:
    void ResetCurrentLevel();
public:
    void OpenRightClickMenu() { m_bRightClick = true; }
private:
    void DrawRightClickMenu();
    void DeleteObject();

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);

public:
    Outliner();
    ~Outliner();
};

