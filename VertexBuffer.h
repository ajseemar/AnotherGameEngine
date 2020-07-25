#pragma once

#include "Bindable.h"
#include <vector>

namespace AnotherGameEngine
{
	class VertexBuffer : public IBindable
	{
	public:
		template <class T>
		VertexBuffer(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<T> vertices) : offset(0), stride(static_cast<UINT>(sizeof(T)))
		{
			D3D11_BUFFER_DESC vbd = { 0 };
			vbd.ByteWidth = static_cast<UINT>(stride * vertices.size());
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;;
			vbd.StructureByteStride = stride;

			D3D11_SUBRESOURCE_DATA vd = { 0 };
			vd.pSysMem = vertices.data();

			deviceResources->GetD3DDevice()->CreateBuffer(&vbd, &vd, &m_vertexBuffer);
		}

		virtual void Bind(std::shared_ptr<DX::DeviceResources> deviceResources) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		UINT stride;
		UINT offset;
	};
}

