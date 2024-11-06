#include "Application.h"

namespace MintEditor
{
	Application::Application(HINSTANCE instance)
	{
		if (!Logger::Initialize())
		{
			OutputDebugStringA("Failed to initialize logger\n");
			MessageBox(nullptr, "Failed to initialize logger!", "Mint Editor", MB_OK | MB_ICONERROR);
			return;
		}

		Logger::Info("Application startup");

		// Initialize Editor
		m_Editor = std::make_unique<Editor>(instance);

		if (!m_Editor)
		{
			Logger::Fatal("Failed to initialize Editor");
			return;
		}

		m_Initialized = true;
	}

	Application::~Application()
	{
		if (!m_Initialized) return;

		Logger::Info("Application shutdown");

		if (!m_Editor || !m_Editor->IsRunning())
			return;

		// Shutdown Editor
		m_Editor->Shutdown();

		// Wait for Editor to finish
		while (m_Editor->IsRunning())
		{
			std::this_thread::sleep_for(100ms);
		}

		// Cleanup Instance
		m_Editor.reset();

		Logger::Shutdown();

		m_Initialized = false;
	}

	void Application::Run()
	{
		if (!m_Initialized)
		{
			OutputDebugStringA("Application not initialized\n");
			return;
		}

		while (m_Editor && m_Editor->IsRunning())
		{
			m_Editor->Run();
		}
	}
}