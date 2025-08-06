#include "SkyboxShader.h"
#include <DirectXTK/Inc/DDSTextureLoader.h>

void SkyboxShader::Init()
{
	//入力レイアウトとシェーダのコンパイル
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	KdShaderManager::CompileShader(L"Src/Framework/Shader/SkyBoxShader/SkyboxVS.hlsl", "main", "vs_5_0", &m_VS, &m_inputLayout, layout, ARRAYSIZE(layout));
	KdShaderManager::CompileShader(L"Src/Framework/Shader/SkyBoxShader/SkyboxPS.hlsl", "main", "ps_5_0", &m_PS);

	const float size = 50.0f;
	SkyboxVertex vertices[] =
	{
		{{-size, size,-size}},	//0
		{{ size, size,-size}},	//1
		{{ size,-size,-size}},	//2
		{{-size,-size,-size}},	//3
		{{-size, size, size}},	//4
		{{ size, size, size}},	//5
		{{ size,-size, size}},	//6
		{{-size,-size, size}},	//7
	};

	DWORD indices[] =
	{
		//-z面(奥)
		0, 1, 2, 0, 2, 3,
		// +Z面（手前）
		5, 4, 7, 5, 7, 6,
		// -X面（左）
		4, 0, 3, 4, 3, 7,
		// +X面（右）
		1, 5, 6, 1, 6, 2,
		// +Y面（上）
		4, 5, 1, 4, 1, 0,
		// -Y面（下）
		3, 2, 6, 3, 6, 7,
	};

	//キューブモデルの頂点/インデックスバッファ作成
	m_indexCount = _countof(indices);

	//頂点バッファ作成
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.ByteWidth = sizeof(vertices);
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;

	KdDirect3D::Instance().WorkDev()->CreateBuffer(&vbDesc, &vbData, &m_vertexBuffer);

	//インデックスバッファ作成
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;

	KdDirect3D::Instance().WorkDev()->CreateBuffer(&ibDesc, &ibData, &m_indexBuffer);

	//定数バッファ作成
	m_cbCamera.Create();
}

bool SkyboxShader::LoadCubeMap(const std::wstring& filePath)
{
	HRESULT hr = DirectX::CreateDDSTextureFromFile
	(
		KdDirect3D::Instance().WorkDev(),
		filePath.c_str(),
		nullptr,
		m_cubeSRV.ReleaseAndGetAddressOf()
	);

	if (FAILED(hr))
	{
		OutputDebugStringA("Skybox DDS読み込み失敗\n");
		return false;
	}

	return true;
}

void SkyboxShader::Draw(const Math::Matrix& view, const Math::Matrix& proj)
{
	if (!m_cubeSRV || !m_vertexBuffer || !m_indexBuffer)return;

	ID3D11DeviceContext* context = KdDirect3D::Instance().WorkDevContext();

	D3D11_VIEWPORT vp;
	vp.Width = (float)1280;   // 画面幅
	vp.Height = (float)720; // 画面高さ
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	context->RSSetViewports(1, &vp);

	KdShaderManager::Instance().SetVertexShader(m_VS);
	KdShaderManager::Instance().SetPixelShader(m_PS);
	KdShaderManager::Instance().SetInputLayout(m_inputLayout);

	auto& cb = m_cbCamera.Work();
	Math::Matrix viewRotOnly = view;
	viewRotOnly.Translation(Math::Vector3::Zero);  // ← DX11/DirectXMathの正式な書き方
	cb.mView = viewRotOnly.Transpose();
	cb.mProj = proj.Transpose();
	m_cbCamera.Write();
	context->VSSetConstantBuffers(0, 1, m_cbCamera.GetAddress());

	UINT stride = sizeof(SkyboxVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ★ インデックスバッファの設定を追加
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// トポロジ設定
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// テクスチャとサンプラを設定
	context->PSSetShaderResources(0, 1, m_cubeSRV.GetAddressOf());

	// ステート設定
	KdShaderManager::Instance().ChangeDepthStencilState(KdDepthStencilState::ZWriteDisable);
	KdShaderManager::Instance().ChangeRasterizerState(KdRasterizerState::CullFront);
	KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);
	KdShaderManager::Instance().ChangeSamplerState(KdSamplerState::Anisotropic_Clamp);

	// ★ DrawIndexedに戻す
	context->DrawIndexed(m_indexCount, 0, 0);

	// ステート戻し
	KdShaderManager::Instance().UndoDepthStencilState();
	KdShaderManager::Instance().UndoRasterizerState();
	KdShaderManager::Instance().UndoBlendState();
	KdShaderManager::Instance().UndoSamplerState();
}

void SkyboxShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_inputLayout);
	KdSafeRelease(m_vertexBuffer);
	KdSafeRelease(m_indexBuffer);

	m_cubeSRV.Reset();
	m_cbCamera.Release();
}
