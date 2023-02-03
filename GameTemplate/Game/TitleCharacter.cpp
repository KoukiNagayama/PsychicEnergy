#include "stdafx.h"
#include "TitleCharacter.h"
#include "Title.h"

namespace
{
	const float INCREASE_Y_WHEN_FLOATING = 30.0f;
	const float DISTANCE_TRAVEL_PER_FRAME = 20.0f;
	const float ANIMATION_INTERPORATE_TIME = 0.3f;	// アニメーションの補間時間

}

TitleCharacter::~TitleCharacter()
{
	g_renderingEngine->SetIsFloating(false);
}

bool TitleCharacter::Start()
{
	// タイトルクラスのインスタンスを検索
	m_title = FindGO<Title>("title");

	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/WD/idle_2.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_IdleAir].Load("Assets/animData/WD/idle_air_4.tka");
	m_animationClips[enAnimationClip_IdleAir].SetLoopFlag(true);
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
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	m_modelRender.Update();

	return true;
}

void TitleCharacter::Update()
{
	if (!m_isCompletedStateChange && m_title->IsPushedB()) {
		m_modelRender.SetIsFloating(true);
		g_renderingEngine->SetIsFloating(true);
		m_position.y += INCREASE_Y_WHEN_FLOATING;
		m_currentAnimationClip = enAnimationClip_IdleAir;
		m_isFloating = true;
		m_isCompletedStateChange = true;
	}

	if (m_isFloating) {
		m_floatingCount += g_gameTime->GetFrameDeltaTime();
		if (m_floatingCount >= 0.3f) {
			m_currentAnimationClip = enAnimationClip_FallAir;
			m_position.z += DISTANCE_TRAVEL_PER_FRAME;
		}
	}

	PlayAnimation();

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