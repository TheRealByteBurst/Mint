#include "EditorPCH.h"
#include "Core/Application.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdShow);

	MintEditor::Application* application = new MintEditor::Application(instance);

	application->Run();

	delete application;

	return EXIT_SUCCESS;
}