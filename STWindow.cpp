#include "STWindow.h"

using namespace ST;

Window::Window(const String& name, const CustomParameters* paras, const WindowFactory* f):
	mName(name), mFactory(f)
{
}

Window::~Window()
{

}

const WindowFactory* Window::getFactory()const
{
	return mFactory;
}


const String DefaultWinFactory::NAME = L"DefaultWinFactory";

Window* DefaultWinFactory::createWindowImpl(const String& name, const CustomParameters* paras)const
{
	return new Window(name, paras, this);
}

void DefaultWinFactory::destroyWindowImpl(Window* window)const
{
	delete window;
}