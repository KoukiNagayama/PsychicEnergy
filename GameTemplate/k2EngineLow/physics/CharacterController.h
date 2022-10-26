/*!
 * @brief	�L�����N�^�R���g���[���[�B
 */

#pragma once

#include "CapsuleCollider.h"
#include "RigidBody.h"

namespace nsK2EngineLow {
	/*!
	* @brief	�L�����N�^�R���g���[���[�B
	*@details
	* �L�����N�^�[�Ɣw�i�̏Փˉ������s�����Ƃ��ł��܂��B\n
	* ���L�̃T���v���v���O�������Q�l�ɂ��Ă��������B\n
	* Sample/Sample07
	*/
	class CharacterController {
	public:
		CharacterController() {

		}
		~CharacterController()
		{
			RemoveRigidBoby();
		}
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="radius">�J�v�Z���R���C�_�[�̔��a�B</param>
		/// <param name="height">�J�v�Z���R���C�_�[�̍����B</param>
		/// <param name="position">�����ʒu�B</param>
		/// <param name="rotation">��]�B�I�C���[�p</param>
		void Init(float radius, float height, const Vector3& position, Vector3& rotation);
		/// <summary>
		/// ���s�B
		/// </summary>
		/// <param name="moveSpeed">�ړ����x�B</param>
		/// <param name="deltaTime">�o�ߎ��ԁB�P�ʂ͕b�B</param>
		/// <param name="rotation">��]�B�I�C���[�p�B</param>
		/// <returns></returns>
		const Vector3& Execute(Vector3& moveSpeed, float deltaTime, Vector3& rotation);
		const Vector3& Float(Vector3& moveSpeed, float deltaTime, Vector3& rotation);
		/// <summary>
		/// ���W���擾�B
		/// </summary>
		/// <returns>���W�B</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// �W�����v��������B
		/// </summary>
		/// <returns>�W�����v���Ȃ�true�B</returns>
		bool IsJump() const
		{
			return m_isJump;
		}
		/// <summary>
		/// �n�ʏ�ɂ��邩����B
		/// </summary>
		/// <returns>�n�ʏ�ɂ���Ȃ�true�B</returns>
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/// <summary>
		/// �R���C�_�[���擾�B
		/// </summary>
		CCapsuleCollider* GetCollider()
		{
			return &m_collider;
		}
		/// <summary>
		/// ���̂��擾�B
		/// </summary>
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/// <summary>
		/// ���̂𕨗��G���W������폜�B
		/// </summary>
		void RemoveRigidBoby();
	private:
		bool				m_isInited = false;				//!<�������ς݁H
		Vector3 			m_position;						//!<���W�B
		bool 				m_isJump = false;				//!<�W�����v���H
		bool				m_isOnGround = true;			//!<�n�ʂ̏�ɂ���H
		CCapsuleCollider	m_collider;						//!<�R���C�_�[�B
		float				m_radius = 0.0f;				//!<�J�v�Z���R���C�_�[�̔��a�B
		float				m_height = 0.0f;				//!<�J�v�Z���R���C�_�[�̍����B
		RigidBody			m_rigidBody;					//���́B
		bool				m_isFloating = true;
		Vector3				m_rotation;
		Vector3				m_up;							// ������B
	};
}