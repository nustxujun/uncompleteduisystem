#ifndef _RenderWindowEventProcessor_H_
#define _RenderWindowEventProcessor_H_

#include "STCommon.h"
#include <Windows.h>
#include "STSingleton.h"

namespace ST
{
	class ShoutenExport RenderWindowEventProcessor : public Singleton<RenderWindowEventProcessor>
	{
	public: 
		static void pumpMessage();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void addListener(RenderWindow* win);
		void removeListener(RenderWindow* win);

	private:
		typedef std::set<RenderWindow*> RenderWindows;
		RenderWindows mWindows;
	};
}

#endif