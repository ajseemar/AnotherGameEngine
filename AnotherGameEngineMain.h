﻿#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"

// Renders Direct2D and 3D content on the screen.
namespace AnotherGameEngine
{
	class AnotherGameEngineMain : public DX::IDeviceNotify
	{
	public:
		AnotherGameEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~AnotherGameEngineMain();
		void CreateWindowSizeDependentResources();
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
	};
}