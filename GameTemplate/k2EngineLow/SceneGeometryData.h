#pragma once
#include "GeometryData.h"

namespace nsK2EngineLow
{
	/// <summary>
	/// �V�[���̃W�I���g�����
	/// </summary>
	class SceneGeometryData : public Noncopyable
	{
	public:
        /// <summary>
        /// �W�I���g������o�^
        /// </summary>
        /// <param name="geomData">�􉽊w�f�[�^</param>
        void RegisterGeometryData(GeometryData* geomData)
        {
            m_geometryDatas.push_back(geomData);
        }
        /// <summary>
        /// �W�I���g�����̓o�^�����B
        /// </summary>
        /// <param name="geomData"></param>
        void UnregisterGeometryData(GeometryData* geomData)
        {
            auto it = std::find(
                m_geometryDatas.begin(),
                m_geometryDatas.end(),
                geomData
            );
            if (it != m_geometryDatas.end()) {
                m_geometryDatas.erase(it);
            }
        }
        /// <summary>
        /// �􉽊w�����X�V�B
        /// </summary>
        void Update();
        /// <summary>
        /// �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W���擾
        /// </summary>
        /// <returns>�r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W</returns>
        const Vector3& GetShadowCasterMaxPositionInViewFrustum() const
        {
            return m_shadowCasterMaxPositionInViewFrustum;
        }
        /// <summary>
        /// �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W
        /// </summary>
        /// <returns>�r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W</returns>
        const Vector3& GetShadowCasterMinPositionInViewFrustum() const
        {
            return m_shadowCasterMinPositionInViewFrustum;
        }
        /// <summary>
        /// �V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł�����
        /// </summary>
        /// <returns></returns>
        bool IsBuildshadowCasterGeometryData() const
        {
            return m_isBuildshadowCasterGeometryData;
        }
    private:
        Vector3 m_shadowCasterMaxPositionInViewFrustum;     // �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W�B
        Vector3 m_shadowCasterMinPositionInViewFrustum;     // �r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W�B
        bool m_isBuildshadowCasterGeometryData = false;     // �V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł����H
        std::list< GeometryData* > m_geometryDatas;         // �V�[�����\������W�I���g�����̃��X�g�B
    };
}


