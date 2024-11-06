#pragma once

namespace MintEditor
{
	class Editor
	{
	public:
		Editor(HINSTANCE instance);
		~Editor() = default;

		void Shutdown();
		void Run();

		bool IsRunning() const { return m_Running; }

	private:
		bool m_Running;
		HINSTANCE m_InstanceHandle;
	};
}