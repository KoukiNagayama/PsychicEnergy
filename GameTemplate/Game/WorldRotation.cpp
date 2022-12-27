#include "stdafx.h"
#include "WorldRotation.h"

void WorldRotation::RotationWorldMatrix()
{
	m_testRot.AddRotationDegY(5.0f);
	m_rotationMatrix.MakeRotationFromQuaternion(m_testRot);
	// プレイヤーのワールド行列の逆行列
	Matrix playerWorldMatInv;
	// プレイヤーのワールド行列をもとにワールド行列の逆行列を計算
	playerWorldMatInv.Inverse(*m_playerWorldMatrix[0]);

	for (auto& modelMat : m_modelArray) {
		// 回転後のワールド座標変数
		Matrix afterRotMatrix;
		// ワールド座標の回転は
		// マップチップのワールド行列(仮)
		// ×プレイヤーのワールド行列の逆行列
		// ×世界の回転行列
		// ×プレイヤーのワールド行列で求める
		afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
		afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
		afterRotMatrix.Multiply(afterRotMatrix, *m_playerWorldMatrix[0]);

		modelMat->SetWorldMatrix(afterRotMatrix);
	}
}

WorldRotation* g_worldRotation = nullptr;
