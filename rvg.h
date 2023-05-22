#pragma once
#include <windows.h>
#define RVG_START RvG::_hInstance = GetModuleHandle(NULL)
#define RVGW_MAINWINDOW 0
#define RVGW_DIALOG 1

typedef int func(HWND, HWND);

namespace RvG {

	class ParentWidget {
	public:
		HWND hWnd = 0;
		ParentWidget* children[256] = { };
		ParentWidget* parent = nullptr;
		func* bind = nullptr;
		int xPos = 0;
		int yPos = 0;
		int widLen = 0;
		int heiLen = 0;
		int setText(const wchar_t* text);
		int getText(wchar_t* output, int size);
		int setGeometry(int x, int y, int wid, int hei);
		int show();
		int hide();
		int bindCommand(func bind);
	};

	class Window : public ParentWidget {
	public:
		int type;
		Window(const wchar_t* titleText, int type = RVGW_DIALOG, int lx = CW_USEDEFAULT, int ly = CW_USEDEFAULT, int wid = CW_USEDEFAULT, int hei = CW_USEDEFAULT, long long otherStyle = 0L);
		int keepResponding();
		int respond();
	};

	class Button : public ParentWidget {
	public:
		Button(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle = 0L);
	};

	class InputBox : public ParentWidget {
	public:
		InputBox(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle = 0L);
	};

	class Container : public ParentWidget {
	public:
		Container(int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle = 0L);
	};

	class ListBox : public ParentWidget {
	public:
		ListBox(int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle = 0L);
		int add(const wchar_t* text);
		int getSelIndex();
		int setSelIndex(int index);
		int getText(int index, char* output);
	};

	class Label : public ParentWidget {
	public:
		Label(const wchar_t* text, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle = 0L);
	};

	static HINSTANCE _hInstance;

}