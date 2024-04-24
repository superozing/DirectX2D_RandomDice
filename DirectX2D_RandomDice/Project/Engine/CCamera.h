#pragma once
#include "CComponent.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // 직교 투영
    PERSPECTIVE,  // 원근 투영
};

class CCamera :
    public CComponent
{
private:
    PROJ_TYPE               m_ProjType;     // 투영 방식

    // 원근투영(Perspective)
    float                   m_FOV;          // 시야 각(Filed Of View)

    // 직교투영(Orthographic)
    float                   m_Width;        // 직교투영 가로 길이
    float                   m_Scale;        // 직교투영 배율

    // Both
    float                   m_AspectRatio;  // 종횡비, 투영 가로세로 비율
    float                   m_Far;          // 투영 최대 거리
    
    // 변환 행렬
    Matrix                  m_matView;
    Matrix                  m_matProj;

    UINT                    m_LayerCheck;

    int                     m_CameraPriority;


    // 물체 분류
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMasked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;

public:
    PROJ_TYPE GetProjType() const { return m_ProjType; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

    float GetScale() const { return m_Scale; }
    void SetScale(float _Scale) { m_Scale = _Scale; }

    float GetFOV() const { return m_FOV; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() const { return m_Far; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetCameraPriority(int _Priority);
    void LayerCheck(UINT _LayerIdx, bool _bCheck);
    void LayerCheck(const wstring& _strLayerName, bool _bCheck);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }


    int GetPriority() const { return m_CameraPriority; }

    // Layer 체크 확인
    bool IsLayerCheck(UINT _LayerIdx) const;
    bool IsLayerCheck(const wstring& _strLayerName) const;

    //  인자로 들어오는 값은 카메라 기준 Diff 벡터
    Vec2 GetWorldPosInWindow(Vec2 _Diff);

public:
    virtual void begin() override;
    virtual void finaltick() override;


    void SortObject();
    void DepthSort();
    void render();

private:
    void render(vector<CGameObject*>& _vecObj);
    void render_postprocess();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CCamera);
public:
    CCamera();
    ~CCamera();
};

