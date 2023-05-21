// RiverGuiFrame.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "rvg.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE: {
		DefWindowProc(hwnd, uMsg, wParam, lParam);
		ExitProcess(0);
		return 0;
	}
	case WM_DESTROY: {
		ExitProcess(0);
		return 0;
	}
	case WM_COMMAND: {
		if (wParam > 255) break;
		RvG::ParentWidget* pw = (RvG::ParentWidget*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pw->children[LOWORD(wParam)] != NULL)
			if (pw->children[LOWORD(wParam)]->bind != NULL)
				pw->children[LOWORD(wParam)]->bind(hwnd, (HWND)lParam);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
		EndPaint(hwnd, &ps);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int RvG::ParentWidget::show() {
	ShowWindow(this->hWnd, 1);
	for (RvG::ParentWidget* i : this->children) {
		if (i == nullptr) break;
		i->show();
	}
	return 0;
}

int RvG::ParentWidget::hide() {
	ShowWindow(this->hWnd, 0);
	for (RvG::ParentWidget* i : this->children) {
		if (i == nullptr) break;
		i->hide();
	}
	return 0;
}

int RvG::ParentWidget::setTitleText(const wchar_t* titleText) {
	SetWindowText(this->hWnd, titleText);
	return 0;
}

int RvG::ParentWidget::setGeometry(int x, int y, int wid, int hei) {
	MoveWindow(this->hWnd, x, y, wid, hei, TRUE);
	this->widLen = wid;
	this->heiLen = hei;
	return 0;
}

int RvG::ParentWidget::bindCommand(func bind) {
	this->bind = bind;
	return 0;
}

RvG::Window::Window(const wchar_t* titleText) {
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = RvG::_hInstance;
	wc.lpszClassName = L"RvG W";
	//wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClass(&wc);
	this->hWnd = CreateWindowEx(0, L"RvG W", L"Win32 Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, RvG::_hInstance, NULL);
	
	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setTitleText(titleText);
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)this);
}

int RvG::Window::keepResponding() {
	MSG msg = { };
	while (GetMessage(&msg, this->hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

int RvG::Window::respond() {
	MSG msg = { };
	if (GetMessage(&msg, this->hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

RvG::Button::Button(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent) {
	this->parent = argParent;
	int x = lx;
	int y = ly;
	while (1) {
		if (this->parent->parent == nullptr) break;
		x += this->parent->xPos + 1;
		y += this->parent->yPos + 1;
		this->parent = this->parent->parent;
	}
	this->xPos = x;
	this->yPos = y;
	this->widLen = wid;
	this->heiLen = hei;
	if (this->parent->children[255] != NULL) return;
	int id;
	for (id = 0; id < 256; id++) {
		if (this->parent->children[id] == NULL) {
			break;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (argParent->children[i] == NULL) {
			argParent->children[i] = this;
			break;
		}
	}
	this->hWnd = CreateWindowEx(0, L"Button", L"Win32 Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setTitleText(titleText);
}

RvG::Edit::Edit(const wchar_t* titleText, int lx, int ly, int wid, int hei, bool multiline, ParentWidget* argParent) {
	this->parent = argParent;
	int x = lx;
	int y = ly;
	while (1) {
		if (this->parent->parent == nullptr) break;
		x += this->parent->xPos + 1;
		y += this->parent->yPos + 1;
		this->parent = this->parent->parent;
	}
	this->xPos = x;
	this->yPos = y;
	this->widLen = wid;
	this->heiLen = hei;
	if (this->parent->children[255] != NULL) return;
	int id;
	for (id = 0; id < 256; id++) {
		if (this->parent->children[id] == NULL) {
			break;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (argParent->children[i] == NULL) {
			argParent->children[i] = this;
			break;
		}
	}
	long style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_WANTRETURN;
	if (multiline) style |= ES_MULTILINE;
	this->hWnd = CreateWindowEx(0, L"Edit", L"Win32 Edit", style,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;
	//SendMessage(this->hWnd, WM_SETFONT, WPARAM(RvG::_hFont), TRUE);

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setTitleText(titleText);
}

RvG::Container::Container(int lx, int ly, int wid, int hei, ParentWidget* argParent) {
	this->parent = argParent;
	int x = lx;
	int y = ly;
	while (1) {
		if (this->parent->parent == nullptr) break;
		x += this->parent->xPos + 1;
		y += this->parent->yPos + 1;
		this->parent = this->parent->parent;
	}
	this->xPos = x;
	this->yPos = y;
	this->widLen = wid + 2;
	this->heiLen = hei + 2;
	if (this->parent->children[255] != NULL) return;
	this->parent = parent;
	int id;
	for (id = 0; id < 256; id++) {
		if (this->parent->children[id] == NULL) {
			break;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (argParent->children[i] == NULL) {
			argParent->children[i] = this;
			break;
		}
	}
	this->hWnd = CreateWindowEx(0, L"Static", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		x, y, widLen, heiLen, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
}

RvG::ListBox::ListBox(int lx, int ly, int wid, int hei, ParentWidget* argParent) {
	this->parent = argParent;
	int x = lx;
	int y = ly;
	while (1) {
		if (this->parent->parent == nullptr) break;
		x += this->parent->xPos + 1;
		y += this->parent->yPos + 1;
		this->parent = this->parent->parent;
	}
	this->xPos = x;
	this->yPos = y;
	this->widLen = wid;
	this->heiLen = hei;
	if (this->parent->children[255] != NULL) return;
	int id;
	for (id = 0; id < 256; id++) {
		if (this->parent->children[id] == NULL) {
			break;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (argParent->children[i] == NULL) {
			argParent->children[i] = this;
			break;
		}
	}
	this->hWnd = CreateWindowEx(0, L"ListBox", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_HASSTRINGS,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;
	//SendMessage(this->hWnd, WM_SETFONT, WPARAM(RvG::_hFont), TRUE);

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
}

int RvG::ListBox::add(const char* text) {
	SendMessageA(this->hWnd, LB_ADDSTRING, 0, (LPARAM)text);
	return 0;
}

int RvG::ListBox::getSelIndex() {
	int t = SendMessage(this->hWnd, LB_GETCURSEL, 0, 0);
	return t;
}

int RvG::ListBox::setSelIndex(int index) {
	SendMessage(this->hWnd, LB_SETCURSEL, index, 0);
	return 0;
}

int RvG::ListBox::getText(int index, char* output) {
	SendMessageA(this->hWnd, LB_GETTEXT, index, (LPARAM)output);
	return 0;
}

RvG::Label::Label(const wchar_t* text, int lx, int ly, int wid, int hei, ParentWidget* argParent) {
	this->parent = argParent;
	int x = lx;
	int y = ly;
	while (1) {
		if (this->parent->parent == nullptr) break;
		x += this->parent->xPos + 1;
		y += this->parent->yPos + 1;
		this->parent = this->parent->parent;
	}
	this->xPos = x;
	this->yPos = y;
	this->widLen = wid;
	this->heiLen = hei;
	if (this->parent->children[255] != NULL) return;
	int id;
	for (id = 0; id < 256; id++) {
		if (this->parent->children[id] == NULL) {
			break;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (argParent->children[i] == NULL) {
			argParent->children[i] = this;
			break;
		}
	}
	this->hWnd = CreateWindowEx(0, L"Static", L"Win32 Label", WS_CHILD | WS_VISIBLE,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setTitleText(text);
}
