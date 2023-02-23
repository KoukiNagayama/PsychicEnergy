#pragma once
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsPlayer { class Player; }
	namespace nsRing { class Ring; }


	/// <summary>
	/// ���b�N�I���̖��O��ԁB
	/// </summary>
	namespace nsLockOn
	{
		/// <summary>
		/// ���b�N�I���N���X�B
		/// </summary>
		class LockOn : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			LockOn() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~LockOn() {}
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �X�V
			/// </summary>
			void Update();
			/// <summary>
			/// �`��
			/// </summary>
			void Render(RenderContext& rc);
		private:
			/// <summary>
			/// �ڕW�����߂�
			/// </summary>
			void DecideTarget();
			/// <summary>
			/// �ڕW�����E���ɂ��邩���ʂ���
			/// </summary>
			void IdentifyIfTargetIsInView();
			/// <summary>
			/// ���p�X�v���C�g�̃X�N���[�����W���v�Z����
			/// </summary>
			void CalculateScreenPositionOfSpriteForArrow();
			/// <summary>
			/// �ǂ̒[�ɋ߂������߂�
			/// </summary>
			void SeekWhichEdgeIsClose();
		private:
			SpriteRender						m_lockOnSprite;							// ���b�N�I�����Ɏg�p����摜
			SpriteRender						m_arrowSprite;							// ���̉摜
			std::vector<nsRing::Ring*>			m_ringArray;							// �����O�̔z��
			nsPlayer::Player*					m_player = nullptr;						// �v���C���[
			bool								m_isLockOn = false;						// ���b�N�I�����Ă���H
			Vector3								m_targetPos = Vector3::Zero;			// �ڕW�ƂȂ���W
			nsRing::Ring*						m_targetRing = nullptr;					// �ڕW�ƂȂ郊���O
			float								m_minRatingPoint = 10000.0f;			// �ŏ��̕]���_
			Vector2								m_screenPos = Vector2::Zero;			// �X�v���C�g��\������X�N���[�����W
			bool								m_isTargetInView = false;				// ���E���ɂ��邩�H
			Vector3								m_camForward = Vector3::AxisZ;			// �J�����̐��ʕ���
			bool								m_isBehind = false;						// ���ɖڕW�����邩�H
			bool								m_isNearRightEdge = false;				// �ڕW����ʂ̉E�[�ɋ߂���
			bool								m_isDisable = false;					// ���������Ă��邩
			Vector3								m_playerToTargetPos = Vector3::Zero;	// �v���C���[����ڕW�ւ̃x�N�g��
		};

	}
}

