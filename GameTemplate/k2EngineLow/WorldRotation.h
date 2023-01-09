#pragma once
class BackGround;
class Ring;
namespace nsK2EngineLow {
	class WorldRotation
	{
	public:
		WorldRotation() {}
		~WorldRotation() {}
		void AddBackGround(BackGround* modelRender)
		{
			m_backGroundArray.push_back(modelRender);
		}
		void AddRing(Ring* modelRender)
		{
			m_ringArray.push_back(modelRender);
		}
		void InitPlayerModelData(const Matrix& matrix)
		{
			m_playerWorldMatrix.push_back(&matrix);
		}
		void SetGroundWorldMatrix(const Matrix& matrix)
		{
			m_groundWorldMatrix = &matrix;
		}
		void SetHitNormal(const Vector3& hitNormal)
		{
			if (m_isGetNormal) {
				m_hitNormal = hitNormal;
				m_isGetNormal = false;
			}
		}
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
		//std::vector<ModelRender*> m_modelArray;								// 回転するオブジェクトの配列
		std::vector<BackGround*> m_backGroundArray;
		std::vector<Ring*> m_ringArray;
		
		Quaternion  m_matrixRot = Quaternion::Identity;						// 
		Vector3		m_hitNormal = Vector3::Zero;							// プレイヤーが触れたオブジェクトの面の法線
		Vector3		m_lastHitNormal = Vector3::Zero;						// 最後に記録されたプレイヤーが触れたオブジェクトの面の法線
		int			m_count = 0;
		bool		m_isGetNormal = false;
		const Matrix*				m_groundWorldMatrix;
		Vector3		m_groundNormal = Vector3::Up;
	};

	extern WorldRotation* g_worldRotation;
}


