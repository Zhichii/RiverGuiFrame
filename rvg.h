#pragma once
#include <windows.h>

typedef int func(HWND, HWND);

namespace RvG {
	
	class ParentWidget {
	public:
		HWND hWnd = 0;
		ParentWidget* children[256] = { };
		ParentWidget* parent = nullptr;
		func* bind = nullptr;
		int setTitleText(const wchar_t* titleText);
		int setGeometry(int x, int y, int wid, int hei);
		int show();
		int hide();
		int bindCommand(func bind);
	};

	class Window : public ParentWidget {
	public:
		Window(const wchar_t* className, const wchar_t* titleText);
		int keepResponding();
	};

	class Button : public ParentWidget {
	public:
		Button(const wchar_t* titleText, int x, int y, int wid, int hei, ParentWidget* parent);
	};

	class Edit : public ParentWidget {
	public:
		Edit(const wchar_t* titleText, int x, int y, int wid, int hei, ParentWidget* parent);
	};

	static HFONT _hFont;
	static HINSTANCE _hInstance;
	int start();
	int end();

}