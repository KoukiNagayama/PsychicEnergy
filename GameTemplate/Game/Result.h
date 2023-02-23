#pragma once
/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsFade { class Fade; }
	namespace nsTimer { class DisplayGameTimer; }

	/// <summary>
	/// ���U���g�Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsResult
	{
		// �O���錾�B
		class ResultBGM;
		class ResultSprite;

		/// <summary>
		/// ���U���g�N���X
		/// </summary>
		class Result : public IGameObject
		{
		public:
			/// <summary>
			/// ���U���g�̏�ԁB
			/// </summary>
			enum EnResultState
			{
				enResultState_GameClear,	// �Q�[���N���A�B
				enResultState_TimeUp		// �^�C���A�b�v�B
			};
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			Result() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~Result();
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �X�V����
			/// </summary>
			void Update();
			/// <summary>
			/// �`�揈��
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
			/// <summary>
			/// ���U���g�̃X�e�[�g���擾�B
			/// </summary>
			EnResultState& GetResultState()
			{
				return m_resultState;
			}
			/// <summary>
			/// �L�^���ꂽ���Ԃ��擾�B
			/// </summary>
			/// <returns>�v���C�I���܂łɂ����������ԁB</returns>
			int& GetRecordedTime()
			{
				return m_recordedTime;
			}
		private:
			void FadeOut();
		private:

			EnResultState				m_resultState;					// ���U���g�̃X�e�[�g
			SpriteRender				m_resultBGSprite;				// ���U���g�̔w�i�̃X�v���C�g
			int							m_recordedTime = -1;			// �L�^���ꂽ����
			//float				m_pushDeltaTime = 10000.0f;	// 
			bool						m_disable = true;				// �������B
			nsFade::Fade*				m_fade = nullptr;				// �t�F�[�h
			bool						m_isWaitFadeout = false;		// �t�F�[�h�A�E�g��ҋ@���Ă��邩
			SoundSource*				m_decisionSound = nullptr;		// ���艹�B		
			nsTimer::DisplayGameTimer*	m_displayGameTimer = nullptr;	// �^�C�}�[
			ResultBGM*					m_resultBGM = nullptr;			// ���U���g���ɗ����BGM
			ResultSprite*				m_resultSprite = nullptr;		// ���U���g�Ŏg�p����X�v���C�g
		};

	}
}

