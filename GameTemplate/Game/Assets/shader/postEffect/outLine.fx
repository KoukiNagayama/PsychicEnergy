/*!
 * @brief	ポストエフェクト版輪郭線描画用シェーダー。
 */

///////////////////////////////////////////
// 定数バッファー
///////////////////////////////////////////
cbuffer cb : register(b0)
{
    float4x4 mvp; //ビュープロジェクション行列。
    float4 mulColor;
};

cbuffer floating : register(b1)
{
    int isFloating;
}

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////

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

///////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////
Texture2D<float4> g_depthTexture : register(t0);
Texture2D<float4> g_sceneTexture : register(t1);
sampler g_sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

float4 PSSamplingEdge(PSInput In) : SV_Target0
{
    // 近傍8テクセルへのUVオフセット
    //float2 uvOffset[8] =
    //{
    //    float2(0.0f, 1.0f / 1080.0f), //上
    //    float2(0.0f, -1.0f / 1080.0f), //下
    //    float2(1.0f / 1920.0f, 0.0f), //右
    //    float2(-1.0f / 1920.0f, 0.0f), //左
    //    float2(1.0f / 1920.0f, 1.0f / 1080.0f), //右上
    //    float2(-1.0f / 1920.0f, 1.0f / 1080.0f), //左上
    //    float2(1.0f / 1920.0f, -1.0f / 1080.0f), //右下
    //    float2(-1.0f / 1920.0f, -1.0f / 1080.0f) //左下
    //};
    
    //float2 uvOffset[8] =
    //{
    //    float2(0.0f, 1.0f / 1440.0f), //上
    //    float2(0.0f, -1.0f / 1440.0f), //下
    //    float2(1.0f / 2560.0f, 0.0f), //右
    //    float2(-1.0f / 2560.0f, 0.0f), //左
    //    float2(1.0f / 2560.0f, 1.0f / 1440.0f), //右上
    //    float2(-1.0f / 2560.0f, 1.0f / 1440.0f), //左上
    //    float2(1.0f / 2560.0f, -1.0f / 1440.0f), //右下
    //    float2(-1.0f / 2560.0f, -1.0f / 1440.0f) //左下
    //};
    
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 2520.0f), //上
        float2(0.0f, -1.0f / 2520.0f), //下
        float2(1.0f / 4480.0f, 0.0f), //右
        float2(-1.0f / 4480.0f, 0.0f), //左
        float2(1.0f / 4480.0f, 1.0f / 2520.0f), //右上
        float2(-1.0f / 4480.0f, 1.0f / 2520.0f), //左上
        float2(1.0f / 4480.0f, -1.0f / 2520.0f), //右下
        float2(-1.0f / 4480.0f, -1.0f / 2520.0f) //左下
    };
    
    // 深度値
    // このピクセルの深度値を取得
    float depth = g_depthTexture.Sample(g_sampler, In.uv);
  
        // 近傍8テクセルの深度値の平均値を計算する
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, In.uv + uvOffset[i]);
    }
    depth2 /= 8.0f;
    
    if (abs(depth - depth2) > 0.0005f)
    {
        float4 sceneColor = g_sceneTexture.Sample(g_sampler, In.uv);
        float4 edgeColor;
        
        // 加算合成のためシーンカラーに足したときに目的の色になる値を出力する
        if (isFloating)
        {
            // 加算合成で水色になる値を出力する
            edgeColor.x = 0.68f - sceneColor.x;
            edgeColor.y = 0.87f - sceneColor.y;
            edgeColor.z = 0.89f - sceneColor.z;
            edgeColor.w = 0.0f;
            return edgeColor;
        }
        else
        {
            // 加算合成で黒色になる値を出力する
            edgeColor.x = -sceneColor.x;
            edgeColor.y = -sceneColor.y;
            edgeColor.z = -sceneColor.z;
            edgeColor.w = 0.0f;
            return edgeColor;
        }
    }

    // 輪郭線ではないため合成時に値がおかしくならないようにピクセルを0の値で塗りつぶす
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

Texture2D<float4> g_edgeTexture : register(t0);

float4 PSOutLineFinal(PSInput In) : SV_Target0
{
    float4 combineColor = g_edgeTexture.Sample(g_sampler, In.uv);
    return combineColor;
}