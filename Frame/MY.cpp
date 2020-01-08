#include "my.h"

CMyWinApp theApp; // global object

BOOL CMyWinApp::InitInstance()
{
    m_pMainWnd = new CMyFrameWnd;
    return TRUE;
}

CMyFrameWnd::CMyFrameWnd()
{
    Create();
}

//---------------------------------------------------------------
// main
//---------------------------------------------------------------
void main()
{
    CWinApp *pApp = AfxGetApp();
    pApp->InitApplication(); //调用的是CWinApp::InitApplication
    pApp->InitInstance();    //调用的是CMyWinApp::InitInstance（因为CMyWinApp 改写它了），
    pApp->Run();             //调用的是CWinApp::

    CMyDoc *pMyDoc = new CMyDoc;
    CMyView *pMyView = new CMyView;
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CMyDoc));     // 應該獲得 TRUE
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CDocument));  // 應該獲得 TRUE
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CCmdTarget)); // 應該獲得 TRUE
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CObject));    // 應該獲得 TRUE
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CWinApp));    // 應該獲得 FALSE
    cout << pMyDoc->IsKindOf(RUNTIME_CLASS(CView));      // 應該獲得 FALSE
    cout << pMyView->IsKindOf(RUNTIME_CLASS(CView));     // 應該獲得 TRUE
    cout << pMyView->IsKindOf(RUNTIME_CLASS(CObject));   // 應該獲得 TRUE
    cout << pMyView->IsKindOf(RUNTIME_CLASS(CWnd));      // 應該獲得 TRUE
    cout << pMyView->IsKindOf(RUNTIME_CLASS(CFrameWnd)); // 应该获得FALSE
}

IMPLEMENT_DYNAMIC(CMyFrameWnd, CFrameWnd) //

IMPLEMENT_DYNAMIC(CMyDoc, CDocument) //

IMPLEMENT_DYNAMIC(CMyView, CView)