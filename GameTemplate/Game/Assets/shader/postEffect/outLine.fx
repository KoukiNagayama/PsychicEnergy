/*!
 * @brief	�|�X�g�G�t�F�N�g�ŗ֊s���`��p�V�F�[�_�[�B
 */

///////////////////////////////////////////
// �萔�o�b�t�@�[
///////////////////////////////////////////
cbuffer cb : register(b0)
{
    float4x4 mvp; //�r���[�v���W�F�N�V�����s��B
    float4 mulColor;
};

///////////////////////////////////////////////////
// �\����
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
// �O���[�o���ϐ�
///////////////////////////////////////////
Texture2D<float4> g_depthTexture : register(t0);
sampler g_sampler : register(s0);
Texture2D<float4> g_sceneTexture : register(t0);
Texture2D<float4> g_edgeTexture : register(t0);



PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}


float4 PSSamplingEdge(PSInput In) : SV_Target0
{
    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //��
        float2(0.0f, -1.0f / 900.0f), //��
        float2(1.0f / 1600.0f, 0.0f), //�E
        float2(-1.0f / 1600.0f, 0.0f), //��
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //����
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //����
    };
    
    // �[�x�l
    // ���̃s�N�Z���̐[�x�l���擾
    float depth = g_depthTexture.Sample(g_sampler, In.uv);
  
        // �ߖT8�e�N�Z���̐[�x�l�̕��ϒl���v�Z����
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, In.uv + uvOffset[i]);
    }
    depth2 /= 8.0f;
    
    if (abs(depth - depth2) > 0.0000455f)
    {
        // ��
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

float4 PSOutLineFinal(PSInput In) : SV_Target0
{
    float combine = g_sceneTexture.Sample(g_sampler, In.uv);
    float4 combineColor = float4(combine, combine, combine, 1.0f);
    return combineColor;
}