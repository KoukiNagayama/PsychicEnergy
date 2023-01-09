
///////////////////////////////////////
// 構造体。
///////////////////////////////////////
struct SVSIn
{
    float4 pos : POSITION; //頂点座標。
    float3 normal : NORMAL; //法線。
    float2 uv : TEXCOORD0; //UV座標。
    float3 tangent : TANGENT; //接ベクトル。
    float3 biNormal : BINORMAL; //従ベクトル。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; //座標。
    float3 normal : NORMAL; //法線。
    float3 tangent : TANGENT; //接ベクトル。
    float3 biNormal : BINORMAL; //従ベクトル。
    float2 uv : TEXCOORD0; //UV座標。
    float3 worldPos : TEXCOORD1; // ワールド座標
};

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
}


///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normal : register(t1); //法線マップ
Texture2D<float4> g_spacular : register(t2); //スペキュラマップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);



// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
	// 頂点座標をワールド座標系に変換する。
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
  //  psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.uv = vsIn.uv;
	
    return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
 
    return albedoColor;
}
