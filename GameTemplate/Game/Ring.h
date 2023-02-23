#pragma once
#include "sound/SoundSource.h"
/// <summary>
/// PsychicEnergyの名前空間。
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsGame { class Game; }
	namespace nsPlayer { class Player; }

	/// <summary>
	/// リングの名前空間。
	/// </summary>
	namespace nsRing
	{
		/// <summary>
		/// リングクラス。
		/// </summary>
		class Ring : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			Ring() {}
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~Ring();
			/// <summary>
			/// 開始処理。
			/// インスタンス生成時に初期化を行う。trueを返したときに呼ばれなくなる。
			/// </summary>
			bool Start();
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update();
			/// <summary>
			/// 描画処理。
			/// </summary>
			/// <param name="rc"></param>
			void Render(RenderContext& rc);
			/// <summary>
			/// 座標を設定。
			/// </summary>
			/// <param name="position">設定する座標。</param>
			void SetPosition(Vector3& position)
			{
				m_position = position;
			}
			/// <summary>
			/// 回転を設定。
			/// </summary>
			/// <param name="rotation">設定する回転値。</param>
			void SetRotation(Quaternion& rotation)
			{
				m_rotation = rotation;
			}
			/// <summary>
			/// 拡大率を設定。
			/// </summary>
			/// <param name="scale">設定する拡大率。</param>
			void SetScale(Vector3& scale)
			{
				m_scale = scale;
			}
			/// <summary>
			/// 座標を取得。
			/// </summary>
			/// <returns>座標。</returns>
			const Vector3& GetPosition() const
			{
				return m_position;
			}
			/// <summary>
			/// モデルレンダーを取得。
			/// </summary>
			/// <returns>使用しているモデルレンダー。</returns>
			ModelRender& GetModelRender()
			{
				return m_modelRender;
			}
			/// <summary>
			/// 線形補間を行うための目標となるワールド行列を設定。
			/// </summary>
			/// <param name="worldMat">目標となるワールド行列。</param>
			inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
			{
				m_prevMatrix = m_modelRender.GetWorldMatrix();
				m_nextMatrix = worldMat;
				m_rotateTimer = 0.0f;
			}
			/// <summary>
			/// リングを取得したことを通知
			/// </summary>
			void NotifyGet();
		private:
			/// <summary>
			/// 回転
			/// </summary>
			void Rotation();
			/// <summary>
			/// 当たり判定
			/// </summary>
			void Collision();
			/// <summary>
			/// プレイヤーとの距離を計算
			/// </summary>
			float CalcDistanceToPlayer();
		private:
			nsGame::Game*			m_game = nullptr;					// ゲームクラス
			ModelRender				m_modelRender;						// モデルレンダー
			Vector3					m_position;							// 座標
			Vector3					m_scale;							// 拡大率
			Quaternion				m_rotation;							// 回転
			nsPlayer::Player*		m_player = nullptr;					// プレイヤー
			Matrix					m_prevMatrix = Matrix::Identity;	// 事前の行列
			Matrix					m_nextMatrix = Matrix::Identity;	// 回転後の目標となる行列
			float					m_rotateTimer = 1.0f;				// 回転が完了するまでのタイマー
			SoundSource*			m_acquisitionSound = nullptr;		// 取得音
		};

	}
}

