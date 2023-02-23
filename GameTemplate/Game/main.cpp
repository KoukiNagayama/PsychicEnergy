#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"


// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("PsychicEnergy"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 70.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	// レンダリングコンテキスト
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//auto engineTest = NewGO<EngineTest>(0, "engineTest");
	//auto game = NewGO<Game>(0, "game");
	auto title = NewGO<nsPsychicEnergy::nsTitle::Title>(0, "title");

	// レンダリングエンジンを初期化
	g_renderingEngine = new RenderingEngine;
	g_renderingEngine->Init();

	// 世界の回転クラスのインスタンスを生成。
	nsK2EngineLow::nsWorldRotation::g_worldRotation = new nsK2EngineLow::nsWorldRotation::WorldRotation;

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// 世界の回転
		nsK2EngineLow::nsWorldRotation::g_worldRotation->RotationWorldMatrix();

		nsK2EngineLow::nsWorldRotation::g_worldRotation->ResetWorldMatrix();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		// レンダリングエンジンの描画処理を実行。
		g_renderingEngine->Execute(renderContext);

		// 世界の回転クラスの更新処理を実行。
		nsK2EngineLow::nsWorldRotation::g_worldRotation->Update();

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;
	delete g_renderingEngine;
	delete nsK2EngineLow::nsWorldRotation::g_worldRotation;

	return 0;
}

