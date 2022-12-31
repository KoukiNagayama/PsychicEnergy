#pragma once
namespace nsK2EngineLow {
	class WorldRotation
	{
	public:
		WorldRotation() {}
		~WorldRotation() {}
		void AddMapModelData(ModelRender& modelRender)
		{
			m_modelArray.push_back(&modelRender);
		}
		void InitPlayerModelData(const Matrix& matrix)
		{
			m_playerWorldMatrix.push_back(&matrix);
		}
		void SetHitNormal(const Vector3& hitNormal)
		{
			m_hitNormal = hitNormal;
		}

		void RotationWorldMatrix();
	private:
		std::vector<const Matrix*>	m_playerWorldMatrix;					// プレイヤーのワールド行列
		Matrix		m_rotationMatrix = Matrix::Identity;					// 世界の回転行列
		std::vector<ModelRender*> m_modelArray;								// 回転するオブジェクトの配列
		Quaternion  m_testRot = Quaternion::Identity;						// 
		Vector3		m_hitNormal = Vector3::Zero;							// プレイヤーが触れたオブジェクトの面の法線
		Vector3		m_lastHitNormal = Vector3::Zero;						// 最後に記録されたプレイヤーが触れたオブジェクトの面の法線
		int			m_count = 0;
		//Quaternion  m_testRot2 = Quaternion::Identity;
	};

	extern WorldRotation* g_worldRotation;
}


