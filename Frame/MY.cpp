#include "my.h"

CMyWinApp theApp; // global object

BOOL CMyWinApp::InitInstance()
{
    cout << "CMyWinApp::InitInstance \n";
    m_pMainWnd = new CMyFrameWnd;
    return TRUE;
}

CMyFrameWnd::CMyFrameWnd()
{
    cout << "CMyFrameWnd::CMyFrameWnd \n";
    Create();
}

//---------------------------------------------------------------
// main
//---------------------------------------------------------------
void main()
{
    cout << "main \n";
    CWinApp *pApp = AfxGetApp();
    pApp->InitApplication(); //调用的是CWinApp::InitApplication
    pApp->InitInstance();    //调用的是CMyWinApp::InitInstance（因为CMyWinApp 改写它了），
    pApp->Run();             //调用的是CWinApp::
}