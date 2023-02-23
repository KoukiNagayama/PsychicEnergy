#pragma once
#include "Result.h"

/// <summary>
/// PsychicEnergy�̖��O��ԁB
/// </summary>
namespace nsPsychicEnergy
{

	/// <summary>
	/// ���U���g�Ɋւ��閼�O��ԁB
	/// </summary>
	namespace nsResult
	{
		// �O���錾�B
		class Result;

		/// <summary>
		/// ���U���g�Ŏg�p����X�v���C�g�N���X�B
		/// </summary>
		class ResultSprite : public IGameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^�B
			/// </summary>
			ResultSprite() {}
			/// <summary>
			/// �f�X�g���N�^�B
			/// </summary>
			~ResultSprite() {}
			/// <summary>
			/// �J�n�����B
			/// �C���X�^���X�������ɏ��������s���Btrue��Ԃ����Ƃ��ɌĂ΂�Ȃ��Ȃ�B
			/// </summary>
			bool Start();
			/// <summary>
			/// �`�揈���B
			/// </summary>
			/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
			void Render(RenderContext& rc);
		private:
			/// <summary>
			/// �X�v���C�g��������
			/// </summary>
			void InitSprite();
		private:
			SpriteRender	m_numberSprite[3];			// �����̃X�v���C�g(3����)
			SpriteRender	m_textSprite;				// �\������e�L�X�g�̃X�v���C�g
			Result*			m_result = nullptr;			// ���U���g�N���X
			bool			m_isTimeUp = false;			// �^�C���A�b�v�ł��������H
		};
	}
}


