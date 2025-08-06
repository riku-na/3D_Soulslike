TextureCube skyTex : register(t0);
SamplerState smp : register(s0);

struct VS_OUT
{
	float4 svpos : SV_POSITION;
	float3 dir : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return skyTex.Sample(smp, normalize(input.dir));
}
