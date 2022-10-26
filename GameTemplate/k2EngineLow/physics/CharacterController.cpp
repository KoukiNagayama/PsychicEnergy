/*!
* @brief	キャラクタのコリジョンコントロール。
*/

#include "k2EngineLowPreCompile.h"
#include "CharacterController.h"

namespace nsK2EngineLow {
	namespace {
		void Vector3CopyFrom(Vector3& vDst, btVector3& vSrc)
		{
			vDst.x = vSrc.x();
			vDst.y = vSrc.y();
			vDst.z = vSrc.z();
		}
		//衝突したときに呼ばれる関数オブジェクト(地面用)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;									//衝突フラグ。
			Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);		//衝突点。
			Vector3 startPos;									//レイの始点。
			Vector3 hitNormal;									//衝突点の法線。
			btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
			float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
			Vector3 up;
																//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
				//上方向と法線のなす角度を求める。
				float cos = up.Dot(hitNormalTmp);
				float angle = fabsf(acosf(cos));
				if (angle < Math::PI * 0.3f		//地面の傾斜が54度より小さいので地面とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
					) {
					//衝突している。
					isHit = true;
					Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
					//衝突点の距離を求める。。
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					if (dist > distTmp) {
						//この衝突点の方が近いので、最近傍の衝突点を更新する。
						hitPos = hitPosTmp;
						hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
						dist = distTmp;
					}
				}
				return 0.0f;
			}
		};
		//衝突したときに呼ばれる関数オブジェクト(壁用)
		struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;						//衝突フラグ。
			Vector3 hitPos;							//衝突点。
			Vector3 startPos;						//レイの始点。
			float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
			Vector3 hitNormal;						//衝突点の法線。
			btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
			Vector3 up;
													//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject == me
					|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
					) {
					//自分に衝突した。or 地面に衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				Vector3 hitNormalTmp;
				Vector3CopyFrom(hitNormalTmp, convexResult.m_hitNormalLocal);

				//上方向と衝突点の法線のなす角度を求める。

				float cos = up.Dot(hitNormalTmp);
				float angle = fabsf(acosf(cos));
				if (angle >= Math::PI * 0.3f		//地面の傾斜が54度以上なので壁とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
					) {
					isHit = true;
					Vector3 hitPosTmp;
					Vector3CopyFrom(hitPosTmp, convexResult.m_hitPointLocal);

					//交点との距離を調べる。
					Vector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					//vDist.y = 0.0f;
					float distTmp = vDist.Length();
					if (distTmp < dist) {
						//この衝突点の方が近いので、最近傍の衝突点を更新する。
						hitPos = hitPosTmp;
						dist = distTmp;
						hitNormal = hitNormalTmp;
					}
				}
				return 0.0f;
			}
		};
	}


	void CharacterController::Init(float radius, float height, const Vector3& position, Vector3& rotation)
	{
		m_position = position;
		//コリジョン作成。
		m_radius = radius;
		m_height = height;
		m_collider.Init(radius, height);

		//剛体を初期化。
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Init(rbInfo);
		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(position.x, position.y + m_height * 0.5f + m_radius, position.z));
		
		trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		m_isInited = true;
	}
	const Vector3& CharacterController::Execute(
		Vector3& moveSpeed, 
		float deltaTime, 
		Vector3& rotation
	)
	{
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		// モデルの上方向
		m_up = Vector3::Up;
		// 剛体の基点までのベクトルを更新。
		Quaternion qRot = {
			trans.getRotation().getX(),
			trans.getRotation().getY() ,
			trans.getRotation().getZ() ,
			trans.getRotation().getW()
		};
		qRot.Apply(m_up);
		m_up.Normalize();

		Vector3 nextPosition = m_position;
		// 速度からこのフレームでの移動量を求める。オイラー積分。
		Vector3 addPos = moveSpeed;
		addPos.Scale(deltaTime);
		nextPosition.Add(addPos);
		
		Vector3 originalDir;
		originalDir = addPos;
		originalDir.Normalize();

		{
			int loopCount = 0;
			while (true) {
				// 現在の座標から次の移動先へ向かうベクトルを求める。
				Vector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				// 移動量が小さすぎる場合は調べる必要がない。
				if (addPos.LengthSq() < FLT_EPSILON * FLT_EPSILON) {
					break;
				}

				// 下方向
				Vector3 v1 = (m_up * -1.0f) * addPos.Dot(m_up * -1.0f);
				// 横方向ベクトル
				Vector3 v2 = addPos - v1;

				// 値を別途使用するため上方向の変数をもう一つ使用。
				Vector3 up = m_up;
				up.Scale(m_height * 0.5f + m_radius + m_height * 0.1f);
				//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
				Vector3 posTmp = m_position + up;

				// レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));

				// 横方向の座標
				Vector3 widePos = posTmp + v2;
				//終点は次の移動。
				end.setOrigin(btVector3(widePos.x, widePos.y, widePos.z));
				
				SweepResultWall callback;
				callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				callback.up = m_up;
				//衝突検出。
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				
				if (callback.isHit) {
					//当たった。
					//壁。
					Vector3 vT0, vT1;
					//移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosition.x, nextPosition.y, nextPosition.z);
					vT1.Set(callback.hitPos.x, callback.hitPos.y, callback.hitPos.z);
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi.Subtract(vT0, vT1);
					
					Vector3 v2n = v2;
					v2n.Normalize();
					float dot = vMerikomi.Dot(v2n);
					vMerikomi = v2n * dot;
					
					//XZ平面での衝突した壁の法線を求める。。
					Vector3 hitNormal = callback.hitNormal;
					hitNormal.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormal.Dot(vMerikomi);
					//押し戻し返すベクトルを求める。
					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					Vector3 vOffset;
					vOffset = hitNormal;
					vOffset.Scale(-fT0 + m_radius);
					
					vOffset = v2 * vOffset.Dot(v2);
					nextPosition.Add(vOffset);

					Vector3 currentDir;
					currentDir.Subtract(nextPosition, m_position);
					currentDir.Normalize();
					if (currentDir.Dot(originalDir) < 0.0f) {
						//角に入った時のキャラクタの振動を防止するために、
						//移動先が逆向きになったら移動をキャンセルする。
						nextPosition = m_position;
						break;
					}
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
				loopCount++;
				if (loopCount == 5) {
					break;
				}
			}
		}
		m_position = nextPosition;
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);
			
			m_position = nextPosition;	//移動の仮確定。
										//レイを作成する。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			Vector3 up = m_up;
			up.Scale(m_height * 0.5f + m_radius);
			Vector3 posTmp = m_position + up;
			start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
			//終点は地面上にいない場合は1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			Vector3 endPos;
			Vector3CopyFrom(endPos, start.getOrigin());


			
			if (m_isOnGround == false) {
				if ((up * addPos.Dot(up)).LengthSq() > 0.0f) {
					//ジャンプ中とかで上昇中。
					//上昇中でも横方向に移動した結果めり込んでいる可能性があるので下を調べる。
					/*endPos.y -= addPos.y * 0.01f;*/
					endPos -= up * addPos.Dot(up) * 0.01f;
				}
				else {
					//落下している場合はそのまま下を調べる。
					//endPos.y += addPos.y;
					endPos += up * addPos.Dot(up);
				}
			}
			else {
				//地面上にいない場合は1m下を見る。
				//endPos.y -= 100.0f;
				endPos -= up * 100.0f;
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			SweepResultGround callback;
			callback.me = m_rigidBody.GetBody();
			callback.up = m_up;
			Vector3CopyFrom(callback.startPos, start.getOrigin());
			
			//衝突検出。
			if ((endPos - callback.startPos).LengthSq() > FLT_EPSILON * FLT_EPSILON) {
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				if (callback.isHit) {
					//当たった。
					moveSpeed.y = 0.0f;
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y;
				}
				else {
					//地面上にいない。
					m_isOnGround = false;
			
				}
			}
		}
		//移動確定。
		m_position = nextPosition;

		// 剛体を回転させる。
		trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));


		// 剛体の位置を更新。
		m_up.Scale(m_height * 0.5f + m_radius);
		auto finalPos = m_position + m_up;
		trans.setOrigin(btVector3(finalPos.x, finalPos.y, finalPos.z));
		
		return m_position;
	}

	const Vector3& CharacterController::Float(Vector3& moveSpeed, float deltaTime, Vector3& rotation)
	{
		Vector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。オイラー積分。
		Vector3 addPos = moveSpeed;
		addPos.Scale(deltaTime);
		nextPosition.Add(addPos);
		//移動確定。
		m_position = nextPosition;
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();

		// 剛体を回転させる。
		trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		// モデルの上方向
		Vector3 up = Vector3::Up;
		// 剛体の基点までのベクトルを更新。
		Quaternion qRot = {
			trans.getRotation().getX(),
			trans.getRotation().getY() ,
			trans.getRotation().getZ() ,
			trans.getRotation().getW()
		};
		qRot.Apply(up);
		up.Normalize();
		up.Scale(m_height * 0.5f + m_radius);
		// 剛体の位置を更新。
		auto finalPos = m_position + up;
		trans.setOrigin(btVector3(finalPos.x, finalPos.y, finalPos.z));
		return m_position;
	}


	void CharacterController::RemoveRigidBoby()
	{
		PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
	}
}




//const Vector3& CharacterController::Execute(Vector3& moveSpeed, float deltaTime, Vector3& rotation)
//	{
//		if (moveSpeed.y > 0.0f) {
//			//吹っ飛び中にする。
//			m_isJump = true;
//			m_isOnGround = false;
//		}
//		//次の移動先となる座標を計算する。
//		Vector3 nextPosition = m_position;
//		//速度からこのフレームでの移動量を求める。オイラー積分。
//		Vector3 addPos = moveSpeed;
//		addPos.Scale(deltaTime);
//		nextPosition.Add(addPos);
//		Vector3 originalXZDir = addPos;
//		originalXZDir.y = 0.0f;
//		originalXZDir.Normalize();
//		//XZ平面での衝突検出と衝突解決を行う。
//		{
//			int loopCount = 0;
//			while (true) {
//				//現在の座標から次の移動先へ向かうベクトルを求める。
//				Vector3 addPos;
//				addPos.Subtract(nextPosition, m_position);
//				Vector3 addPosXZ = addPos;
//				addPosXZ.y = 0.0f;
//				if (addPosXZ.Length() < FLT_EPSILON) {
//					//XZ平面で動きがないので調べる必要なし。
//					//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
//					//とても小さい値のことです。
//					break;
//				}
//				//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
//				Vector3 posTmp = m_position;
//				posTmp.y += m_height * 0.5f + m_radius + m_height * 0.1f;
//				//レイを作成。
//				btTransform start, end;
//				start.setIdentity();
//				end.setIdentity();
//				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
//				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
//				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
//				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));
//
//				SweepResultWall callback;
//				callback.me = m_rigidBody.GetBody();
//				callback.startPos = posTmp;
//				//衝突検出。
//				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
//
//				if (callback.isHit) {
//					//当たった。
//					//壁。
//					Vector3 vT0, vT1;
//					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
//					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
//					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
//					//めり込みが発生している移動ベクトルを求める。
//					Vector3 vMerikomi;
//					vMerikomi.Subtract(vT0, vT1);
//					//XZ平面での衝突した壁の法線を求める。。
//					Vector3 hitNormalXZ = callback.hitNormal;
//					hitNormalXZ.y = 0.0f;
//					hitNormalXZ.Normalize();
//					//めり込みベクトルを壁の法線に射影する。
//					float fT0 = hitNormalXZ.Dot(vMerikomi);
//					//押し戻し返すベクトルを求める。
//					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
//					Vector3 vOffset;
//					vOffset = hitNormalXZ;
//					vOffset.Scale(-fT0 + m_radius);
//					nextPosition.Add(vOffset);
//
//					Vector3 currentDir;
//					currentDir.Subtract(nextPosition, m_position);
//					currentDir.y = 0.0f;
//					currentDir.Normalize();
//					if (currentDir.Dot(originalXZDir) < 0.0f) {
//						//角に入った時のキャラクタの振動を防止するために、
//						//移動先が逆向きになったら移動をキャンセルする。
//						nextPosition.x = m_position.x;
//						nextPosition.z = m_position.z;
//						break;
//					}
//				}
//				else {
//					//どことも当たらないので終わり。
//					break;
//				}
//				loopCount++;
//				if (loopCount == 5) {
//					break;
//				}
//			}
//		}
//		//XZの移動は確定。
//		m_position.x = nextPosition.x;
//		m_position.z = nextPosition.z;
//		//下方向を調べる。
//		{
//			Vector3 addPos;
//			addPos.Subtract(nextPosition, m_position);
//
//			m_position = nextPosition;	//移動の仮確定。
//										//レイを作成する。
//			btTransform start, end;
//			start.setIdentity();
//			end.setIdentity();
//			//始点はカプセルコライダーの中心。
//			start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
//			//終点は地面上にいない場合は1m下を見る。
//			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
//			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
//			Vector3 endPos;
//			Vector3CopyFrom(endPos, start.getOrigin());
//
//			if (m_isOnGround == false) {
//				if (addPos.y > 0.0f) {
//					//ジャンプ中とかで上昇中。
//					//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
//					endPos.y -= addPos.y * 0.01f;
//				}
//				else {
//					//落下している場合はそのまま下を調べる。
//					endPos.y += addPos.y;
//				}
//			}
//			else {
//				//地面上にいない場合は1m下を見る。
//				endPos.y -= 100.0f;
//			}
//			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
//			SweepResultGround callback;
//			callback.me = m_rigidBody.GetBody();
//			Vector3CopyFrom(callback.startPos, start.getOrigin());
//
//			//衝突検出。
//			if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
//				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
//				if (callback.isHit) {
//					//当たった。
//					moveSpeed.y = 0.0f;
//					m_isJump = false;
//					m_isOnGround = true;
//					nextPosition.y = callback.hitPos.y;
//				}
//				else {
//					//地面上にいない。
//					m_isOnGround = false;
//
//				}
//			}
//		}
//		//移動確定。
//		m_position = nextPosition;
//		btRigidBody* btBody = m_rigidBody.GetBody();
//		//剛体を動かす。
//		btBody->setActivationState(DISABLE_DEACTIVATION);
//		btTransform& trans = btBody->getWorldTransform();
//
//		// 剛体を回転させる。
//		trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
//		// モデルの上方向
//		Vector3 up = Vector3::Up;
//		// 剛体の基点までのベクトルを更新。
//		Quaternion qRot = {
//			trans.getRotation().getX(),
//			trans.getRotation().getY() ,
//			trans.getRotation().getZ() ,
//			trans.getRotation().getW()
//		};
//		qRot.Apply(up);
//		up.Normalize();
//		up.Scale(m_height * 0.5f + m_radius);
//		// 剛体の位置を更新。
//		auto finalPos = m_position + up;
//		trans.setOrigin(btVector3(finalPos.x, finalPos.y , finalPos.z));
//		return m_position;
//	}

