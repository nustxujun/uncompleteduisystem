#include "STWindowFactory.h"
#include "STWindow.h"

using namespace ST;

const Char* DefaultWinFactory::NAME = L"DefaultWinFactory";

Window* DefaultWinFactory::createWindowImpl(const String& name, WindowManager* wm)const
{
	return new Window(name, this, wm);
}

void DefaultWinFactory::destroyWindowImpl(Window* window)const
{
	delete window;
}