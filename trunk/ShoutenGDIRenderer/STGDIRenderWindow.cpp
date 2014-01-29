#include "STGDIRenderWindow.h"
#include "STWindow.h"
#include "STRenderWindowEventProcessor.h"
#include "STGDIRenderTarget.h"

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
	mWnd = ::CreateWindowEx(WS_EX_LAYERED /*^ WS_EX_TOOLWINDOW*/, 
		win->getName().c_str(), 
		win->getName().c_str(), 
		0,
		CW_USEDEFAULT, CW_USEDEFAULT, win->getWidth(), win->getHeight(),
		NULL, NULL, instance, NULL);

	HRESULT ret = ::GetLastError();

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
	RectI rect(0, 0, getWindow()->getWidth(), getWindow()->getHeight());
	return rect;
}
void GDIRenderWindow::notifyUpdateWindow()
{
	Window* win = getWindow();
	const RectI& rect = win->getAbsRect();
	::SetWindowPos(mWnd, HWND_NOTOPMOST, rect.left, rect.top, rect.width(), rect.height(), SWP_NOACTIVATE);
}

void GDIRenderWindow::render()
{

	GDIRenderTarget* target = (GDIRenderTarget*)getRenderTarget();

	HDC dc = GetDC(mWnd);

	RECT winrect;
	::GetWindowRect(mWnd, &winrect);
	size_t winwidth = winrect.right - winrect.left;
	size_t winheight = winrect.bottom - winrect.top;
	POINT dstpos = { winrect.left, winrect.top };
	POINT srcpos = { 0, 0 };
	SIZE size = { winwidth, winheight };
	BLENDFUNCTION bf = { 0 };
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 0xff;

	BOOL ret = ::UpdateLayeredWindow(mWnd, dc, &dstpos, &size, target->getDC(), &srcpos, 0, &bf, ULW_ALPHA);

	ReleaseDC(mWnd, dc);

	//ע��˳����
	//  o-> fill -> child draw -> update dc ->o
	//	|_____________________________________|

	target->fill(0x80ffffff);

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