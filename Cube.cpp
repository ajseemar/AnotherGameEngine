#include "pch.h"
#include "Cube.h"
#include "Common/DirectXHelper.h"

using namespace AnotherGameEngine;

Cube::Cube(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources), m_loadingComplete(false), angle(0.f)
{
	CreateWindowSizeDependentResources();
	CreateDeviceDependentResources();
}

void Cube::CreateDeviceDependentResources()
{
	auto loadVSTask = DX::ReadDataAsync(L"VertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

	auto createVSTask = loadVSTask.then([this](std::vector<byte>& data) {
		m_deviceResources->GetD3DDevice()->CreateVertexShader(&data[0], data.size(), nullptr, &m_vertexShader);

		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};

		m_deviceResources->GetD3DDevice()->CreateInputLayout(ied, ARRAYSIZE(ied), &data[0], data.size(), &m_inputLayout);
	});

	auto createPSTask = loadPSTask.then([this](std::vector<byte>& data) {
		m_deviceResources->GetD3DDevice()->CreatePixelShader(&data[0], data.size(), nullptr, &m_pixelShader);
	});

	auto createMeshTask = (createVSTask && createPSTask).then([this]() {
		const float size = 1.f;

		std::vector<VERTEX> vertices = {

			// TOP

			{ -size, size, -size }, // front left	 6 -> 0
			{  size, size, -size }, // front right	 5 -> 1

			{ -size, size,  size }, // back left	 2 -> 2
			{  size, size,  size }, // back right	 1 -> 3

			// BOTTOM

			{ -size, -size, -size }, // front left	 7 -> 4
			{  size, -size, -size }, // front right	 8 -> 5

			{ -size, -size,  size }, // back left	 4 -> 6
			{  size, -size,  size }, // back right	 3 -> 7

		};

		//D3D11_BUFFER_DESC vbd = { 0 };
		//vbd.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(vertices);
		//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;;
		//vbd.StructureByteStride = sizeof(VERTEX);

		//D3D11_SUBRESOURCE_DATA vd = { 0 };
		//vd.pSysMem = &vertices;

		//m_deviceResources->GetD3DDevice()->CreateBuffer(&vbd, &vd, &m_vertexBuffer);
		m_vertexBuffer = std::make_unique<VertexBuffer>(m_deviceResources, vertices);
		bindables.push_back(m_vertexBuffer.get());


		UINT indices[] = {
			6u, 7u, 4u, 5u, 1u, 7u, 3u, 6u, 2u, 4u, 0u, 1u, 2u, 3u
		};

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;;
		ibd.StructureByteStride = sizeof(UINT);

		D3D11_SUBRESOURCE_DATA id = { 0 };
		id.pSysMem = &indices;

		m_deviceResources->GetD3DDevice()->CreateBuffer(&ibd, &id, &m_indexBuffer);

		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.ByteWidth = sizeof(CBUFFER);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA cbdata = { 0 };
		cbdata.pSysMem = &m_cb;

		m_deviceResources->GetD3DDevice()->CreateBuffer(&cbd, &cbdata, &m_constantBuffer);
	});

	createMeshTask.then([this]() { m_loadingComplete = true; });
}

void Cube::CreateWindowSizeDependentResources()
{
	DirectX::XMVECTOR vecCamPosition = DirectX::XMVectorSet(0, 0, -10, 0);
	DirectX::XMVECTOR vecCamLookAt = DirectX::XMVectorSet(0, 0, 0, 0);
	DirectX::XMVECTOR vecCamUp = DirectX::XMVectorSet(0, 1, 0, 0);

	m_cb.view = DirectX::XMMatrixLookAtLH(vecCamPosition, vecCamLookAt, vecCamUp);

	Windows::Foundation::Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	m_cb.projection = DirectX::XMMatrixPerspectiveFovLH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	m_cb.model = DirectX::XMMatrixIdentity();
}

void Cube::Update()
{
	if (!m_loadingComplete) return;
	angle += 0.01f;

	m_cb.model = DirectX::XMMatrixRotationY(angle) * DirectX::XMMatrixRotationX(angle);
}

void Cube::Render()
{
	if (!m_loadingComplete) return;

	UINT strides = sizeof(VERTEX);
	UINT offsets = 0u;

	auto context = m_deviceResources->GetD3DDeviceContext();

	for (auto b : bindables)
	{
		b->Bind(m_deviceResources);
	}

	context->IASetInputLayout(m_inputLayout.Get());
	//context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strides, &offsets);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	context->UpdateSubresource(m_constantBuffer.Get(), 0u, nullptr, &m_cb, 0u, 0u);
	context->VSSetConstantBuffers(0u, 1u, m_constantBuffer.GetAddressOf());
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0u);

	context->PSSetShader(m_pixelShader.Get(), nullptr, 0u);

	context->DrawIndexed(14, 0u, 0);
}

