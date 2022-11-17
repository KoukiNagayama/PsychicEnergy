/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const int NUM_DIRECTIONAL_LIGHT = 4; // ディレクションライトの本数
static const int NUM_SHADOW_MAP = 3;

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
    float3 normal : NORMAL;
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float2 uv : TEXCOORD0; //uv座標。
    float3 normal : NORMAL;
};
// ディレクションライト
struct DirectionalLight
{
    float3 direction; // ライトの方向
    float4 color; // ライトの色
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
// ライト用の定数バッファー
cbuffer LightingCb : register(b1)
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 eyePos; // カメラの視点
    float specPow; // スペキュラの絞り
    float3 ambientLight; // 環境光
    float pad;
    float4x4 lvp[NUM_SHADOW_MAP];
    float farClip[NUM_SHADOW_MAP];
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);       // アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // ボーン行列。
sampler g_sampler : register(s0);                       // サンプラステート。
Texture2D<float4> g_toonTexture : register(t10);        // トゥーンマップ
////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.uv = vsIn.uv;

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダー。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //モデルのテクスチャから色をフェッチする
    float4 albedoColor = g_albedoTexture.Sample(g_sampler, psIn.uv);

    //ハーフランバート拡散照明によるライティング計算
    float p = dot(psIn.normal * -1.0f, directionalLight[0].direction.xyz);
    p = p * 0.2f + 0.5f;
    //p = p * 0.5f + 0.5f;
    //p = p * p;
    p = p * p + 0.45f;
    


    //計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
    float4 Col = g_toonTexture.Sample(g_sampler, float2(p, 0.0f));

    albedoColor.x *= Col.x;
    albedoColor.y *= Col.y;
    albedoColor.z *= Col.z;
    return albedoColor;
}
