//---------------------------------------------------------------
// 编译联结: 请参考generic.mak
// Generic - Win32 程序的基础写法
// Top Studio * J.J.Hou
// 档名: generic.c
// 作者: 侯俊杰
//---------------------------------------------------------------

#include <windows.h>  // 每一个 Windows 程序都需要包含此档
#include "resource.h" // 內含各个 resource IDs
#include "generic.h"  // 本程序之含入档
#include "stdio.h"  // 本程序之含入档

HINSTANCE _hInst; // Instance handle
HWND _hWnd;
char _szAppName[] = "Generic";                  //
char _szTitle[] = "Generic Sample Application"; //

// 消息映射
struct MSGMAP_ENTRY
{
  UINT nMessage;
  LONG(*pfn)
  (HWND, UINT, WPARAM, LPARAM);
};
#define dim(x) (sizeof(x) / sizeof(x[0]))
// 消息与处理例程之对照表格
LONG OnAbout(HWND hWnd, UINT message,
             WPARAM wParam, LPARAM lParam)
{
  DialogBox(_hInst,
            "AboutBox",
            hWnd,
            (DLGPROC)About);
  return 0;
}
LONG OnExit(HWND hWnd, UINT message,
            WPARAM wParam, LPARAM lParam)
{
  DestroyWindow(hWnd);
  return 0;
}
LONG OnDestroy(HWND hWnd, UINT message,
               WPARAM wParam, LPARAM lParam)
{
  PostQuitMessage(0);
  return 0;
}
LONG OnCommand(HWND hWnd, UINT message,
               WPARAM wParam, LPARAM lParam);
struct MSGMAP_ENTRY _messageEntries[] =
    {
       // {WM_CREATE,OnCreate},
       // {WM_PAINT,OnPaint},
       // {WM_SIZE,OnSize},
       {WM_COMMAND, OnCommand},
       // {WM_SETFOCUS, OnSetFocus},
       // {WM_CLOSE,OnClose},
       {WM_DESTROY,OnDestroy},
};

// Command-ID与处理例程之对照表格
struct MSGMAP_ENTRY _commandEntries[] =
    {
        {IDM_ABOUT,OnAbout},
        {IDM_EXIT,OnExit},
        //{IDM_FILEOPEN,OnFileOpen},
        //{IDM_SAVEAS,OnSaveAs},
};

//---------------------------------------------------------------
// WinMain -main是c程序的进入点，WinMain则是Windows的进入点
//---------------------------------------------------------------
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
  MSG msg;

  UNREFERENCED_PARAMETER(lpCmdLine); //

  //这里分两次来初始化窗口，是因为之前win版本决定希望一个窗口类型注册一次，可以在多个实例中使用。
  if (!hPrevInstance)
    if (!InitApplication(hInstance))
      return (FALSE);

  if (!InitInstance(hInstance, nCmdShow))
    return (FALSE);

  AttachConsole(ATTACH_PARENT_PROCESS); // 将当前程序附着到父进程上，因为是从控制台启动的，所以当前程序的父进程就是那个控制台。
  freopen_s(0,"CONIN$", "r+t", stdin); // 重定向 STDIN
  freopen_s(0,"CONOUT$", "w+t", stdout); // 重定向STDOUT

  // 消息循环
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg); // 转换键盘消息
    DispatchMessage(&msg);  // 分派消息
  }
  return (msg.wParam); //
}
//---------------------------------------------------------------
// InitApplication -窗口类型注册
//---------------------------------------------------------------
BOOL InitApplication(HINSTANCE hInstance)
{
  WNDCLASS wc;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = (WNDPROC)WndProc; //
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, "jjhouricon");
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject(WHITE_BRUSH); //
  wc.lpszMenuName = "GenericMenu";                // .RC
  wc.lpszClassName = _szAppName;
  return (RegisterClass(&wc));
}
//---------------------------------------------------------------
// InitInstance -窗口诞生
//---------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  _hInst = hInstance; //

  _hWnd = CreateWindow(
      _szAppName,
      _szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  if (!_hWnd)
    return (FALSE);

  ShowWindow(_hWnd, nCmdShow); //
  UpdateWindow(_hWnd);         // 送出 WM_PAINT
  return (TRUE);
}
//---------------------------------------------------------------
// WndProc -窗口函数
//---------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
  int i;
  for (i = 0; i < dim(_messageEntries); i++)
  { //
    if (message == _messageEntries[i].nMessage)
    {
       return ((*_messageEntries[i].pfn)(hWnd, message, wParam, lParam));
    } 
  }
  return (DefWindowProc(hWnd, message, wParam, lParam));
}
//----------------------------------------------------------------------
// OnCommand --专门处理 WM_COMMAND
//----------------------------------------------------------------------
LONG OnCommand(HWND hWnd, UINT message,
               WPARAM wParam, LPARAM lParam)
{
  int i;
  for (i = 0; i < dim(_commandEntries); i++)
  { //
    if (LOWORD(wParam) == _commandEntries[i].nMessage)
      return ((*_commandEntries[i].pfn)(hWnd, message, wParam, lParam));
  }
  return (DefWindowProc(hWnd, message, wParam, lParam));
}



//---------------------------------------------------------------
// About -对话框函数
//---------------------------------------------------------------
LRESULT CALLBACK About(HWND hDlg, UINT message,
                       WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam); // 避免編译时的警告

  switch (message)
  {
  case WM_INITDIALOG:
    return (TRUE); // TRUE 表示我已处理过这个消息

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, TRUE);
      return (TRUE); // TRUE 表示我已处理过这个消息
    }
    break;
  }
  return (FALSE); // FALSE 表示我没有处理这个消息
}


