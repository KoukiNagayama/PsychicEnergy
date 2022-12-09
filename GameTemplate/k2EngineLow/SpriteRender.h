#pragma once
namespace nsK2EngineLow
{
	class SpriteRender : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="w">画像の横幅</param>
		/// <param name="h">画像の縦幅</param>
		/// <param name="alphaBlendMode">アルファブレンディングモード。デフォルトは半透明合成</param>
		void Init(const char* filePath,
			const float w,
			const float h,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans
		);
		/// <summary>
		/// 特殊な画像を初期化
		/// </summary>
		/// <param name="spriteInitData">スプライトの初期化データ</param>
		void InitSpecialImage(SpriteInitData spriteInitData);
		/// <summary>
		/// 更新
		/// </summary>
		void Update();
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Draw(RenderContext& rc);
		/// <summary>
		/// 座標を設定。zは0.0fで。
		/// </summary>
		/// <param name="pos">座標。</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns>座標。</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 大きさを設定。zは1.0fで。
		/// </summary>
		/// <param name="scale">大きさ。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを取得。
		/// </summary>
		/// <returns>大きさ。</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot">回転。</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// 回転を取得。
		/// </summary>
		/// <returns>回転。</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}
		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot">ピボット。</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得。
		/// </summary>
		/// <returns>ピボット。</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}
		/// <summary>
		/// 乗算カラーを設定。
		/// </summary>
		/// <param name="mulColor">乗算カラー。</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// 乗算カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
		}
	private:
		Sprite		m_sprite;								// スプライト
		Vector3		m_position = Vector3::Zero;				// 座標
		Quaternion	m_rotation = Quaternion::Identity;		// 回転
		Vector3		m_scale = Vector3::One;					// 大きさ
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		// ピボット

	};
}


