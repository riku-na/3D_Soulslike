#pragma once

struct UpdateCtx
{
	float dt;
	void* blackboard = nullptr;
};

struct Transition
{
	std::string to;
	std::function<bool(const UpdateCtx&)>condition;
	std::function<void(const UpdateCtx&)>onTransit = nullptr;
};

struct State
{
	std::string name;
	std::function<void(const UpdateCtx&)>onEnter = nullptr;
	std::function<void(const UpdateCtx&)>onUpdate = nullptr;
	std::function<void(const UpdateCtx&)>onExit = nullptr;
	std::vector<Transition>transitions;
};

class StateMachineComponent : public Component
{
public:
	void AddState(State s) { states_[s.name] = std::move(s); }
	void SetInitial(const std::string& name) { current_ = name; firstTick_ = true; }
	const std::string& Current()const { return current_; }

	void Update(const UpdateCtx& ctx)
	{
		if (current_.empty())return;
		auto& s = states_.at(current_);

		if (firstTick_) { if (s.onEnter) s.onEnter(ctx); firstTick_ = false; }

		if (s.onUpdate)s.onUpdate(ctx);

		for (auto& t : s.transitions) {
			if (t.condition && t.condition(ctx)) {

				// ★ 再遷移ガード：同一ステートへは遷移しない
				if (t.to == current_) {
					// 必要ならログ
					//OutputDebugStringA(("[SM] suppress re-transition to " + current_ + "\n").c_str());
					continue;
				}

				// exit current
				if (s.onExit) s.onExit(ctx);
				if (t.onTransit) t.onTransit(ctx);

				current_ = t.to;
				firstTick_ = true; // 次フレームで onEnter
				break;
			}
		}
	}

private:
	std::unordered_map<std::string, State> states_;
	std::string current_;
	bool firstTick_ = false;
};