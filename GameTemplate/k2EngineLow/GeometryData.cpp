#include "k2EngineLowPreCompile.h"
#include "GeometryData.h"

namespace nsK2EngineLow
{
	void GeometryData::Update()
	{
		Matrix worldMatrix = m_modelRender->GetWorldMatrix();
		m_aabb.CalcVertexPositions(
			m_aabbWorldVertexPositions,
			worldMatrix
		);

		// �r���[�v���W�F�N�V������Ԃł̍ő�l�ƍŏ��l
		Vector4 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vector4 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX, FLT_MAX };

		// �r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s����擾�B
		const auto& viewProjMatrix = g_renderingEngine->GetViewProjectionMatrixForViewCulling();

		// AABB��Ԃł̍ő�l�ƍŏ��l��ݒ�B
		m_aabbMax = { -FLT_MIN, -FLT_MIN, -FLT_MIN };
		m_aabbMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

		// �r���[�t���X�^���̒��ɉf���Ă��邩
		m_isInViewFrustum = false;

		// AABB�̒��_���v�Z����B
		for (Vector4 v : m_aabbWorldVertexPositions) {

			m_aabbMax.Max({ v.x, v.y, v.z });
			m_aabbMin.Min({ v.x, v.y, v.z });

			// AABB�̒��_�̃��[���h���W���r���[�v���W�F�N�V�������W�ɕϊ�����B
			viewProjMatrix.Apply(v);
			
			// ���_�𐳋K������
			v.x /= fabsf(v.w);
			v.y /= fabsf(v.w);
			v.z /= fabsf(v.w);

			vMax.Max(v);
			vMin.Min(v);
		}

		if (vMax.x > -1.0f
			&& vMax.y > -1.0f
			&& vMax.z > 0.0f
			&& vMin.x < 1.0f
			&& vMin.y < 1.0f
			&& vMin.z < 1.0f
			) {
			//��ʂɉf��B
			m_isInViewFrustum = true;
		}
	}
	bool GeometryData::IsShadowCaster() const
	{
		return m_modelRender->IsShadowCaster();
	}
	void GeometryData::Init(ModelRender* modelRender)
	{
		m_modelRender = modelRender;
		// tkm�t�@�C�����烂�f���̍ŏ����W�ƍő���W�𒲂ׂ�B
		Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

		const auto& tkmFile = modelRender->GetModel().GetTkmFile();
		tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
			for (const auto& vertex : mesh.vertexBuffer) {
				vMax.Max(vertex.pos);
				vMin.Min(vertex.pos);
			}
			});

		// �ő���W�ƍŏ����W����AABB������������B
		m_aabb.Init(vMax, vMin);
	}
}
