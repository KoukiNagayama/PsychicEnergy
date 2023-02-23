#include "stdafx.h"
#include "TitleBackGround.h"
#include "CommonDataForModel.h"

namespace nsPsychicEnergy
{

	namespace nsTitle
	{
		bool TitleBackGround::Start()
		{
			// tkmファイルパスの文字列。
			const char* filePath = nullptr;

			// 影をキャストするか。
			bool isShadowCaster = true;

			// tkmファイルパスを指定する。
			// 地面
			if (m_typeNum == nsBackGround::enModelType_Ground) {
				filePath = "Assets/modelData/backGroundModel/ground.tkm";
				// 地面オブジェクトだけは影をキャストしない。
				isShadowCaster = false;
			}
			// 箱1
			else if (m_typeNum == nsBackGround::enModelType_Box1) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box1.tkm";
			}
			// 箱2
			else if (m_typeNum == nsBackGround::enModelType_Box2) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box2.tkm";
			}
			// 箱3
			else if (m_typeNum == nsBackGround::enModelType_Box3) {
				filePath = "Assets/modelData/backGroundModel/backGroundModel_box3.tkm";
			}

			// モデルを初期化。
			m_modelRender.Init(
				filePath,
				nullptr,
				nsModel::NO_ANIMATION_CLIPS,
				enModelUpAxisZ,
				false,
				false,
				isShadowCaster,
				false
			);

			// モデルの座標、回転、拡大率を設定。
			m_modelRender.SetTRS(m_position, m_rotation, m_scale);

			// モデルを更新。
			m_modelRender.Update();
			return true;
		}

		void TitleBackGround::Render(RenderContext& rc)
		{
			// 描画。
			m_modelRender.Draw(rc);

		}
	}
}