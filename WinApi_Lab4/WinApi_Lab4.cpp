// WinApi_Lab4.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WinApi_Lab4.h"
#include <string>

class TextBox {
private:

	RECT instance;
public:
	std::string label, textLine;
	LONG left, top, right, bottom;
	TextBox() {}
	TextBox(RECT rect, std::string label, std::string text) {
		left = instance.left = rect.left;
		top = instance.top = rect.top;
		right = instance.right = rect.right;
		bottom = instance.bottom = rect.bottom;
		this->textLine = text;
		this->label = label;
	}
	LONG width() {
		return instance.right - instance.left;
	}
	LONG height() {
		return instance.bottom - instance.top;
	}
	void Print(HDC hDC) {
		TextOut(hDC, left, top + 10, (label + textLine).c_str(), strlen((label + textLine).c_str()));
	}
};

constexpr auto MAX_LOADSTRING = 100;
constexpr auto LINES_COUNTER = 10;

// Глобальные переменные:
HINSTANCE hInst;                                //	текущий экземпляр
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  //	Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            //	имя класса главного 
TextBox inputLine;								//	Строка для поиска
TextBox statusLine;								//	Статус поиска
TextBox lines[LINES_COUNTER];					//	Строки
TextBox indexes[LINES_COUNTER];					//	Порядковые номера строк

// Прототипы:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, HWND, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void PaintLine(HWND, HDC, TextBox);
RECT InitRect(int, int, int, int);
void SearchLine();
std::string LeftTrim(std::string);
std::string RightTrim(std::string);
std::string Trim(std::string);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINAPILAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	inputLine = TextBox(InitRect(400, 400, 200, 40), "Search line: ", "");
	statusLine = TextBox(InitRect(400, 500, 200, 40), "Status: ", "");
	
	for (int i = 0; i < LINES_COUNTER; ++i) {
		indexes[i] = TextBox(InitRect(inputLine.left + inputLine.width() + 50, (i + 1) * 40, 20, 40), "", "");
		lines[i] = TextBox(InitRect(indexes[i].left + indexes[i].width(), (i + 1) * 40, 200, 40), std::to_string(i+1) + ". ", "");
	}
	lines[0].textLine = "So if you";
	lines[1].textLine = "Love me";
	lines[2].textLine = "Let me go";
	lines[3].textLine = "And run away";
	lines[4].textLine = "Before I know.";
	lines[5].textLine = "My heart is ";
	lines[6].textLine = "Just to dark";
	lines[7].textLine = "To care";
	lines[8].textLine = "I can't destroy";
	lines[9].textLine = "What is not there.";	

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, hWnd, nCmdShow))
	{
		return FALSE;
	}

	HWND button = CreateWindow((LPCSTR)"button", (LPCSTR)"Find", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		inputLine.left, inputLine.bottom, 100, inputLine.height(), hWnd, (HMENU)BTN_CLICK, hInstance, NULL);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPILAB4));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINAPILAB4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPILAB4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, HWND hwnd, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::string defaultText = "Type message here:";

	RECT rectForClear;
	GetClientRect(hWnd, &rectForClear);

	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
			inputLine.textLine += "<";
			break;

		case VK_RIGHT:
			inputLine.textLine += ">";
			break;
		}
		InvalidateRect(hWnd, &rectForClear, true);
		UpdateWindow(hWnd);
	}
	break;
	case WM_CHAR:
	{
		switch (wParam) {
		case 0x08:
			/* Обработка BACKSPACE . */
			if (inputLine.textLine.length() > 0)
				inputLine.textLine = inputLine.textLine.substr(0, inputLine.textLine.length() - 1);
			break;
		case 0x0A:

			/* Обработка перевода строки. */
			inputLine.textLine += "\n";
			break;
		case 0x1B:

			/* Обработка Esc. */
			inputLine.textLine = "";
			break;
		case 0x09:

			/* Обработка табуляции TAB. */
			inputLine.textLine += " tab ";
			break;
		case 0x0D:

			/* Обработка перевода каретки. */
			inputLine.textLine += "\r";
			break;
		default:

			/* Обработка воспроизводимых символов. */
			inputLine.textLine += char(wParam);
			std::string tmp = inputLine.textLine;
			break;
		}
		InvalidateRect(hWnd, &rectForClear, true);
		UpdateWindow(hWnd);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case BTN_CLICK:
			SearchLine();
			inputLine.textLine = "";
			InvalidateRect(hWnd, &rectForClear, true);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		PaintLine(hWnd, hdc, inputLine);
		PaintLine(hWnd, hdc, statusLine);
		for (int i = 0; i < LINES_COUNTER; ++i) {
			PaintLine(hWnd, hdc, lines[i]);
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void PaintLine(HWND hWnd, HDC hDC, TextBox line) {
	line.Print(hDC);
}
RECT InitRect(int x, int y, int width, int height) {
	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;
	return rect;
}

void SearchLine() {
	std::string searchLine = Trim(inputLine.textLine);
	for (int i = 0; i < LINES_COUNTER; ++i) {
		for (int j = 0; j < searchLine.length(); ++j) {
			if (searchLine[j] != lines[i].textLine[j]) {
				break;
			}
			if (j == searchLine.length() - 1) {
				statusLine.textLine = "Найдено: " + std::to_string(i + 1) + " строка";
				return;
			}
			
		}
		statusLine.textLine = "Совпадений не найдено";
	}
}

// trim from both ends (in place)
std::string Trim(std::string s) {
	return LeftTrim(RightTrim(s));
}

// trim from start (in place)
std::string LeftTrim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
	return s;
}

// trim from end (in place)
std::string RightTrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
	return s;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
