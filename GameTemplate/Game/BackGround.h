#pragma once


/// <summary>
/// PsychicEnergyの名前空間
/// </summary>
namespace nsPsychicEnergy
{
	// 前方宣言
	namespace nsPlayer { class Player; }	// プレイヤー。


	/// <summary>
	/// 背景オブジェクトの名前空間
	/// </summary>
	namespace nsBackGround
	{
		/// <summary>
		/// 背景モデルクラス
		/// </summary>
		class BackGround : public IGameObject
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			BackGround() {}
			/// <summary>
			///	デストラクタ。
			/// </summary>
			~BackGround() {}
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
			/// モデルのタイプを設定。
			/// </summary>
			/// <param name="num">設定するタイプの番号。</param>
			void SetTypeNum(int num)
			{
				m_typeNum = num;
			}
			/// <summary>
			/// モデルレンダーを取得。
			/// </summary>
			/// <returns></returns>
			ModelRender& GetModelRender()
			{
				return m_modelRender;
			}
			/// <summary>
			/// ワールド行列を設定し、補間するため情報を初期化。
			/// </summary>
			/// <param name="worldMat">回転後のワールド行列。</param>
			inline void SetWorldMatrixWithLerp(const Matrix& worldMat)
			{
				m_prevMatrix = m_modelRender.GetWorldMatrix();
				m_nextMatrix = worldMat;
				m_rotateTimer = 0.0f;
			}
		private:
			ModelRender				m_modelRender;								// モデルレンダー
			Vector3					m_position = Vector3::Zero;					// 座標
			Vector3					m_scale = Vector3::One;						// 拡大率
			Quaternion				m_rotation = Quaternion::Identity;			// 回転
			int						m_typeNum = 10;								// モデルのタイプ
			PhysicsStaticObject		m_physicsStaticObject;						// 静的物理オブジェクト
			Matrix					m_worldMatrix = Matrix::Identity;			// ワールド行列
			nsPlayer::Player*		m_player = nullptr;							// プレイヤークラス
			Matrix					m_prevMatrix = Matrix::Identity;			// 直前のワールド行列
			Matrix					m_nextMatrix = Matrix::Identity;			// 次のワールド行列
			float					m_rotateTimer = 1.0f;						// 回転する時間
		};


	}
}
