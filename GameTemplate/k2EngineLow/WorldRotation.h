#pragma once

namespace nsPsychicEnergy
{
	// 前方宣言。
	namespace nsBackGround { class BackGround; }
	namespace nsRing { class Ring; }
}

namespace nsK2EngineLow {

	/// <summary>
	/// 世界の回転の名前空間。
	/// </summary>
	namespace nsWorldRotation
	{

		/// <summary>
		/// 世界の回転クラス。
		/// </summary>
		class WorldRotation
		{
		public:
			/// <summary>
			/// コンストラクタ。
			/// </summary>
			WorldRotation() {}
			/// <summary>
			///	デストラクタ。
			/// </summary>
			~WorldRotation() {}
			/// <summary>
			/// 更新。
			/// </summary>
			void Update();
			/// <summary>
			/// 登録データを破棄する。
			/// </summary>
			void DiscardRegisteredData();
			/// <summary>
			/// 背景オブジェクトを回転する対象に追加
			/// </summary>
			/// <param name="modelRender"></param>
			void AddBackGround(nsPsychicEnergy::nsBackGround::BackGround* modelRender)
			{
				m_backGroundArray.push_back(modelRender);
			}
			/// <summary>
			/// リングを回転する対象に追加
			/// </summary>
			/// <param name="modelRender"></param>
			void AddRing(nsPsychicEnergy::nsRing::Ring* modelRender)
			{
				m_ringArray.push_back(modelRender);
			}
			/// <summary>
			/// プレイヤーのワールド行列を登録
			/// </summary>
			/// <param name="matrix"></param>
			void RegisterPlayerWorldMatrix(const Matrix& matrix)
			{
				m_playerWorldMatrix = nullptr;
				m_playerWorldMatrix = &matrix;
			}
			/// <summary>
			/// 衝突した法線を設定。
			/// </summary>
			/// <param name="hitNormal"></param>
			void SetHitNormal(const Vector3& hitNormal)
			{
				if (m_isGetNormal) {
					m_hitNormal = hitNormal;
					m_isGetNormal = false;
				}
			}
			/// <summary>
			/// 法線を取得するかどうかを設定。
			/// </summary>
			/// <param name="isGetNormal"></param>
			void SetIsGetNormal(const bool& isGetNormal)
			{
				m_isGetNormal = isGetNormal;
			}
			/// <summary>
			/// ワールド行列の回転
			/// </summary>
			void RotationWorldMatrix();
			/// <summary>
			/// ワールド行列のリセット
			/// </summary>
			void ResetWorldMatrix();
			/// <summary>
			/// 現在リセット中か設定。
			/// </summary>
			/// <param name="isReseting"></param>
			void SetIsReseting(const bool& isReseting)
			{
				m_isReseting = isReseting;
			}

		private:
			const Matrix*											m_playerWorldMatrix;							// プレイヤーのワールド行列
			Matrix													m_rotationMatrix = Matrix::Identity;			// 世界の回転行列
			std::vector<nsPsychicEnergy::nsBackGround::BackGround*>	m_backGroundArray;								// 背景オブジェクトの配列
			std::vector <nsPsychicEnergy::nsRing::Ring* >			m_ringArray;									// リングの配列
			Quaternion												m_matrixRot = Quaternion::Identity;				// 回転行列を作るためのクォータニオン
			Vector3													m_hitNormal = Vector3::Zero;					// プレイヤーが触れたオブジェクトの面の法線
			Vector3													m_lastHitNormal = Vector3::Zero;				// 最後に記録されたプレイヤーが触れたオブジェクトの面の法線
			bool													m_isGetNormal = false;							// 衝突した法線を取得するか
			Vector3													m_directionOfCurrentReference = Vector3::Up;	// 現在の基準となる方向(リセット時に使用)
			bool													m_isReseting = false;							// リセット中か
		};

		extern WorldRotation* g_worldRotation;		// 世界の回転クラス。

	}
}


