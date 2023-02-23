#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "Fade.h"
#include "sound/SoundSource.h"



/// <summary>
/// PsychicEnergy�Ɋւ��閼�O��ԁB
/// </summary>
namespace nsPsychicEnergy
{
	// �O���錾�B
	namespace nsFade { class Fade; }


	/// <summary>
	/// �^�C�g���Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsTitle
	{
		// �O���錾�B
		class TitleBackGround;
		class TitleCharacter;
		class TitleCamera;
		class TitleSprite;
		class TitleBGM;

		/// <summary>
		/// �^�C�g���N���X
		/// </summary>
		class Title : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			Title() {}
			///<summary>
			/// �f�X�g���N�^
			///</summary>
			~Title();
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update();
			/// <summary>
			/// B�{�^���͉����ꂽ���B
			/// </summary>
			bool& IsPushedB()
			{
				return m_isPushedB;
			}
			/// <summary>
			/// �t�F�[�h�A�E�g���J�n�B
			/// </summary>
			void StartFadeOut()
			{
				m_isFadeOut = true;
			}
		private:
			/// <summary>
			/// �t�F�[�h�A�E�g�B
			/// </summary>
			void FadeOut();

		private:
			LevelRender						m_titleLevel;							// �^�C�g���̃��x��
			std::vector<TitleBackGround*>	m_titleBackGroundArray;					// �^�C�g���Ŏg�p����w�i���f��
			TitleCharacter*					m_titleCharacter = nullptr;				// �^�C�g���Ŏg�p����L�����N�^�[���f��
			TitleCamera*					m_titleCamera = nullptr;				// �^�C�g���Ŏg�p����J����
			TitleSprite*					m_titleSprite = nullptr;				// �^�C�g���Ŏg�p����X�v���C�g
			SpriteRender					m_titleTextSprite;						// �^�C�g���Ɏg�p����e�L�X�g�̃X�v���C�g
			float							m_currentAlpha = 0.0f;					// �X�v���C�g�̏�Z���錻�݂̃��l
			SkyCube*						m_skyCube = nullptr;					// �X�J�C�L���[�u
			nsFade::Fade*					m_fade = nullptr;						// �t�F�[�h
			bool							m_isWaitFadeout = false;				// �t�F�[�h�A�E�g��ҋ@���Ă��邩
			bool							m_isFadeOut = false;					// �t�F�[�h�A�E�g����
			TitleBGM*						m_titleBGM = nullptr;					// �^�C�g��BGM�N���X
			SoundSource*					m_decisionSound = nullptr;				// ���艹�̉���
			bool							m_isPushedB = false;					// B�{�^���͉����ꂽ��

		};
	}
}


