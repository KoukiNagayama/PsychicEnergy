/*!
 * @brief	スプライト
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer angleCb : register(b1)
{
    float angle;
}

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
    float PI = 3.14159f;
    
    float2 center = { 0.5f,0.5f };
    float2 up = { 0.5f, 1.0f };
    float2 vector1 = { 0.0, -1.0f };
    float2 vector2 = In.uv - center;
    vector1 = normalize(vector1);
    vector2 = normalize(vector2);
    float deg = acos(dot(vector1, vector2));
    deg = abs(deg);
    
    if (vector2.x < vector1.x)
    {
        deg = PI + (PI - deg);
    }
    if (deg >= angle)
    {
        clip(-1);
    }
    return colorTexture.Sample(Sampler, In.uv);
}