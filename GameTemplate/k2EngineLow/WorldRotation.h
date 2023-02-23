#pragma once

namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsBackGround { class BackGround; }
	namespace nsRing { class Ring; }
}

namespace nsK2EngineLow {

	/// <summary>
	/// ���E�̉�]�̖��O��ԁB
	/// </summary>
	namespace nsWorldRotation
	{

		/// <summary>
		/// ���E�̉�]�N���X�B
		/// </summary>
		class WorldRotation
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			WorldRotation() {}
			/// <summary>
			///	�f�X�g���N�^�B
			/// </summary>
			~WorldRotation() {}
			/// <summary>
			/// �X�V�B
			/// </summary>
			void Update();
			/// <summary>
			/// �o�^�f�[�^��j������B
			/// </summary>
			void DiscardRegisteredData();
			/// <summary>
			/// �w�i�I�u�W�F�N�g����]����Ώۂɒǉ�
			/// </summary>
			/// <param name="modelRender"></param>
			void AddBackGround(nsPsychicEnergy::nsBackGround::BackGround* modelRender)
			{
				m_backGroundArray.push_back(modelRender);
			}
			/// <summary>
			/// �����O����]����Ώۂɒǉ�
			/// </summary>
			/// <param name="modelRender"></param>
			void AddRing(nsPsychicEnergy::nsRing::Ring* modelRender)
			{
				m_ringArray.push_back(modelRender);
			}
			/// <summary>
			/// �v���C���[�̃��[���h�s���o�^
			/// </summary>
			/// <param name="matrix"></param>
			void RegisterPlayerWorldMatrix(const Matrix& matrix)
			{
				m_playerWorldMatrix = nullptr;
				m_playerWorldMatrix = &matrix;
			}
			/// <summary>
			/// �Փ˂����@����ݒ�B
			/// </summary>
			/// <param name="hitNormal"></param>
			void SetHitNormal(const Vector3& hitNormal)
			{
				if (m_isGetNormal) {
					m_hitNormal = hitNormal;
					m_isGetNormal = false;
				}
			}
			/// <summary>
			/// �@�����擾���邩�ǂ�����ݒ�B
			/// </summary>
			/// <param name="isGetNormal"></param>
			void SetIsGetNormal(const bool& isGetNormal)
			{
				m_isGetNormal = isGetNormal;
			}
			/// <summary>
			/// ���[���h�s��̉�]
			/// </summary>
			void RotationWorldMatrix();
			/// <summary>
			/// ���[���h�s��̃��Z�b�g
			/// </summary>
			void ResetWorldMatrix();
			/// <summary>
			/// ���݃��Z�b�g�����ݒ�B
			/// </summary>
			/// <param name="isReseting"></param>
			void SetIsReseting(const bool& isReseting)
			{
				m_isReseting = isReseting;
			}

		private:
			const Matrix*											m_playerWorldMatrix;							// �v���C���[�̃��[���h�s��
			Matrix													m_rotationMatrix = Matrix::Identity;			// ���E�̉�]�s��
			std::vector<nsPsychicEnergy::nsBackGround::BackGround*>	m_backGroundArray;								// �w�i�I�u�W�F�N�g�̔z��
			std::vector <nsPsychicEnergy::nsRing::Ring* >			m_ringArray;									// �����O�̔z��
			Quaternion												m_matrixRot = Quaternion::Identity;				// ��]�s�����邽�߂̃N�H�[�^�j�I��
			Vector3													m_hitNormal = Vector3::Zero;					// �v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
			Vector3													m_lastHitNormal = Vector3::Zero;				// �Ō�ɋL�^���ꂽ�v���C���[���G�ꂽ�I�u�W�F�N�g�̖ʂ̖@��
			bool													m_isGetNormal = false;							// �Փ˂����@�����擾���邩
			Vector3													m_directionOfCurrentReference = Vector3::Up;	// ���݂̊�ƂȂ����(���Z�b�g���Ɏg�p)
			bool													m_isReseting = false;							// ���Z�b�g����
		};

		extern WorldRotation* g_worldRotation;		// ���E�̉�]�N���X�B

	}
}


