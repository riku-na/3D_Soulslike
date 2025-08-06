#pragma once

namespace input
{
	constexpr int KEY_COUNT = 256;
	constexpr SHORT KEY_STATE_MASK = static_cast<SHORT>(0x8000);

	class InputManager
	{
	public:
		static InputManager& Instance()
		{
			static InputManager s_instance;
			return s_instance;
		}

		void Init(HWND hwnd)
		{
			m_hwnd = hwnd;

			RECT rect;
			GetClientRect(hwnd, &rect);
			POINT center = {
				(rect.left + rect.right) / 2,
				(rect.top + rect.bottom) / 2
			};
			ClientToScreen(hwnd, &center);
			m_centerPos = center;
			m_prevMousePos = center;

			// 最初に中央に固定
			if (m_lockMouseToCenter)
			{
				SetCursorPos(m_centerPos.x, m_centerPos.y);
				while (ShowCursor(FALSE) >= 0);
				m_isCursorVisible = false;
			}
		}

		// 追加：ウィンドウがオフスクリーン（最小化 or 非アクティブ）かどうか判定
		bool IsWindowOffScreen()
		{
			// 最小化されている
			if (IsIconic(m_hwnd))
				return true;

			// アクティブでない
			if (GetForegroundWindow() != m_hwnd)
				return true;

			return false;
		}

		// 毎フレーム最初に呼ぶ
		void Update()
		{
			UpdateKeyboard();

			// オフスクリーン時はマウスロックしない
			if (IsWindowOffScreen())
			{
				// ロック解除（必要なら）
				if (m_lockMouseToCenter)
				{
					m_lockMouseToCenter = false;
					while (ShowCursor(TRUE) < 0)
						m_isCursorVisible = true;
				}

				UpdateMouse(); // 入力更新だけしてリターン
				return;
			}

			if (IsKeyPressed(VK_ESCAPE) && m_lockMouseToCenter)
			{
				m_lockMouseToCenter = false;
				while (ShowCursor(TRUE) < 0)
					m_isCursorVisible = true;
			}

			// ウィンドウ内を左クリックしたときにマウスロック再開
			if (IsKeyPressed(VK_LBUTTON) && !m_lockMouseToCenter)
			{
				//ImGuiウィンドウ上ならスキップ
				if (ImGui::GetIO().WantCaptureMouse)
					return;

				//カーソル位置を取得
				POINT cursorPos;
				GetCursorPos(&cursorPos);

				//ウィンドウ矩形を取得
				RECT windowRect;
				GetWindowRect(m_hwnd, &windowRect);

				//ウィンドウ内なら再ロック
				if (PtInRect(&windowRect, cursorPos))
				{
					m_lockMouseToCenter = true;

					SetCursorPos(m_centerPos.x, m_centerPos.y);
					m_prevMousePos = m_centerPos;
					m_mouseDelta = { 0.0f,0.0f };

					while (ShowCursor(FALSE) >= 0); // 非表示
					m_isCursorVisible = false;
				}
			}

			UpdateMouse();
		}

		//マウス移動量取得(相対値)
		DirectX::XMFLOAT2 GetMouseDelta()const { return m_mouseDelta; }

		//キーが押されている
		bool IsKeyDown(int _keyCode)const
		{
			if (IsValidKey(_keyCode))
			{
				return m_current[_keyCode];
			}
			else
			{
				//存在しないキーコード
				return false;
			}
		}

		//押した瞬間(1フレームだけtrue)
		bool IsKeyPressed(int _keyCode)const
		{
			if (IsValidKey(_keyCode))
			{
				return m_pressed[_keyCode];
			}
			else
			{
				//存在しないキーコード
				return false;
			}
		}

		//離した瞬間(1フレームだけtrue)
		bool IsKeyReleased(int _keyCode)const
		{
			if (IsValidKey(_keyCode))
			{
				return m_released[_keyCode];
			}
			else
			{
				//存在しないキーコード
				return false;
			}
		}

		inline bool GetIsLockedMouse()const { return m_lockMouseToCenter; }

	private:
		InputManager() = default;

		//コピー禁止
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		//範囲外アクセス防止
		bool IsValidKey(int _keyCode)const
		{
			return _keyCode >= 0 && _keyCode < KEY_COUNT;
		}

		void UpdateKeyboard()
		{
			//ウィンドウがアクティブじゃなければ無効化
			if (GetForegroundWindow() != m_hwnd)
			{
				m_current.fill(false);
				m_pressed.fill(false);
				m_released.fill(false);
				return;
			}

			for (int i = 0; i < KEY_COUNT; i++)
			{
				SHORT keyState = GetAsyncKeyState(i);
				m_current[i] = (keyState & KEY_STATE_MASK) != 0;
				m_pressed[i] = !m_prev[i] && m_current[i];
				m_released[i] = m_prev[i] && !m_current[i];
			}
			m_prev = m_current;
		}



		void UpdateMouse()
		{
			POINT nowPos;
			GetCursorPos(&nowPos);

			m_mouseDelta.x = static_cast<float>(nowPos.x - m_prevMousePos.x);
			m_mouseDelta.y = static_cast<float>(nowPos.y - m_prevMousePos.y);

			if (m_lockMouseToCenter)
			{
				SetCursorPos(m_centerPos.x, m_centerPos.y);
				m_prevMousePos = m_centerPos;
			}
			else
			{
				m_mouseDelta.x = m_mouseDelta.y = 0;
				m_prevMousePos = nowPos;
			}
		}

		std::array<bool, KEY_COUNT> m_current{};	//現在の状態
		std::array<bool, KEY_COUNT>m_prev{};		//1フレーム前の状態
		std::array<bool, KEY_COUNT>m_pressed{};	//押した瞬間
		std::array<bool, KEY_COUNT>m_released{};	//離した瞬間

		//マウス情報
		POINT m_centerPos{};
		POINT m_prevMousePos{};
		DirectX::XMFLOAT2 m_mouseDelta{};
		HWND m_hwnd = nullptr;

		bool m_lockMouseToCenter = true;
		bool m_isCursorVisible = false;

		int m_windowLeft = 0;
		int m_windowTop = 0;
		int m_windowRight = 0;
		int m_windowBottom = 0;
	};

	//ショートカット関数
	//押されているか
	inline bool IsKeyHeld(int _keyCode) { return InputManager::Instance().IsKeyDown(_keyCode); }
	//押した瞬間
	inline bool IsKeyTriggered(int _keyCode) { return InputManager::Instance().IsKeyPressed(_keyCode); }
	//離した瞬間
	inline bool IsKeyReleasedThisFrame(int _keyCode) { return InputManager::Instance().IsKeyReleased(_keyCode); }
	inline DirectX::XMFLOAT2 GetMouseDelta() { return InputManager::Instance().GetMouseDelta(); }

	//#define GetAsyncKeyState DO_NOT_USE_GetAsyncKeyState__Use_InputManager_Instead

}