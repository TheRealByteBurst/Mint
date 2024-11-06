#pragma once
#include "Editor/Editor.h"

namespace MintEditor
{
	class Application
	{
	public:
		Application(HINSTANCE instance);
		~Application();

		void Run();

	private:
		std::unique_ptr<Editor> m_Editor;
		bool m_Initialized = false;
	};
}