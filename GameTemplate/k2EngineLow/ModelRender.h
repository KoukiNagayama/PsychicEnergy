#pragma once
#include "ToonMap.h"
#include "MyRenderer.h"
#include "GeometryData.h"
namespace nsK2EngineLow
{
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="isDrawOutLine">輪郭線を描画するか。</param>
		/// <param name="isCharacterModel">キャラクターのモデルか。</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isDrawOutLine = false,
			bool isCharacterModel = false,
			bool isShadowCaster = true,
			bool isFloating = false
		);
		/// <summary>
		/// 特殊なフォワードレンダリング用モデルの初期化関数
		/// </summary>
		/// <param name="initData">モデルの初期化データ</param>
		void InitDirectlyNotifyForwardRendering(ModelInitData initData);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// 描画処理
		/// </summary>
		void Draw(RenderContext& rc);
		/// <summary>
		/// アニメーション再生
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補間時間(単位：秒)。</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		/// <returns>再生中ならばtrue。再生していないならばfalse</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// 座標、回転、拡大率を設定
		/// </summary>
		/// <param name="position">座標。</param>
		/// <param name="rotation">回転。</param>
		/// <param name="scale">拡大率。</param>
		void SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(position);
			SetRotation(rotation);
			SetScale(scale);
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="position">座標。</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="x">x座標。</param>
		/// <param name="y">y座標。</param>
		/// <param name="z">z座標。</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転。</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 拡大率を設定
		/// </summary>
		/// <param name="scale">拡大率。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 拡大率を設定
		/// </summary>
		/// <param name="x">x座標。</param>
		/// <param name="y">y座標。</param>
		/// <param name="z">z座標。</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}
		/// <summary>
		/// アニメーションの再生速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">アニメーションの再生速度。数値の分だけ速度が倍になる。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		/// <summary>
		/// アニメーションイベントの追加
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/// <summary>
		/// 浮遊状態かどうかを設定。
		/// </summary>
		void SetIsFloating(const bool isFloating)
		{
			if (isFloating) {
				m_floating = 1;
			}
			else {
				m_floating = 0;
			}
			
		}
		bool IsFloating() const
		{
			if (m_floating == 0) {
				return false;
			}
			else {
				return true;
			}
		}
		/// <summary>
		/// モデルを取得
		/// </summary>
		/// <returns>モデル。</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <param name="instanceId">インスタンスID。</param>
		/// <returns>ワールド行列。</returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_model.GetWorldMatrix();
		}
		/// <summary>
		/// シャドウキャスター？
		/// </summary>
		/// <returns>シャドウキャスターであればtrue。そうでなければfalse。</returns>
		bool IsShadowCaster() const
		{
			return m_isShadowCaster;
		}
	private:
		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// フォワードレンダリング用モデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		void InitForwardRenderingModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// PBRを利用して描画する背景モデルを初期化
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitBackGroundModelWithPBR(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// キャラクターのモデルを初期化
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitModelForBackWithOutLine(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isFloating = false
		);
		/// <summary>
		/// 深度値記録用モデルを初期化
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitDepthModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// シャドウマップに描画するモデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitShadowMapModel(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// ジオメトリ情報を初期化
		/// </summary>
		void InitGeometryData();
		/// <summary>
		/// ワールド行列の更新
		/// </summary>
		void UpdateWorldMatrix();

	private:
		Model									m_model;										// フォワードレンダリングで描画されるモデル
		Model									m_frontCullingModel;							// フロントカリングモデル
		Model									m_depthModel;									// 深度値抽出用モデル
		Model									m_shadowMapModel[NUM_SHADOW_MAP];				// シャドウマップに描画するモデル
		Skeleton								m_skeleton;										// スケルトン
		Animation								m_animation;									// アニメーション
		AnimationClip*							m_animationClips = nullptr;						// アニメーションクリップ
		int										m_numAnimationClips;							// アニメーションクリップの数
		Vector3									m_position = Vector3::Zero;						// 座標
		Quaternion								m_rotation = Quaternion::Identity;				// 回転
		Vector3									m_scale = Vector3::One;							// 拡大率
		float									m_animationSpeed = 1.0f;						// アニメーションの再生速度
		ToonMap									m_toonMap;										// トゥーンマップ
		int										m_floating = 0;									// 浮遊状態か
		bool									m_isShadowCaster = true;						// シャドウキャスターか
		GeometryData							m_geometryData;									// ジオメトリ情報
	};
}


