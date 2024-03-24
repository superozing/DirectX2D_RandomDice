#ifndef _ALPHABLEND
#define _ALPHABLEND

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_OUT VS_AlphaBlend(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}

float4 PS_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 1.f, 1.f, 1.f);
    
    // 1) 애니메이션을 사용할 경우
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        float2 vRightBottom = float2(g_vLeftTop.x + g_vSlizeSize.x, g_vLeftTop.y + g_vSlizeSize.y);
        
        // 범위 안에 없을 경우 discard
        if (vUV.x < g_vLeftTop.x || vRightBottom.x < vUV.x || vUV.y < g_vLeftTop.y || vRightBottom.y < vUV.y)
            discard;
        
        
        vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
    }
    
    // 2) 텍스쳐 
    else if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    // 3) 외의 경우 discard
    else
    {
        discard;
    }
    
    if (vColor.a < 0.1f)
        discard;
        
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
        
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}

#endif
