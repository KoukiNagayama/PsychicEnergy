#pragma once

/// <summary>
/// ���̊ւ��鋤�ʃf�[�^
/// </summary>
namespace nsSound
{
	/// <summary>
	/// ���̓o�^�ԍ�
	/// </summary>
	enum EnSoundNumber
	{
		enSoundNumber_PlayerFootStep,		// �v���C���[�̑���
		enSoundNumber_PlayerSliding,		// �v���C���[�̃X���C�f�B���O��
		enSoundNumber_PlayerLanding,		// �v���C���[�̒��n��
		enSoundNumber_PlayerModeChange,		// �v���C���[�̃��[�h�`�F���W��
		enSoundNumber_TitleBGM,				// �^�C�g���ŗ����BGM
		enSoundNumber_MainBGM,				// �C���Q�[�����ɗ����BGM
		enSoundNumber_Decision,				// ���艹
		enSoundNumber_Acquisition,			// �����O�̎擾��
		enSoundNumber_Wind,					// ���̉�
		enSoundNumber_ResultBGM,			// ���U���g�ŗ����BGM
	};

	const float INITIAL_VOLUME = 0.0f;		// �ŏ��̉���
	const float FINAL_VOLUME = 0.0f;		// �Ō�̉���	
	const float BGM_MIN_VOLUME = 0.0f;		// BGM�̍ŏ�����
	const float BGM_MAX_VOLUME = 0.4f;		// BGM�̍ő剹��
	const float DECISION_VOLUME = 0.5f;		// ���艹�̉���
}

