#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include "Bindable.h"
#include "Common/DeviceResources.h"

namespace AnotherGameEngine
{
	class IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(std::shared_ptr<DX::DeviceResources> deviceResources, std::vector<UINT> indices);
		virtual void Bind(std::shared_ptr<DX::DeviceResources> deviceResources) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	};
}