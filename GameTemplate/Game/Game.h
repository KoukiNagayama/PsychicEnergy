#pragma once
#include "LevelRender.h"
#include "SkyCube.h"
class TestModel;
class Player;
class GameCamera;

class Game : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// デスストラクタ
	/// </summary>
	~Game();	
	/// <summary>
	/// 開始処理。
	/// インスタンス生成時に一度だけ実行。
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	void Render(RenderContext& rc);
private:

private:
	LevelRender		m_levelRender;						// レベルレンダー
	Player*			m_player = nullptr;					// プレイヤー
	GameCamera*		m_gameCamera = nullptr;				// ゲームカメラ
	TestModel*		m_testModel0 = nullptr;				// テストモデル1
	TestModel*		m_testModel1 = nullptr;				// テストモデル2
	TestModel*		m_testModel2 = nullptr;				// テストモデル3
	SkyCube*		m_skyCube = nullptr;				// スカイキューブ
	SpriteRender	m_sprite;
};

