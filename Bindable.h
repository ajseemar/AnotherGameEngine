#pragma once

namespace AnotherGameEngine
{
	class IBindable
	{
	public:
		virtual ~IBindable() = default;
		virtual void Bind(std::shared_ptr<DX::DeviceResources> deviceResources) = 0;
	};
}