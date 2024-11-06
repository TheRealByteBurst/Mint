#include "Editor.h"

namespace MintEditor
{
	Editor::Editor(HINSTANCE instance) : m_Running(true), m_InstanceHandle(instance)
	{
		Logger::Info("Editor initialized");
	}

	void Editor::Shutdown()
	{
		m_Running = false;

		Logger::Info("Editor shutdown");
	}

	void Editor::Run()
	{
		while (m_Running)
		{
			// Do stuff
		}
	}
}