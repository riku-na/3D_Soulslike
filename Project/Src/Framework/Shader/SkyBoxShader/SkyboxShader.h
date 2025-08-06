#pragma once



class SkyboxShader
{
public:
	//シェーダ・バッファ・キューブモデル生成
	void Init();

	//キューブマップ読み込み
	bool LoadCubeMap(const std::wstring& filePath);

	//View行列の平行移動を除去 → 定数バッファ設定 → キューブ描画
	void Draw(const Math::Matrix& view, const Math::Matrix& proj);

	void Release();
private:
	struct SkyboxVertex
	{
		DirectX::XMFLOAT3 Pos;
	};

	//シェーダ
	ID3D11VertexShader* m_VS = nullptr;
	ID3D11PixelShader* m_PS = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	//定数バッファ
	struct cbSkyboxCamera
	{
		Math::Matrix mView;
		Math::Matrix mProj;
	};
	KdConstantBuffer<cbSkyboxCamera>m_cbCamera;

	//モデル
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	UINT m_indexCount = 0;

	//CubeMapテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_cubeSRV;
};