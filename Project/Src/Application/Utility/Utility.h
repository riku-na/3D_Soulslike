#pragma once

namespace Utility
{
	namespace Ease
	{
		[[nodiscard]] inline float EaseInSine(float x)
		{
			return 1.0f - std::cos((x * DirectX::XM_PI) / 2.0f);
		}

		[[nodiscard]] inline float EaseOutSine(float x)
		{
			return std::sin((x * DirectX::XM_PI) / 2.0f);
		}

		[[nodiscard]] inline float EaseInOutSine(float x)
		{
			return -(std::cos(DirectX::XM_PI * x) - 1.0f) / 2.0f;
		}

		[[nodiscard]] inline float EaseInCubic(float x)
		{
			return x * x * x;
		}

		[[nodiscard]] inline float EaseOutCubic(float x)
		{
			return 1.0f - std::pow(1.0f - x, 3.0f);
		}

		[[nodiscard]] inline float EaseInOutCubic(float x)
		{
			return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
		}

		[[nodiscard]] inline float EaseInOutQuad(float x)
		{
			return x < 0.5f ? 2.0f * x * x : 1.0f - std::pow(-2.0f * x + 2.0f, 2.0f) / 2.0f;
		}


		//関数
		inline Math::Vector2 MoveWithEasing(
			const Math::Vector2& _startPos,
			const Math::Vector2& _destPos,
			float _currentTime,
			float _duration,
			std::function<float(float)> _easingFunc
		) {
			float t = std::clamp(_currentTime / _duration, 0.0f, 1.0f);
			float easedT = _easingFunc(t);
			return _startPos + (_destPos - _startPos) * easedT;
		}

		inline Math::Vector3 MoveWithEasing(
			const Math::Vector3& _startPos,
			const Math::Vector3& _destPos,
			float _currentTime,
			float _duration,
			std::function<float(float)>_easingFunc)
		{
			float t = std::clamp(_currentTime / _duration, 0.0f, 1.0f);
			float easedT = _easingFunc(t);
			return _startPos + (_destPos - _startPos) * easedT;
		}


		inline Math::Vector3 Hermite(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& t0, const Math::Vector3& t1, float t)
		{
			float tt = t * t;
			float ttt = tt * t;

			float h00 = 2 * ttt - 3 * tt + 1;
			float h10 = ttt - 2 * tt + t;
			float h01 = -2 * ttt + 3 * tt;
			float h11 = ttt - tt;

			return h00 * p0 + h10 * t0 + h01 * p1 + h11 * t1;
		}

		inline Math::Quaternion Slerp(const Math::Quaternion& q0, const Math::Quaternion& q1, float t)
		{
			return Math::Quaternion::Slerp(q0, q1, t);
		}
	}

	inline Math::Quaternion LookRotation(const Math::Vector3& forward, const Math::Vector3& up = Math::Vector3::Up)
	{
		Math::Vector3 z = forward;
		z.Normalize();

		Math::Vector3 x = up.Cross(z);
		x.Normalize();

		Math::Vector3 y = z.Cross(x);

		Math::Matrix mat;
		mat.Right(x);
		mat.Up(y);
		mat.Backward(z);

		return Math::Quaternion::CreateFromRotationMatrix(mat);
	}

	namespace shader
	{
		inline const char* GetAddressModeStr(D3D11_TEXTURE_ADDRESS_MODE mode)
		{
			switch (mode)
			{
			case D3D11_TEXTURE_ADDRESS_WRAP: return "WRAP";
			case D3D11_TEXTURE_ADDRESS_MIRROR: return "MIRROR";
			case D3D11_TEXTURE_ADDRESS_CLAMP: return "CLAMP";
			case D3D11_TEXTURE_ADDRESS_BORDER: return "BORDER";
			case D3D11_TEXTURE_ADDRESS_MIRROR_ONCE: return "MIRROR_ONCE";
			default: return "UNKNOWN";
			}
		}

		inline void CheckSamplerState()
		{
			ID3D11SamplerState* sampler = nullptr;
			KdDirect3D::Instance().WorkDevContext()->PSGetSamplers(0, 1, &sampler);

			if (sampler)
			{
				D3D11_SAMPLER_DESC desc;
				sampler->GetDesc(&desc);

				// フィルタ確認
				OutputDebugStringA("[Sampler] Filter: ");
				switch (desc.Filter)
				{
				case D3D11_FILTER_MIN_MAG_MIP_POINT:
					OutputDebugStringA("POINT\n"); break;
				case D3D11_FILTER_MIN_MAG_MIP_LINEAR:
					OutputDebugStringA("LINEAR\n"); break;
				case D3D11_FILTER_ANISOTROPIC:
					OutputDebugStringA("ANISOTROPIC\n"); break;
				default:
					OutputDebugStringA("OTHER\n"); break;
				}

				// アドレスモード確認
				OutputDebugStringA(("[Sampler] AddressU: " + std::string(GetAddressModeStr(desc.AddressU)) + "\n").c_str());
				OutputDebugStringA(("[Sampler] AddressV: " + std::string(GetAddressModeStr(desc.AddressV)) + "\n").c_str());
				OutputDebugStringA(("[Sampler] AddressW: " + std::string(GetAddressModeStr(desc.AddressW)) + "\n").c_str());


				char buf[256];
				sprintf_s(buf,
					"[Sampler Slot %d] AddressU: %d, AddressV: %d\n",
					0,
					static_cast<int>(desc.AddressU),
					static_cast<int>(desc.AddressV)
				);
				OutputDebugStringA(buf);

				sampler->Release(); // 解放忘れずに
			}
		}

		/*	inline void CheckSamplerState(int slot)
			{
				ID3D11SamplerState* sampler = nullptr;
				KdDirect3D::Instance().WorkDevContext()->PSGetSamplers(slot, 1, &sampler);
				if (sampler)
				{
					D3D11_SAMPLER_DESC desc;
					sampler->GetDesc(&desc);

					const char* addrModeStr[] = { "WRAP", "MIRROR", "CLAMP", "BORDER", "MIRROR_ONCE" };

					char buf[256];
					sprintf_s(buf,
						"[Sampler Slot %d] Filter: %d, AddressU: %s, AddressV: %s\n",
						slot,
						desc.Filter,
						addrModeStr[desc.AddressU],
						addrModeStr[desc.AddressV]
					);
					OutputDebugStringA(buf);

					sampler->Release();
				}
			}*/


	}
}