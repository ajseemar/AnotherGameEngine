#include "pch.h"
#include "IndexBuffer.h"

using namespace AnotherGameEngine;

AnotherGameEngine::IndexBuffer::IndexBuffer(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<UINT> indices)
{
	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(UINT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;;
	ibd.StructureByteStride = sizeof(UINT);

	D3D11_SUBRESOURCE_DATA id = { 0 };
	id.pSysMem = indices.data();

	deviceResources->GetD3DDevice()->CreateBuffer(&ibd, &id, &m_indexBuffer);
}

void AnotherGameEngine::IndexBuffer::Bind(std::shared_ptr<DX::DeviceResources> deviceResources)
{
	deviceResources->GetD3DDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}
