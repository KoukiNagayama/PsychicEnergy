///////////////////////////////////////////////////
// �萔
///////////////////////////////////////////////////
static const int NUM_DIRECTIONAL_LIGHT = 4; // �f�B���N�V�������C�g�̖{��
static const float PI = 3.1415926f; // ��
static const int NUM_SHADOW_MAP = 3;

///////////////////////////////////////////////////
// �\����
///////////////////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// �f�B���N�V�������C�g
struct DirectionalLight
{
    float3 direction; // ���C�g�̕���
    float4 color; // ���C�g�̐F
};

// ���C�g�p�̒萔�o�b�t�@�[
cbuffer LightCb : register(b1)
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 eyePos; // �J�����̎��_
    float specPow; // �X�y�L�����̍i��
    float3 ambientLight; // ����
    float4x4 lvp[NUM_SHADOW_MAP];
    //float farClip[NUM_SHADOW_MAP];
    float farClip1;
    float farClip2;
    float farClip3;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV���W
    SSkinVSIn skinVert;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // uv���W
    float3 worldPos : TEXCOORD1; // ���[���h��Ԃł̃s�N�Z���̍��W
    float4 posInLVP[3] : TEXCOORD2;
    float4 posInCamera : TEXCOORD5;
};

///////////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); // �A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1); // �@���}�b�v
Texture2D<float4> g_metallicSmoothMap : register(t2); // ���^���b�N�X���[�X�}�b�v�Br�Ƀ��^���b�N�Aa�ɃX���[�X
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
sampler g_sampler : register(s0); // �T���v���[�X�e�[�g
Texture2D<float4> g_shadowMap_0 : register(t10);
Texture2D<float4> g_shadowMap_1 : register(t11);
Texture2D<float4> g_shadowMap_2 : register(t12);


///////////////////////////////////////////////////
// �֐�
///////////////////////////////////////////////////

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}

// �x�b�N�}�����z���v�Z����
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// �t���l�����v�Z�BSchlick�ߎ����g�p
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torrance���f���̋��ʔ��˂��v�Z
/// </summary>
/// <param name="L">�����Ɍ������x�N�g��</param>
/// <param name="V">���_�Ɍ������x�N�g��</param>
/// <param name="N">�@���x�N�g��</param>
/// <param name="metallic">�����x</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
    // �����x�������قǃt���l�����˂͑傫���Ȃ�
    float f0 = metallic;

    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D�����x�b�N�}�����z��p���Čv�Z����
    float D = Beckmann(microfacet, NdotH);

    // F����Schlick�ߎ���p���Čv�Z����
    float F = SpcFresnel(f0, VdotH);

    // G�������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m�������߂�
    float m = PI * NdotV * NdotH;

    // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
    return max(F * D * G / m, 0.0);
}

/// <summary>
/// �t���l�����˂��l�������g�U���˂��v�Z
/// </summary>
/// <remark>
/// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
/// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
/// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
/// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
///
/// </remark>
/// <param name="N">�@��</param>
/// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
/// <param name="V">�����Ɍ������x�N�g���B</param>
/// <param name="roughness">�e���B0�`1�͈̔́B</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-1 �f�B�Y�j�[�x�[�X�̃t���l�����˂ɂ��g�U���˂�^�ʖڂɎ�������B
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);

    // �e����0.5�ŌŒ�B
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(L, H));

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
    // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(N, V));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL * FV * energyFactor);
}

/// <summary>
//�X�L���s����v�Z����B
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
/// ���_�V�F�[�_�[�̃R�A�֐��B
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
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// �s�N�Z���V�F�[�_�[
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
     // �@�����v�Z
    float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

    // �A���x�h�J���[�A�X�y�L�����J���[�A�����x�A���炩�����T���v�����O����B
    // �A���x�h�J���[�i�g�U���ˌ��j
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    // �X�y�L�����J���[�̓A���x�h�J���[�Ɠ����ɂ���B
    float3 specColor = albedoColor;

    // �����x
    float metallic = g_metallicSmoothMap.Sample(g_sampler, psIn.uv).r;

    // ���炩��
    float smooth = g_metallicSmoothMap.Sample(g_sampler, psIn.uv).a;

    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
    float3 toEye = normalize(eyePos - psIn.worldPos);

    float3 lig = 0;
    for (int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
    {
        // �V���v���ȃf�B�Y�j�[�x�[�X�̊g�U���˂���������B
        // �t���l�����˂��l�������g�U���˂��v�Z
        float diffuseFromFresnel = CalcDiffuseFromFresnel(
            normal, -directionalLight[ligNo].direction, toEye);

        // ���K��Lambert�g�U���˂����߂�
        float NdotL = saturate(dot(normal, -directionalLight[ligNo].direction));
        float3 lambertDiffuse = directionalLight[ligNo].color * NdotL / PI;

        // �ŏI�I�Ȋg�U���ˌ����v�Z����
        float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

        // Cook-Torrance���f���𗘗p�������ʔ��˗����v�Z����
        // Cook-Torrance���f���̋��ʔ��˗����v�Z����
        float3 spec = CookTorranceSpecular(
            -directionalLight[ligNo].direction, toEye, normal, smooth)
            * directionalLight[ligNo].color;

        // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
        // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
        spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);

        // ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
        // ���炩����������΁A�g�U���˂͎キ�Ȃ�
        lig += diffuse * (1.0f - smooth) + spec;
    }

    // �����ɂ���グ
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
            // ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
            float zInLVP = psIn.posInLVP[cascadeIndex].z / psIn.posInLVP[cascadeIndex].w;
            if (zInLVP >= 0.0f && zInLVP <= 1.0f)
            {
                // Z�̒l�����āA���̃s�N�Z�������̃V���h�E�}�b�v�Ɋ܂܂�Ă��邩����
                float2 shadowMapUV = psIn.posInLVP[cascadeIndex].xy / psIn.posInLVP[cascadeIndex].w;
                shadowMapUV *= float2(0.5f, -0.5f);
                shadowMapUV += 0.5f;

                // �V���h�E�}�b�vUV���͈͓�������
                if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f
                    && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
                {
                    // �V���h�E�}�b�v����l���T���v�����O
                    float2 shadowValue = shadowMapArray[cascadeIndex].Sample(g_sampler, shadowMapUV).xy;

                    // �܂����̃s�N�Z�����Օ�����Ă��邩���ׂ�
                    if (zInLVP >= shadowValue.r + 0.0002f)
                    {
                        finalColor.xyz *= 0.5f;

                        // �e�𗎂Ƃ����̂ŏI��
                     
                    }
                    
                    break;
                }
            }
        }
    }
    
    return finalColor;
}
