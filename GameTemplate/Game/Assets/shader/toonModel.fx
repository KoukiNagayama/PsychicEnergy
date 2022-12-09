/*!
 * @brief	トゥーン調のモデルシェーダー。
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
    float4 posInLVP[3] : TEXCOORD1;
 //   float4 posInCamera : TEXCOORD4;
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
    int floating;
    float farClip[NUM_SHADOW_MAP];

};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);       // アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // ボーン行列。
sampler g_sampler : register(s0);                       // サンプラステート。
Texture2D<float4> g_toonTexture : register(t10);        // トゥーンマップ
Texture2D<float4> g_shadowMap_0 : register(t11);
Texture2D<float4> g_shadowMap_1 : register(t12);
Texture2D<float4> g_shadowMap_2 : register(t13);
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
int CheckRecieveShadow(SPSIn psIn)
{
    Texture2D<float4> shadowMapArray[3];
    shadowMapArray[0] = g_shadowMap_0;
    shadowMapArray[1] = g_shadowMap_1;
    shadowMapArray[2] = g_shadowMap_2;

    int recieveShadow = 0;
    
    for (int cascadeIndex = 0; cascadeIndex < 3; cascadeIndex++)
    {

        
        // シャドウマップの範囲内かどうか
        //要修正箇所----------------------------------
        //if (farClip[cascadeIndex] > psIn.posInCamera.z)
        //-----------------------------------------------
        {
            // ライトビュースクリーン空間でのZ値を計算する
            float zInLVP = psIn.posInLVP[cascadeIndex].z / psIn.posInLVP[cascadeIndex].w;
            if (zInLVP >= 0.0f && zInLVP <= 1.0f)
            {
                // Zの値を見て、このピクセルがこのシャドウマップに含まれているか判定
                float2 shadowMapUV = psIn.posInLVP[cascadeIndex].xy / psIn.posInLVP[cascadeIndex].w;
                shadowMapUV *= float2(0.5f, -0.5f);
                shadowMapUV += 0.5f;

                // シャドウマップUVが範囲内か判定
                if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
                    && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
                {
                    // シャドウマップから値をサンプリング
                    float2 shadowValue = shadowMapArray[cascadeIndex].Sample(g_sampler, shadowMapUV).xy;

                    // まずこのピクセルが遮蔽されているか調べる
                    if (zInLVP >= shadowValue.r + 0.0002f)
                    {
                        //recieveShadow += 1;
                    }
                    break;
                }
            }
        }
    }
            
    // トゥーンマップにて影が落ちているか
    float p = dot(psIn.normal * -1.0f, directionalLight[0].direction.xyz);
    p = p * 0.2f + 0.5f;
        //p = p * 0.5f + 0.5f;
        //p = p * p;
    p = p * p + 0.45f;
        
    if (p > (1.0f / 3.0f) && p < (1.0 / 3.0f * 2.0f))
    {
        recieveShadow += 1;
    }
        
    return recieveShadow;
};

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
    psIn.posInLVP[0] = mul(lvp[0], psIn.pos);
    psIn.posInLVP[1] = mul(lvp[1], psIn.pos);
    psIn.posInLVP[2] = mul(lvp[2], psIn.pos);
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
    // モデルのテクスチャから色をフェッチする
    float4 albedoColor = g_albedoTexture.Sample(g_sampler, psIn.uv);
    // 浮遊状態の場合はアルベドカラーを編集する
    if (floating == 1)
    {
        float albedoRed = dot(albedoColor.xyz, float3(1.0f, 0.0f, 0.0f));
        
        albedoColor.x = (1.0f - albedoRed) * 0.5f;
        albedoColor.y *= 0.1f;
        albedoColor.z *= 0.1f;
    }
    

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
    
    //float4 Col;
    //switch (CheckRecieveShadow(psIn))
    //{
    //    case 1:
    //        Col = g_toonTexture.Sample(g_sampler, float2(0.0f, 0.0f));
    //        albedoColor.x *= Col.x;
    //        albedoColor.y *= Col.y;
    //        albedoColor.z *= Col.z;
    //        return albedoColor;
    //    case 2:
    //        Col = g_toonTexture.Sample(g_sampler, float2(-0.5f, 0.0f));
    //        albedoColor.x *= Col.x;
    //        albedoColor.y *= Col.y;
    //        albedoColor.z *= Col.z;
    //        return albedoColor;
    //    default:
    //        return albedoColor;
    //}


}
