#include "k2EngineLowPreCompile.h"
#include "WorldRotation.h"

namespace nsK2EngineLow
{
	void WorldRotation::RotationWorldMatrix()
	{
		//m_testRot.AddRotationDegZ(1.0f);
		if (m_hitNormal.x == m_lastHitNormal.x
			&& m_hitNormal.y == m_lastHitNormal.y
			&& m_hitNormal.z == m_lastHitNormal.z
			) {
			return;
		}
		if (m_count != 0) {
			return;
		}
		m_count++;
		// 現在取得している法線を記録する
		m_lastHitNormal = m_hitNormal;
		m_hitNormal.Normalize();
		// クォータニオンから回転行列を作る
		m_testRot.SetRotation(m_hitNormal, Vector3::Up);
		m_rotationMatrix.MakeRotationFromQuaternion(m_testRot);
		// プレイヤーのワールド行列の逆行列
		Matrix playerWorldMatInv;
		// プレイヤーのワールド行列をもとにワールド行列の逆行列を計算
		//playerWorldMatInv.Inverse(*m_playerWorldMatrix[0]);
		Matrix playerTranslationMat = Matrix::Identity;
		//playerTranslationMat.v[0] = m_playerWorldMatrix[0]->v[0];
		//playerTranslationMat.v[1] = m_playerWorldMatrix[0]->v[1];
		//playerTranslationMat.v[2] = m_playerWorldMatrix[0]->v[2];
		playerTranslationMat.v[3] = m_playerWorldMatrix[0]->v[3];
		playerWorldMatInv.Inverse(playerTranslationMat);

		for (auto& modelMat : m_modelArray) {
			// 回転後のワールド座標変数
			Matrix afterRotMatrix;
			// ワールド座標の回転は
			// マップチップのワールド行列
			// ×プレイヤーのワールド行列の逆行列
			// ×世界の回転行列
			// ×プレイヤーのワールド行列で求める
			afterRotMatrix.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
			afterRotMatrix.Multiply(afterRotMatrix, m_rotationMatrix);
			afterRotMatrix.Multiply(afterRotMatrix, playerTranslationMat);

			modelMat->SetWorldMatrix(afterRotMatrix);
		}
	}

	WorldRotation* g_worldRotation = nullptr;
}

