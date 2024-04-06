#ifndef _ALPHABLEND
#define _ALPHABLEND

#include "value.fx"
#include "func.fx"

#define USE_DEFINE_COLOR g_int_0
#define USE_MUL_COLOR g_int_1

#define ConstBrightness g_float_0

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
    
    // 1) �ִϸ��̼��� ����� ���
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSlizeSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        float2 vRightBottom = float2(g_vLeftTop.x + g_vSlizeSize.x, g_vLeftTop.y + g_vSlizeSize.y);
        
        // ���� �ȿ� ���� ��� discard
        if (vUV.x < g_vLeftTop.x || vRightBottom.x < vUV.x || vUV.y < g_vLeftTop.y || vRightBottom.y < vUV.y)
            discard;
        
        
        vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
    }
    
    // 2) �ؽ��� 
    else if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }
    // 3) ���� ��� discard
    else
    {
        discard;
    }
    
    // ���ø� �� ������ ���İ� �ſ� ���� ��� discard
    if (vColor.a < 0.1f)
        discard;
    
    // ���� USE_DEFINE_COLOR(g_int_0)�� ����Ѵٸ� �Է¹��� �������� ġȯ
    if (USE_DEFINE_COLOR == 1) 
    {
        vColor.r = g_vec4_0.r / 255.f;
        vColor.g = g_vec4_0.g / 255.f;
        vColor.b = g_vec4_0.b / 255.f;
    }
    // ���� USE_MUL_COLOR(g_int_1)�� ����Ѵٸ� �Է¹��� ������ ��
    else if (USE_MUL_COLOR == 1)
    {
        vColor.rgb *= g_vec4_0.rgb / 255.f;
    }
    
    //==========
    // ���� ó��
    //==========
    tLightColor LightColor = (tLightColor) 0.f;
    
    // ��� ������ ���� ó��
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
        
    // ���� ConstBrightness(float 0��)�� ����Ѵٸ� ����ó�� ���ֱ�.
    if (ConstBrightness != 0.f)
    {
        vColor.r *= ConstBrightness;
        vColor.g *= ConstBrightness;
        vColor.b *= ConstBrightness;
    }
    
    return vColor;
}

#endif
