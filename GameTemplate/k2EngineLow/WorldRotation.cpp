#include "k2EngineLowPreCompile.h"
#include "WorldRotation.h"
#include "../Game/BackGround.h"
#include "../Game/Ring.h"

namespace nsK2EngineLow
{
	namespace nsWorldRotation
	{

		void WorldRotation::Update()
		{
			m_ringArray.clear();
		}

		void WorldRotation::RotationWorldMatrix()
		{
			// リセット中であるならば実行しない。
			if (m_isReseting) {
				return;
			}

			// 登録された法線と取得した法線が同じであるならば実行しない。
			if (m_hitNormal.x == m_lastHitNormal.x
				&& m_hitNormal.y == m_lastHitNormal.y
				&& m_hitNormal.z == m_lastHitNormal.z
				) {
				return;
			}

			// 現在取得している法線を記録する
			m_lastHitNormal = m_hitNormal;

			// クォータニオンから回転行列を作る
			m_matrixRot.SetRotation(m_hitNormal, Vector3::Up);
			m_rotationMatrix.MakeRotationFromQuaternion(m_matrixRot);


			// プレイヤーのワールド行列の逆行列
			Matrix playerWorldMatInv;
			// プレイヤーのワールド行列をもとに平行移動行列の逆行列を計算
			Matrix playerTranslationMat = Matrix::Identity;
			// 行列の回転成分と拡大成分を除去し、平行移動成分だけを抽出する。
			playerTranslationMat.v[3] = m_playerWorldMatrix->v[3];
			playerWorldMatInv.Inverse(playerTranslationMat);

			for (auto& backGround : m_backGroundArray) {
				// 回転後のワールド座標変数
				Matrix worldMatAfterRotation;
				// ワールド座標の回転は
				// マップチップのワールド行列
				// ×プレイヤーの平行移動行列の逆行列
				// ×世界の回転行列
				// ×プレイヤーの平行移動行列で求める
				ModelRender* modelMat = &backGround->GetModelRender();
				worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
				backGround->SetWorldMatrixWithLerp(worldMatAfterRotation);
			}
			// 上と同様にリングを回転させる
			for (auto& ring : m_ringArray) {
				Matrix worldMatAfterRotation;
				ModelRender* modelMat = &ring->GetModelRender();
				worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
				ring->SetWorldMatrixWithLerp(worldMatAfterRotation);
			}
			m_ringArray.clear();
			m_matrixRot.Apply(m_directionOfCurrentReference);
		}

		void WorldRotation::ResetWorldMatrix()
		{
			if (m_isReseting == false) {
				return;
			}

			// 記録されている法線情報をリセットする。
			m_hitNormal = Vector3::Zero;
			m_lastHitNormal = Vector3::Zero;

			// ここから回転
			// クォータニオンから回転行列を作る
			m_matrixRot.SetRotation(m_directionOfCurrentReference, Vector3::Up);
			m_rotationMatrix.MakeRotationFromQuaternion(m_matrixRot);

			// プレイヤーのワールド行列の逆行列
			Matrix playerWorldMatInv;
			// プレイヤーのワールド行列をもとにワールド行列の逆行列を計算
			Matrix playerTranslationMat = Matrix::Identity;
			// 行列の回転成分と拡大成分を除去し、平行移動成分だけを抽出する。
			playerTranslationMat.v[3] = m_playerWorldMatrix->v[3];
			playerWorldMatInv.Inverse(playerTranslationMat);

			for (auto& backGround : m_backGroundArray) {
				// 回転後のワールド座標変数
				Matrix worldMatAfterRotation;
				// ワールド座標の回転は
				// マップチップのワールド行列
				// ×プレイヤーの平行移動行列の逆行列
				// ×世界の回転行列
				// ×プレイヤーの平行移動行列で求める
				ModelRender* modelMat = &backGround->GetModelRender();
				worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
				backGround->SetWorldMatrixWithLerp(worldMatAfterRotation);
			}
			// 上と同様にリングを回転させる
			for (auto& ring : m_ringArray) {
				Matrix worldMatAfterRotation;
				ModelRender* modelMat = &ring->GetModelRender();
				worldMatAfterRotation.Multiply(modelMat->GetWorldMatrix(), playerWorldMatInv);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, m_rotationMatrix);
				worldMatAfterRotation.Multiply(worldMatAfterRotation, playerTranslationMat);
				ring->SetWorldMatrixWithLerp(worldMatAfterRotation);
			}
			m_ringArray.clear();
			// リセット中であるとフラグを設定。
			m_isReseting = false;
			m_directionOfCurrentReference = Vector3::Up;

		}

		void WorldRotation::DiscardRegisteredData()
		{
			// 各登録情報をリセットする。
			m_playerWorldMatrix = nullptr;
			m_backGroundArray.clear();
			m_directionOfCurrentReference = Vector3::Up;
			m_hitNormal = Vector3::Zero;
			m_lastHitNormal = Vector3::Zero;
			m_rotationMatrix = Matrix::Identity;
		}

		WorldRotation* g_worldRotation = nullptr;			// 世界の回転クラスのグローバル変数。
	}
}