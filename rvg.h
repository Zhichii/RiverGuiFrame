#pragma once
#include <windows.h>
#define RVG_START RvG::_hInstance = GetModuleHandle(NULL)

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
		int setTitleText(const wchar_t* titleText);
		int setGeometry(int x, int y, int wid, int hei);
		int show();
		int hide();
		int bindCommand(func bind);
	};

	class Window : public ParentWidget {
	public:
		Window(const wchar_t* titleText);
		int keepResponding();
		int respond();
	};

	class Button : public ParentWidget {
	public:
		Button(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent);
	};

	class Edit : public ParentWidget {
	public:
		Edit(const wchar_t* titleText, int lx, int ly, int wid, int hei, bool multiline, ParentWidget* argParent);
	};

	class Container : public ParentWidget {
	public:
		Container(int lx, int ly, int wid, int hei, ParentWidget* argParent);
	};

	class ListBox : public ParentWidget {
	public:
		ListBox(int lx, int ly, int wid, int hei, ParentWidget* argParent);
		int add(const char* text);
		int getSelIndex();
		int setSelIndex(int index);
		int getText(int index, char* output);
	};

	class Label : public ParentWidget {
	public:
		Label(const wchar_t* text, int lx, int ly, int wid, int hei, ParentWidget* argParent);
	};

	static HINSTANCE _hInstance;

}