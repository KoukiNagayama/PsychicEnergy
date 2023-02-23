#include "stdafx.h"
#include "LockOn.h"
#include "Ring.h"
#include "Player.h"

namespace
{
	const float RESET_RATING_POINT = 1000000000.0f;						// �]���_�����Z�b�g����l
	const float RIDIAN_ANGLE_IDENTIFY_TARGET_IS_IN_VIEW = Math::PI / 4;	// �ڕW�����E���ɂ���Ǝ��ʂ���p�x(Degree�P��)
	const float HALF_HEIGHT_OF_FRAME_BUFFER = FRAME_BUFFER_H * 0.5f;	// �t���[���o�b�t�@�̏c���̔���
	const float HALF_WIDTH_OF_FRAME_BUFFER = FRAME_BUFFER_W * 0.5f;		// �t���[���o�b�t�@�̉����̔���
	const float SPACING_FROM_EDGE_IN_SCREEN_POSITION = 50.0f;			// �X�N���[�����W�ɂ������ʒ[�Ƃ̊Ԋu
	const int	MIN_NUM_TARGET = 1;										// �ڕW�̍ŏ���
	const Vector3 SPRITE_SCALE = { 0.3f, 0.3f, 1.0f };					// �X�v���C�g�̊g�嗦
	const Vector2 SPRITE_SIZE = { 512.0f, 512.0f };						// �X�v���C�g�̕�
	const float MISALIGNMENT_OF_CAMERA_FRONT_AND_TARGET = -0.102f;		// �J�����̐��ʂƖڕW�̂���
}

namespace nsPsychicEnergy
{

	namespace nsLockOn
	{
		bool LockOn::Start()
		{

			m_player = FindGO<nsPlayer::Player>("player");

			m_lockOnSprite.Init(
				"Assets/sprite/lockOn/lockOn/lockOn.DDS",
				SPRITE_SIZE.x,
				SPRITE_SIZE.y
			);
			m_lockOnSprite.SetScale(SPRITE_SCALE);
			m_lockOnSprite.Update();

			m_arrowSprite.Init(
				"Assets/sprite/lockOn/arrow/arrow_2.DDS",
				SPRITE_SIZE.x,
				SPRITE_SIZE.y
			);
			m_arrowSprite.SetScale(SPRITE_SCALE);
			m_arrowSprite.Update();

			m_camForward = g_camera3D->GetForward();
			// �����̖ڕW�����߂�
			DecideTarget();

			return true;
		}

		void LockOn::Update()
		{
			m_camForward = g_camera3D->GetForward();
			if (m_targetRing->IsDead()
				|| g_pad[0]->IsTrigger(enButtonRB2)) {
				DecideTarget();
			}

			if (m_isDisable) {
				return;
			}

			IdentifyIfTargetIsInView();
			if (m_isTargetInView) {
				m_lockOnSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
				m_lockOnSprite.Update();
			}
			else {
				CalculateScreenPositionOfSpriteForArrow();
			}


		}

		void LockOn::DecideTarget()
		{
			// �S�Ẵ����O���������A�i�[���Ă���
			m_ringArray = FindGOs<nsRing::Ring>("ring");
			// �����Ɗp�x����]���_���v�Z
			// �������߂��A�p�x�����������̂قǕ]���_���Ⴂ
			// �]���_����ԒႢ���̂����b�N�I������
			if (m_ringArray.size() < MIN_NUM_TARGET) {
				m_isDisable = true;
				return;
			}

			// �ŏ��̕]���_�����߂ɐݒ肵�Ă���
			m_minRatingPoint = RESET_RATING_POINT;

			for (auto& ring : m_ringArray) {
				// �J�������烊���O�։��т�x�N�g��
				Vector3 toRingPos = ring->GetPosition() - g_camera3D->GetPosition();
				// ����
				float distance = toRingPos.Length();

				toRingPos.Normalize();
				// �p�x
				float angle = acosf(m_camForward.Dot(toRingPos));

				// �p�x�Ƌ�������]���_���v�Z
				float ratingPoint = angle * distance;
				if (ratingPoint > m_minRatingPoint) {
					return;
				}
				// �]���_�Ƃ��̃����O���L�^����
				m_targetRing = ring;
				m_minRatingPoint = ratingPoint;

			}
		}

		void LockOn::IdentifyIfTargetIsInView()
		{
			// �ڕW�̃��[���h���W�B
			Vector3 targetPos = m_targetRing->GetPosition();
			// �ڕW�̃��[���h���W����X�N���[�����W���v�Z����B
			g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, targetPos);

			// ���[���h��Ԃł̃v���C���[�̍��W����ڕW�̍��W�ւ̃x�N�g�������߂�B
			m_playerToTargetPos = targetPos - m_player->GetPosition();
			// �J�����̐��ʍ��W�B
			Vector3 camForward = g_camera3D->GetForward();
			m_playerToTargetPos.Normalize();

			// �J�����̐��ʕ������ڕW�ւ̃x�N�g���ɑ΂��āA�ǂ�قǗ���Ă��邩�B
			// �ڕW�ɋ߂��B
			if (camForward.Dot(m_playerToTargetPos) > MISALIGNMENT_OF_CAMERA_FRONT_AND_TARGET) {

				// �X�N���[�����W�͒��S��(0,0)�̂��߃t���[���o�b�t�@�̂��ꂼ��̕��̔����̒l��p��
				if (HALF_HEIGHT_OF_FRAME_BUFFER >= m_screenPos.y
					&& -HALF_HEIGHT_OF_FRAME_BUFFER <= m_screenPos.y
					&& HALF_WIDTH_OF_FRAME_BUFFER >= m_screenPos.x
					&& -HALF_WIDTH_OF_FRAME_BUFFER <= m_screenPos.x) {
					// ��ʓ��ɖڕW������
					m_isTargetInView = true;

				}
				else {
					// ��ʓ��ɖڕW���Ȃ�
					m_isTargetInView = false;
				}
				// �ڕW�͌��ɂ͂Ȃ�
				m_isBehind = false;
			}
			// �J�����̐��ʕ������ڕW�ɑ΂��ĉ����B
			else {
				// ��ʓ��ɖڕW���Ȃ�
				m_isTargetInView = false;
				// �ڕW�͌��ɂ���
				m_isBehind = true;
			}
		}

		void LockOn::CalculateScreenPositionOfSpriteForArrow()
		{
			// ����\������ꍇ��y���W�̏C�����@�͂ǂ̏ꍇ�ł�����
			// y���W����ʏ㕔�͈̔͂𒴂����ꍇ�B
			if (HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.y) {
				// �㕔�ɌŒ�B
				m_screenPos.y = HALF_HEIGHT_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
			}
			// y���W����ʉ����͈̔͂𒴂����ꍇ�B
			else if (-HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.y) {
				// �����ɌŒ�B
				m_screenPos.y = -HALF_HEIGHT_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
			}

			// x���W�̏C��
			// �ڕW�ɑ΂��ċt�����̏ꍇ�̏C��
			if (m_isBehind) {

				// �ǂ̒[�ɋ߂������߂�B
				SeekWhichEdgeIsClose();

				// ��ʉE�ɃX�N���[�����W���߂��ꍇ�B
				if (m_isNearRightEdge) {
					// ��ʉE�ɌŒ�B
					m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// ��ʍ��ɃX�N���[�����W���߂��ꍇ�B
				else {
					// ��ʍ��ɌŒ�B
					m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// ��]���ɊO���Ɍ����邽�߂ɋt�ɂ���
				m_screenPos.y = -m_screenPos.y;


			}
			// ���ʌ����̏ꍇ�̏C��
			else {
				// x���W����ʉE�͈̔͂𒴂����ꍇ�B
				if (HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION < m_screenPos.x) {
					// ��ʉE�ɌŒ�B
					m_screenPos.x = HALF_WIDTH_OF_FRAME_BUFFER - SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
				// x���W����ʍ��͈̔͂𒴂����ꍇ�B
				else if (-HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION > m_screenPos.x) {
					// ��ʍ��ɌŒ�B
					m_screenPos.x = -HALF_WIDTH_OF_FRAME_BUFFER + SPACING_FROM_EDGE_IN_SCREEN_POSITION;
				}
			}
			// �X�v���C�g�̕\�����W���X�V�B
			m_arrowSprite.SetPosition(Vector3(m_screenPos.x, m_screenPos.y, 0.0f));

			// �I�u�W�F�N�g�ɖ���������B
			Quaternion rot;
			rot.SetRotation(Vector3::Up, Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
			m_arrowSprite.SetRotation(rot);

			// �X�v���C�g���X�V�B
			m_arrowSprite.Update();
		}

		void LockOn::SeekWhichEdgeIsClose()
		{
			// ���E�ǂ���̉�ʒ[�ɋ߂������߂�

			// �E��
			if (m_playerToTargetPos.Dot(g_camera3D->GetRight()) > 0.0f) {
				m_isNearRightEdge = true;
			}
			// ����
			else {
				m_isNearRightEdge = false;
			}

		}

		void LockOn::Render(RenderContext& rc)
		{
			if (m_isDisable) {
				// �`�悵�Ȃ�
				return;
			}
			if (m_isTargetInView) {
				m_lockOnSprite.Draw(rc);
			}
			else {
				m_arrowSprite.Draw(rc);
			}
		}

	}
}

