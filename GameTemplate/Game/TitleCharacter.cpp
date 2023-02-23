#include "stdafx.h"
#include "TitleCharacter.h"
#include "Title.h"

namespace
{
	const float INCREASE_Y_WHEN_FLOATING = 30.0f;	// 浮遊時のY座標の増加量
	const float DISTANCE_TRAVEL_PER_FRAME = 20.0f;	// フレームごとに移動する時間
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// アニメーションの補間時間
	const float TIME_TO_COMPLETE_FLOAT = 0.3f;		// 浮遊完了とする時間

}

namespace nsPsychicEnergy
{

	namespace nsTitle
	{
		TitleCharacter::~TitleCharacter()
		{
			// レンダリングエンジンの浮遊設定を解除する。
			g_renderingEngine->SetIsFloating(false);
		}

		bool TitleCharacter::Start()
		{
			// タイトルクラスのインスタンスを検索
			m_title = FindGO<Title>("title");

			// アニメーションクリップをロードする。
			// 待機アニメーション。
			m_animationClips[enAnimationClip_Idle].Load("Assets/animData/WD/idle_2.tka");
			m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
			// 空中での待機アニメーション。
			m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_4.tka");
			m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);
			// 空中での落下(移動)アニメーション。
			m_animationClips[enAnimationClip_FallAir].Load("Assets/animData/WD/fall_air.tka");
			m_animationClips[enAnimationClip_FallAir].SetLoopFlag(true);

			// モデルを初期化。
			m_modelRender.Init(
				"Assets/modelData/WD/WD.tkm",
				m_animationClips,
				enAnimationClip_Num,
				enModelUpAxisZ,
				true,
				true,
				true,
				true
			);

			// モデルの座標、回転、拡大率を設定。
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);

			// モデルの更新。
			m_modelRender.Update();

			return true;
		}

		void TitleCharacter::Update()
		{
			// ステートの変更が完了しておらず、Bが押されているならば。
			if (!m_isCompletedStateChange && m_title->IsPushedB()) {
				
				// 浮遊状態であると設定。
				m_modelRender.SetIsFloating(true);
				g_renderingEngine->SetIsFloating(true);

				// y座標を上げて浮遊させる。
				m_position.y += INCREASE_Y_WHEN_FLOATING;
				m_currentAnimationClip = enAnimationClip_IdleAir;
				m_isFloating = true;

				// ステートの変更が完了したとする。
				m_isCompletedStateChange = true;
			}

			// 浮遊しているならば。
			if (m_isFloating) {

				// 浮遊してからの時間を加算する。
				m_floatingCount += g_gameTime->GetFrameDeltaTime();

				// 一定値を超えたならば。
				if (m_floatingCount >= TIME_TO_COMPLETE_FLOAT) {
					// 手前に浮遊移動させる。
					m_currentAnimationClip = enAnimationClip_FallAir;
					m_position.z += DISTANCE_TRAVEL_PER_FRAME;
				}
			}

			// アニメーションを再生する。
			PlayAnimation();

			// モデルの座標を更新。
			m_modelRender.SetPosition(m_position);
			m_modelRender.Update();
		}

		void TitleCharacter::PlayAnimation()
		{
			// アニメーションを再生
			m_modelRender.PlayAnimation(m_currentAnimationClip, ANIMATION_INTERPORATE_TIME);
		}

		void TitleCharacter::Render(RenderContext& rc)
		{
			// 描画
			m_modelRender.Draw(rc);
		}

	}
}

