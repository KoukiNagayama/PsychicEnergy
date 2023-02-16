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

cbuffer floating : register(b1)
{
    int isFloating;
}

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
    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    //float2 uvOffset[8] =
    //{
    //    float2(0.0f, 1.0f / 1080.0f), //��
    //    float2(0.0f, -1.0f / 1080.0f), //��
    //    float2(1.0f / 1920.0f, 0.0f), //�E
    //    float2(-1.0f / 1920.0f, 0.0f), //��
    //    float2(1.0f / 1920.0f, 1.0f / 1080.0f), //�E��
    //    float2(-1.0f / 1920.0f, 1.0f / 1080.0f), //����
    //    float2(1.0f / 1920.0f, -1.0f / 1080.0f), //�E��
    //    float2(-1.0f / 1920.0f, -1.0f / 1080.0f) //����
    //};
    
    //float2 uvOffset[8] =
    //{
    //    float2(0.0f, 1.0f / 1440.0f), //��
    //    float2(0.0f, -1.0f / 1440.0f), //��
    //    float2(1.0f / 2560.0f, 0.0f), //�E
    //    float2(-1.0f / 2560.0f, 0.0f), //��
    //    float2(1.0f / 2560.0f, 1.0f / 1440.0f), //�E��
    //    float2(-1.0f / 2560.0f, 1.0f / 1440.0f), //����
    //    float2(1.0f / 2560.0f, -1.0f / 1440.0f), //�E��
    //    float2(-1.0f / 2560.0f, -1.0f / 1440.0f) //����
    //};
    
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 2520.0f), //��
        float2(0.0f, -1.0f / 2520.0f), //��
        float2(1.0f / 4480.0f, 0.0f), //�E
        float2(-1.0f / 4480.0f, 0.0f), //��
        float2(1.0f / 4480.0f, 1.0f / 2520.0f), //�E��
        float2(-1.0f / 4480.0f, 1.0f / 2520.0f), //����
        float2(1.0f / 4480.0f, -1.0f / 2520.0f), //�E��
        float2(-1.0f / 4480.0f, -1.0f / 2520.0f) //����
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
    
    if (abs(depth - depth2) > 0.0005f)
    {
        float4 sceneColor = g_sceneTexture.Sample(g_sampler, In.uv);
        float4 edgeColor;
        
        // ���Z�����̂��߃V�[���J���[�ɑ������Ƃ��ɖړI�̐F�ɂȂ�l���o�͂���
        if (isFloating)
        {
            // ���Z�����Ő��F�ɂȂ�l���o�͂���
            edgeColor.x = 0.68f - sceneColor.x;
            edgeColor.y = 0.87f - sceneColor.y;
            edgeColor.z = 0.89f - sceneColor.z;
            edgeColor.w = 0.0f;
            return edgeColor;
        }
        else
        {
            // ���Z�����ō��F�ɂȂ�l���o�͂���
            edgeColor.x = -sceneColor.x;
            edgeColor.y = -sceneColor.y;
            edgeColor.z = -sceneColor.z;
            edgeColor.w = 0.0f;
            return edgeColor;
        }
    }

    // �֊s���ł͂Ȃ����ߍ������ɒl�����������Ȃ�Ȃ��悤�Ƀs�N�Z����0�̒l�œh��Ԃ�
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

Texture2D<float4> g_edgeTexture : register(t0);

float4 PSOutLineFinal(PSInput In) : SV_Target0
{
    float4 combineColor = g_edgeTexture.Sample(g_sampler, In.uv);
    return combineColor;
}