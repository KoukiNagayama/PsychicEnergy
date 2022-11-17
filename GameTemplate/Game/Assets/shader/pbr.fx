///////////////////////////////////////////////////
// 定数
///////////////////////////////////////////////////
static const int NUM_DIRECTIONAL_LIGHT = 4; // ディレクションライトの本数
static const float PI = 3.1415926f; // π
static const int NUM_SHADOW_MAP = 3;

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// ディレクションライト
struct DirectionalLight
{
    float3 direction; // ライトの方向
    float4 color; // ライトの色
};

// ライト用の定数バッファー
cbuffer LightCb : register(b1)
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 eyePos; // カメラの視点
    float specPow; // スペキュラの絞り
    float3 ambientLight; // 環境光
    float4x4 lvp[NUM_SHADOW_MAP];
    //float farClip[NUM_SHADOW_MAP];
    float farClip1;
    float farClip2;
    float farClip3;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION; // モデルの頂点座標
    float3 normal : NORMAL; // 法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV座標
    SSkinVSIn skinVert;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL; // 法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // uv座標
    float3 worldPos : TEXCOORD1; // ワールド空間でのピクセルの座標
    float4 posInLVP[3] : TEXCOORD2;
    float4 posInCamera : TEXCOORD5;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); // アルベドマップ
Texture2D<float4> g_normalMap : register(t1); // 法線マップ
Texture2D<float4> g_metallicSmoothMap : register(t2); // メタリックスムースマップ。rにメタリック、aにスムース
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。
sampler g_sampler : register(s0); // サンプラーステート
Texture2D<float4> g_shadowMap_0 : register(t10);
Texture2D<float4> g_shadowMap_1 : register(t11);
Texture2D<float4> g_shadowMap_2 : register(t12);


///////////////////////////////////////////////////
// 関数
///////////////////////////////////////////////////

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}

// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
///
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
/// <param name="roughness">粗さ。0～1の範囲。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 粗さは0.5で固定。
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
    float dotLH = saturate(dot(L, H));

    // 光源に向かうベクトルとハーフベクトル、
    // 光が平行に入射したときの拡散反射量を求めている
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // 法線と光源に向かうベクトルwを利用して拡散反射率を求める
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (FL * FV * energyFactor);
}

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
    psIn.worldPos = psIn.pos;
    float4 worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.posInCamera = psIn.pos;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    psIn.uv = vsIn.uv;

    psIn.posInLVP[0] = mul(lvp[0], worldPos);
    psIn.posInLVP[1] = mul(lvp[1], worldPos);
    psIn.posInLVP[2] = mul(lvp[2], worldPos);
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
/// ピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
     // 法線を計算
    float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

    // アルベドカラー、スペキュラカラー、金属度、滑らかさをサンプリングする。
    // アルベドカラー（拡散反射光）
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    // スペキュラカラーはアルベドカラーと同じにする。
    float3 specColor = albedoColor;

    // 金属度
    float metallic = g_metallicSmoothMap.Sample(g_sampler, psIn.uv).r;

    // 滑らかさ
    float smooth = g_metallicSmoothMap.Sample(g_sampler, psIn.uv).a;

    // 視線に向かって伸びるベクトルを計算する
    float3 toEye = normalize(eyePos - psIn.worldPos);

    float3 lig = 0;
    for (int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
    {
        // シンプルなディズニーベースの拡散反射を実装する。
        // フレネル反射を考慮した拡散反射を計算
        float diffuseFromFresnel = CalcDiffuseFromFresnel(
            normal, -directionalLight[ligNo].direction, toEye);

        // 正規化Lambert拡散反射を求める
        float NdotL = saturate(dot(normal, -directionalLight[ligNo].direction));
        float3 lambertDiffuse = directionalLight[ligNo].color * NdotL / PI;

        // 最終的な拡散反射光を計算する
        float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

        // Cook-Torranceモデルを利用した鏡面反射率を計算する
        // Cook-Torranceモデルの鏡面反射率を計算する
        float3 spec = CookTorranceSpecular(
            -directionalLight[ligNo].direction, toEye, normal, smooth)
            * directionalLight[ligNo].color;

        // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
        // スペキュラカラーの強さを鏡面反射率として扱う
        spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

        // 滑らかさを使って、拡散反射光と鏡面反射光を合成する
        // 滑らかさが高ければ、拡散反射は弱くなる
        lig += diffuse * (1.0f - smooth) + spec;
    }

    // 環境光による底上げ
    lig += ambientLight * albedoColor;

    Texture2D<float4> shadowMapArray[3];
    shadowMapArray[0] = g_shadowMap_0;
    shadowMapArray[1] = g_shadowMap_1;
    shadowMapArray[2] = g_shadowMap_2;
    
    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    float farClip[3];
    farClip[0] = farClip1;
    farClip[1] = farClip2;
    farClip[2] = farClip3;
    for (int cascadeIndex = 0; cascadeIndex < 3; cascadeIndex++)
    {
        
        //if (farClip[cascadeIndex] > psIn.posInCamera.z)
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
                        finalColor.xyz *= 0.5f;

                        // 影を落とせたので終了
                     
                    }
                    
                    break;
                }
            }
        }
    }
    
    return finalColor;
}
