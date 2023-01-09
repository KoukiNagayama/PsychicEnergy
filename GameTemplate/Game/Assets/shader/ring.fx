
///////////////////////////////////////
// �\���́B
///////////////////////////////////////
struct SVSIn
{
    float4 pos : POSITION; //���_���W�B
    float3 normal : NORMAL; //�@���B
    float2 uv : TEXCOORD0; //UV���W�B
    float3 tangent : TANGENT; //�ڃx�N�g���B
    float3 biNormal : BINORMAL; //�]�x�N�g���B
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; //���W�B
    float3 normal : NORMAL; //�@���B
    float3 tangent : TANGENT; //�ڃx�N�g���B
    float3 biNormal : BINORMAL; //�]�x�N�g���B
    float2 uv : TEXCOORD0; //UV���W�B
    float3 worldPos : TEXCOORD1; // ���[���h���W
};

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
}


///////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
Texture2D<float4> g_normal : register(t1); //�@���}�b�v
Texture2D<float4> g_spacular : register(t2); //�X�y�L�����}�b�v

///////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////
sampler g_sampler : register(s0);



// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
	// ���_���W�����[���h���W�n�ɕϊ�����B
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
  //  psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.uv = vsIn.uv;
	
    return psIn;
}

/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
 
    return albedoColor;
}
