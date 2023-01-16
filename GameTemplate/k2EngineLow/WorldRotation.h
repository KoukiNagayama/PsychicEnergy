#pragma once
class BackGround;
class Ring;
namespace nsK2EngineLow {
	class WorldRotation
	{
	public:
		WorldRotation() {}
		~WorldRotation() {}
		/// <summary>
		/// 背景オブジェクトを回転する対象に追加
		/// </summary>
		/// <param name="modelRender"></param>
		void AddBackGround(BackGround* modelRender)
		{
			m_backGroundArray.push_back(modelRender);
		}
		/// <summary>
		/// リングを回転する対象に追加
		/// </summary>
		/// <param name="modelRender"></param>
		void AddRing(Ring* modelRender)
		{
			m_ringArray.push_back(modelRender);
		}
		/// <summary>
		/// プレイヤーのワールド行列を登録
		/// </summary>
		/// <param name="matrix"></param>
		void InitPlayerWorldMatrix(const Matrix& matrix)
		{
			m_playerWorldMatrix.push_back(&matrix);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="matrix"></param>
		void SetGroundWorldMatrix(const Matrix& matrix)
		{
			m_groundWorldMatrix = &matrix;
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

	private:
		std::vector<const Matrix*>	m_playerWorldMatrix;					// プレイヤーのワールド行列
		Matrix		m_rotationMatrix = Matrix::Identity;					// 世界の回転行列
		std::vector<BackGround*> m_backGroundArray;							// 背景オブジェクトの配列
		std::vector<Ring*> m_ringArray;										// リングの配列
		Quaternion  m_matrixRot = Quaternion::Identity;						// 
		Vector3		m_hitNormal = Vector3::Zero;							// プレイヤーが触れたオブジェクトの面の法線
		Vector3		m_lastHitNormal = Vector3::Zero;						// 最後に記録されたプレイヤーが触れたオブジェクトの面の法線
		bool		m_isGetNormal = false;									// 衝突した法線を取得するか
		const Matrix*				m_groundWorldMatrix;
		Vector3		m_groundNormal = Vector3::Up;		
	};

	extern WorldRotation* g_worldRotation;
}


