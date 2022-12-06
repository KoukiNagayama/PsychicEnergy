/*!
 * @brief �w�ʉ����o���ɂ��֊s���p���f���V�F�[�_�[
 */

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

cbuffer StateCb : register(b1)
{
    int floating; 
}

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
    float3 normal : NORMAL; // �@��
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0; //uv���W�B
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1); // �@���}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
sampler g_sampler : register(s0); //�T���v���X�e�[�g�B

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
   
    // �@�������Ɋg��
    vsIn.pos += float4(vsIn.normal * 0.3f, 0.0f);
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);


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
/// �s�N�Z���V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    if (floating == 1)
    {
        // ���F�ŏo��
        return float4(0.68f, 0.87f, 0.89f, 1.0f);
    }
    else
    {
        // ���F�ŏo��
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}
