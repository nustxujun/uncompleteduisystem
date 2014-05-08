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
		RenderWindowEventProcessor();
		~RenderWindowEventProcessor();

		static void pumpMessage();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		static LRESULT CALLBACK handleHook(int nCode, WPARAM wParam, LPARAM lParam);
		void GlobalMessageProc(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam);

	private:

		HHOOK mHook;
	};
}

#endif