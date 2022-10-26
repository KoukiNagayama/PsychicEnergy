/*!
 * @brief	キャラクタコントローラー。
 */

#pragma once

#include "CapsuleCollider.h"
#include "RigidBody.h"

namespace nsK2EngineLow {
	/*!
	* @brief	キャラクタコントローラー。
	*@details
	* キャラクターと背景の衝突解決を行うことができます。\n
	* 下記のサンプルプログラムを参考にしてください。\n
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
		/// 初期化。
		/// </summary>
		/// <param name="radius">カプセルコライダーの半径。</param>
		/// <param name="height">カプセルコライダーの高さ。</param>
		/// <param name="position">初期位置。</param>
		/// <param name="rotation">回転。オイラー角</param>
		void Init(float radius, float height, const Vector3& position, Vector3& rotation);
		/// <summary>
		/// 実行。
		/// </summary>
		/// <param name="moveSpeed">移動速度。</param>
		/// <param name="deltaTime">経過時間。単位は秒。</param>
		/// <param name="rotation">回転。オイラー角。</param>
		/// <returns></returns>
		const Vector3& Execute(Vector3& moveSpeed, float deltaTime, Vector3& rotation);
		const Vector3& Float(Vector3& moveSpeed, float deltaTime, Vector3& rotation);
		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns>座標。</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ジャンプ中か判定。
		/// </summary>
		/// <returns>ジャンプ中ならtrue。</returns>
		bool IsJump() const
		{
			return m_isJump;
		}
		/// <summary>
		/// 地面上にいるか判定。
		/// </summary>
		/// <returns>地面上にいるならtrue。</returns>
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/// <summary>
		/// コライダーを取得。
		/// </summary>
		CCapsuleCollider* GetCollider()
		{
			return &m_collider;
		}
		/// <summary>
		/// 剛体を取得。
		/// </summary>
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}
		/// <summary>
		/// 剛体を物理エンジンから削除。
		/// </summary>
		void RemoveRigidBoby();
	private:
		bool				m_isInited = false;				//!<初期化済み？
		Vector3 			m_position;						//!<座標。
		bool 				m_isJump = false;				//!<ジャンプ中？
		bool				m_isOnGround = true;			//!<地面の上にいる？
		CCapsuleCollider	m_collider;						//!<コライダー。
		float				m_radius = 0.0f;				//!<カプセルコライダーの半径。
		float				m_height = 0.0f;				//!<カプセルコライダーの高さ。
		RigidBody			m_rigidBody;					//剛体。
		bool				m_isFloating = true;
		Vector3				m_rotation;
		Vector3				m_up;							// 上方向。
	};
}