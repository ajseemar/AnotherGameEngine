#include "pch.h"
#include "VertexBuffer.h"

using namespace AnotherGameEngine;

//template <class T>
//VertexBuffer::VertexBuffer(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<T> vertices) : offset(0), stride(sizeof(T))
//{
//	D3D11_BUFFER_DESC vbd = { 0 };
//	vbd.ByteWidth = stride * vertices.size();
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;;
//	vbd.StructureByteStride = stride;
//
//	D3D11_SUBRESOURCE_DATA vd = { 0 };
//	vd.pSysMem = vertices.data();
//
//	deviceResources->GetD3DDevice()->CreateBuffer(&vbd, &vd, &m_vertexBuffer);
//}

void VertexBuffer::Bind(std::shared_ptr<DX::DeviceResources> deviceResources)
{
	deviceResources->GetD3DDeviceContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);
}
