#include "my.h" // 原本包含mfc.h 就好，但为了CMyWinApp 的定义，所以...

extern CMyWinApp theApp;
CWinApp *AfxGetApp()
{
    return theApp.m_pCurrentWinApp;
}