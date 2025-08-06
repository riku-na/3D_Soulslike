#pragma once
#include <chrono>

namespace deltatime
{
	// DeltaTimeManager クラス：ゲームループのフレームごとの経過時間（DeltaTime）を管理するシングルトンクラス
	class DeltaTimeManager
	{
	public:
		static DeltaTimeManager& Instance()
		{
			static DeltaTimeManager instance;
			return instance;
		}

		//経過時間を更新(毎フレームゲームループの最初に呼ぶ！)
		void Update()
		{
			auto now = std::chrono::high_resolution_clock::now();
			m_unscaledDeltaTime = std::chrono::duration<double >(now - m_prevTime).count();
			m_prevTime = now;

			//OutputDebugStringA(("DeltaTime : " + std::to_string(m_unscaledDeltaTime) + "\n").c_str());

			m_deltaTime = m_unscaledDeltaTime * m_timeScale;
		}

		//リセット(シーン遷移や一時停止解除時などに使用)
		void Reset()
		{
			m_prevTime = std::chrono::high_resolution_clock::now();
			m_unscaledDeltaTime = 0.0;
			m_deltaTime = 0.0;
		}

		//TimerScale制御
		void SetTimeScale(double  _scale) { m_timeScale = std::max(0.0, _scale); }
		double  GetTimeScale()const { return m_timeScale; }

		//DeltaTime取得
		double  GetDeltaTime()const { return m_deltaTime; }
		double  GetUnscaledDeltaTime()const { return m_unscaledDeltaTime; }

	private:
		//コンストラクタ : 初期化時に現在時刻を記録
		DeltaTimeManager() :
			m_prevTime(std::chrono::high_resolution_clock::now()),
			m_deltaTime(0.0),
			m_unscaledDeltaTime(0.0),
			m_timeScale(1.0)
		{

		}

		~DeltaTimeManager() = default;

		//コピー禁止
		DeltaTimeManager(const DeltaTimeManager&) = delete;
		DeltaTimeManager& operator=(const DeltaTimeManager&) = delete;

		std::chrono::high_resolution_clock::time_point m_prevTime;	// 前フレームの記録時間
		double  m_deltaTime;			// タイムスケールが適用されたDeltaTime
		double  m_unscaledDeltaTime;	// 実際の経過時間
		double  m_timeScale;			// タイムスケール（0.0:ポーズ、1.0:通常、2.0:倍速など）
	};

	inline double GetDeltaTime() { return DeltaTimeManager::Instance().GetDeltaTime(); }
	inline double GetUnscaledDeltaTime() { return DeltaTimeManager::Instance().GetUnscaledDeltaTime(); }
	inline double GetTimeScale() { return DeltaTimeManager::Instance().GetTimeScale(); }
}