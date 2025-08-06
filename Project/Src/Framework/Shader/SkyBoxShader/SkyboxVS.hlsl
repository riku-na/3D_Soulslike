cbuffer cbCamera : register(b0)
{
	matrix view;
	matrix proj;
};

struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 svpos : SV_POSITION;
	float3 dir : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

    // 平行移動を除いた回転成分のみのView行列
	matrix viewNoTrans = view;
	viewNoTrans._41 = 0.0f;
	viewNoTrans._42 = 0.0f;
	viewNoTrans._43 = 0.0f;

	float4 worldPos = mul(float4(input.pos, 1.0f), viewNoTrans);
	output.svpos = mul(worldPos, proj); // ← ここで正しい透視投影を使う

    // キューブマップ用方向
	output.dir = input.pos;

	return output;
}
