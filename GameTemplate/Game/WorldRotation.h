#pragma once
class WorldRotation
{
public:
	WorldRotation() {}
	~WorldRotation() {}
	void AddMapModelData(ModelRender& modelRender)
	{
		m_modelArray.push_back(&modelRender);
	}
	void InitPlayerModelData(Matrix& matrix)
	{
		m_playerWorldMatrix.push_back(&matrix);
	}

	void RotationWorldMatrix();
private:
	std::vector<Matrix*>	m_playerWorldMatrix;					// プレイヤーのワールド行列
	Matrix		m_rotationMatrix;						// 世界の回転行列
	bool		m_isGetPlayerWorldMatrix = false;		// プレイヤーのワールド行列は取得済みか？
	std::vector<ModelRender*> m_modelArray;
	Quaternion  m_testRot = Quaternion::Identity;
};

extern WorldRotation* g_worldRotation;

