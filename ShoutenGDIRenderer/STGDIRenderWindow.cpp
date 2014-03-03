#include "STGDIRenderWindow.h"
#include "STWindow.h"
#include "STRenderWindowEventProcessor.h"
#include "STGDIRenderTarget.h"
#include "STStringUtil.h"

using namespace ST;

GDIRenderWindow::GDIRenderWindow(Window* win, WindowRenderer* renderer):
RenderWindow(win, renderer)
{
	HMODULE instance = ::GetModuleHandle(NULL);

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = 0;
	wc.lpfnWndProc = RenderWindowEventProcessor::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = win->getName().c_str();
	::RegisterClassEx(&wc);


	//int width = win->getWidth() ? win->getWidth() : CW_USEDEFAULT;
	//int height = win->getHeight() ? win->getHeight() : CW_USEDEFAULT;

	const RectI& rect = win->getRect();

	mWnd = ::CreateWindowEx(WS_EX_LAYERED /*^ WS_EX_TOOLWINDOW*/, 
		win->getName().c_str(), 
		win->getName().c_str(), 
		WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, 1, 1,//WS_POPUP需要指定宽高
		NULL, NULL, instance, NULL);


	//::SetWindowPos(mWnd, HWND_NOTOPMOST, rect.left + realrect.left, rect.top + realrect.top, rect.width(), rect.height(), SWP_NOACTIVATE);

	::ShowWindow(mWnd, SW_SHOW);
	::UpdateWindow(mWnd);

	RenderWindowEventProcessor::getSingleton().addListener(this);
}

GDIRenderWindow::~GDIRenderWindow()
{
	RenderWindowEventProcessor::getSingleton().removeListener(this);
}

RectI GDIRenderWindow::getWorldAABB()
{
	RectI rect = getWindow()->getRect();
	return rect;

}
void GDIRenderWindow::notifyUpdateWindow(unsigned int dirty)
{
	if (!(dirty & (DT_POSITION | DT_SIZE))) return;
	const RectI& rect = getWindow()->getAbsRect();


	::SetWindowPos(mWnd, HWND_NOTOPMOST, rect.left , rect.top , rect.width(), rect.height(), SWP_NOACTIVATE);

	if (dirty & DT_PROPERTY)
		mBackgroundColor = StringUtil::toColour(
		getWindow()->getProperty(WindowProperty::BACKGROUND_COLOUR)).getAsARGB();
}

void GDIRenderWindow::render()
{
	using namespace std;
	GDIRenderTarget* target = (GDIRenderTarget*)getRenderTarget();

	HDC dc = GetDC(mWnd);

	RECT winrect;
	::GetWindowRect(mWnd, &winrect);
	size_t winwidth = winrect.right - winrect.left;
	size_t winheight = winrect.bottom - winrect.top;
	POINT dstpos = { winrect.left, winrect.top };
	POINT srcpos = { 0, 0 };
	SIZE size = { min(winwidth, target->getSize().width), min(winheight, target->getSize().height) };
	BLENDFUNCTION bf = { 0 };
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 0xff;

	BOOL ret = ::UpdateLayeredWindow(mWnd, dc, &dstpos, &size, target->getDC(), &srcpos, 0, &bf, ULW_ALPHA);

	ReleaseDC(mWnd, dc);

	//注意顺序是
	//  o-> fill -> child draw -> update dc ->o
	//	|_____________________________________|

	target->fill(mBackgroundColor);

}

RenderObject* GDIRenderWindowFactory::createImpl(Window* win, WindowRenderer* renderer)
{
	return new GDIRenderWindow(win, renderer);
}
void GDIRenderWindowFactory::destroyImpl(RenderObject* ro)
{
	delete (GDIRenderWindow*)ro;
}

const Char* GDIRenderWindowFactory::NAME = L"GDIRenderWindowFactory";