#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include "Common/DeviceResources.h"

namespace AnotherGameEngine 
{
	class Cube
	{
	public:

		Cube(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();

		void Update();
		void Render();

	private:
		struct VERTEX {
			float x, y, z;
		};

		struct CBUFFER {
			DirectX::XMMATRIX model, view, projection;
		};

		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		CBUFFER m_cb;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

		bool m_loadingComplete;
		float angle;
	};

}
