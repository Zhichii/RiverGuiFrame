// RiverGuiFrame.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "rvg.h"

LRESULT CALLBACK RvG::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RvG::Window* pw = (RvG::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (uMsg) {
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		if (pw->type == 0) {
			ExitProcess(0);
		}
		return 0;
	}
	case WM_DESTROY: {
		DestroyWindow(hwnd);
		if (pw->type == 0) {
			ExitProcess(0);
		}
		return 0;
	}
	case WM_COMMAND: {
		if (wParam > 255) break;
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
int RvG::ParentWidget::setText(const wchar_t* text) {
	SetWindowText(this->hWnd, text);
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

int RvG::ParentWidget::getText(wchar_t* output, int size) {
	GetWindowText(this->hWnd, output, size);
	return 0;
}

RvG::Window::Window(const wchar_t* titleText, int type, int lx, int ly, int wid, int hei, long long otherStyle) {
	this->type = type;
	this->hWnd = CreateWindowEx(0, L"RvG W", L"Win32 Window", WS_OVERLAPPEDWINDOW,
		lx, ly, wid, hei, NULL, NULL, RvG::_hInstance, NULL);
	
	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setText(titleText);
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)this);
}

int RvG::Window::keepResponding() {
	MSG msg = { };
	while (GetMessage(&msg, this->hWnd, 0, 0) > 0 && IsWindow(this->hWnd)) {
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
	else {
		return 1;
	}
	return 0;
}

RvG::Button::Button(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle) {
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
	this->hWnd = CreateWindowEx(0, L"Button", L"Win32 Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | otherStyle,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setText(titleText);
}

RvG::InputBox::InputBox(const wchar_t* titleText, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle) {
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
	this->hWnd = CreateWindowEx(0, L"Edit", L"Win32 InputBox", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_WANTRETURN | otherStyle,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;
	//SendMessage(this->hWnd, WM_SETFONT, WPARAM(RvG::_hFont), TRUE);

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setText(titleText);
}

RvG::Container::Container(int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle) {
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
	this->hWnd = CreateWindowEx(0, L"Static", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | otherStyle,
		x, y, widLen, heiLen, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
}

RvG::ListBox::ListBox(int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle) {
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
	this->hWnd = CreateWindowEx(0, L"ListBox", L"Win32 ListBox", WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_HASSTRINGS | otherStyle,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;
	//SendMessage(this->hWnd, WM_SETFONT, WPARAM(RvG::_hFont), TRUE);

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
}

int RvG::ListBox::add(const wchar_t* text) {
	SendMessage(this->hWnd, LB_ADDSTRING, 0, (LPARAM)text);
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

RvG::Label::Label(const wchar_t* text, int lx, int ly, int wid, int hei, ParentWidget* argParent, long long otherStyle) {
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
	this->hWnd = CreateWindowEx(0, L"Static", L"Win32 Label", WS_CHILD | WS_VISIBLE | otherStyle,
		x, y, wid, hei, this->parent->hWnd, (HMENU)id, RvG::_hInstance, NULL);
	this->parent->children[id] = this;

	if (this->hWnd == NULL) {
		return;
	}

	this->show();
	this->setText(text);
}
