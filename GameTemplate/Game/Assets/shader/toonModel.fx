/*!
 * @brief	�V���v���ȃ��f���V�F�[�_�[�B
 */

static const int NUM_DIRECTIONAL_LIGHT = 4; // �f�B���N�V�������C�g�̖{��
static const int NUM_SHADOW_MAP = 3;

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////
//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn
{
    float4 pos : POSITION; //���f���̒��_���W�B
    float2 uv : TEXCOORD0; //UV���W�B
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
    float3 normal : NORMAL;
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0; //uv���W�B
    float3 normal : NORMAL;
};
// �f�B���N�V�������C�g
struct DirectionalLight
{
    float3 direction; // ���C�g�̕���
    float4 color; // ���C�g�̐F
};

////////////////////////////////////////////////
// �萔�o�b�t�@�B
////////////////////////////////////////////////
//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
// ���C�g�p�̒萔�o�b�t�@�[
cbuffer LightingCb : register(b1)
{
    DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
    float3 eyePos; // �J�����̎��_
    float specPow; // �X�y�L�����̍i��
    float3 ambientLight; // ����
    float pad;
    float4x4 lvp[NUM_SHADOW_MAP];
    float farClip[NUM_SHADOW_MAP];
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedoTexture : register(t0);       // �A���x�h�}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); // �{�[���s��B
sampler g_sampler : register(s0);                       // �T���v���X�e�[�g�B
Texture2D<float4> g_toonTexture : register(t10);        // �g�D�[���}�b�v
////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////

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
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.uv = vsIn.uv;

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
/// �s�N�Z���V�F�[�_�[�B
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //���f���̃e�N�X�`������F���t�F�b�`����
    float4 albedoColor = g_albedoTexture.Sample(g_sampler, psIn.uv);

    //�n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
    float p = dot(psIn.normal * -1.0f, directionalLight[0].direction.xyz);
    p = p * 0.2f + 0.5f;
    //p = p * 0.5f + 0.5f;
    //p = p * p;
    p = p * p + 0.45f;
    


    //�v�Z���ʂ��g�D�[���V�F�[�_�[�p�̃e�N�X�`������F���t�F�b�`����
    float4 Col = g_toonTexture.Sample(g_sampler, float2(p, 0.0f));

    albedoColor.x *= Col.x;
    albedoColor.y *= Col.y;
    albedoColor.z *= Col.z;
    return albedoColor;
}
