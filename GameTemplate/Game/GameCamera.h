#pragma once
#include "SpringCamera.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsPlayer { class Player; }


	/// <summary>
	/// ゲームカメラの名前空間。
	/// </summary>
	namespace nsGameCamera
	{

		/// <summary>
		/// ゲームカメラクラス
		/// </summary>
		class GameCamera : public IGameObject
		{
		public:
			/// <summary>
			/// 開始処理。
			/// </summary>
			bool Start();
			/// <summary>
			/// 更新。
			/// </summary>
			void Update();
			/// <summary>
			/// 正面を修正。
			/// </summary>
			void FixFront();
		private:
			/// <summary>
			/// 初期化。
			/// </summary>
			void Init();
			/// <summary>
			/// プレイヤーの足が地面に着いている時の更新処理。
			/// </summary>
			void UpdateTouchingObject();
			/// <summary>
			/// プレイヤーが空中にいる時の更新処理。
			/// </summary>
			void UpdateOnAirspace();
			/// <summary>
			/// スライディング時の更新処理。
			/// </summary>
			void UpdateOnSlide();


		private:
			nsPlayer::Player*	m_player = nullptr;								// プレイヤー
			Vector3				m_position = Vector3::Zero;						// カメラの座標
			Quaternion			m_rotation = Quaternion::Identity;				// カメラの回転
			Vector3				m_target = Vector3::Zero;						// 注視点
			Vector3				m_toCameraPos = Vector3::Zero;					// 注視点から視点へのベクトル
			SpringCamera		m_springCamera;									// ばねカメラ							
			float				m_degreeX = 0.0f;								// Degree単位のX軸方向の回転量
			float				m_degreeY = 0.0f;								// Degree単位のY軸方向の回転量
			bool				m_isResetToCameraPosScale = true;				// 注視点から視点へのベクトルの倍率がリセット済みか			
		};
	}
}
