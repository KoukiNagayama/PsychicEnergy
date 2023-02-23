#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
#include "WorldRotation.h"

/// <summary>
/// PsychicEnergy�̖��O���
/// </summary>
namespace nsPsychicEnergy 
{
	// �O���錾�B
	namespace nsPlayer { class Player; }
	namespace nsFade { class Fade; }
	namespace nsBackGround { class BackGround; }
	namespace nsRing { class Ring; }
	namespace nsGameCamera { class GameCamera; }
	namespace nsGravityGauge { class GravityGauge; }
	namespace nsSight { class Sight; }
	namespace nsLockOn { class LockOn; }
	namespace nsTimer { class DisplayGameTimer; }
	namespace nsSound { class MainBGM; }

	/// <summary>
	/// �Q�[���̖��O���
	/// </summary>
	namespace nsGame 
	{

		/// <summary>
		/// �C���Q�[���N���X
		/// </summary>
		class Game : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Game();
			/// <summary>
			/// �f�X�g���N�^
			/// </summary>
			~Game();
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������Ɉ�x�������s�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update();
			/// <summary>
			/// �����O���擾
			/// </summary>
			void GetRing()
			{
				m_numOfGetRing++;
			}
			/// <summary>
			/// �C���Q�[���͏I�����Ă��邩
			/// </summary>
			bool& IsFinishedInGame()
			{
				return m_isFinishedInGame;
			}
		private:
			/// <summary>
			/// �t�F�[�h�A�E�g�B
			/// </summary>
			void FadeOut();
		private:
			LevelRender									m_levelRender;					// ���x�������_�[
			nsPlayer::Player*							m_player = nullptr;				// �v���C���[
			nsGameCamera::GameCamera*					m_gameCamera = nullptr;			// �Q�[���J����
			std::vector<nsBackGround::BackGround*>		m_backGroundArray;				// �w�i
			SkyCube*									m_skyCube = nullptr;			// �X�J�C�L���[�u						
			nsGravityGauge::GravityGauge*				m_gravityGauge = nullptr;		// �d�̓Q�[�W
			nsSight::Sight*								m_sight = nullptr;				// �Ə�
			nsRing::Ring*								m_ring = nullptr;				// �����O
			nsLockOn::LockOn*							m_lockOn = nullptr;				// ���b�N�I��
			nsTimer::DisplayGameTimer*					m_displayGameTimer = nullptr;	// �\������^�C�}�[
			nsFade::Fade*								m_fade = nullptr;				// �t�F�[�h
			nsSound::MainBGM*							m_mainBGM = nullptr;			// ���C��BGM
			int											m_maxRing = 0;					// �����O�̍ő吔
			int											m_numOfGetRing = 0;				// �擾���������O�̐�
			bool										m_isFinishedInGame = false;		// �C���Q�[���͏I���������H
		};
	}
}


