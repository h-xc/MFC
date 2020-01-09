#include "MY.h" // 原本包含mfc.h 就好，但为了CMyWinApp 的定义，所以...
extern CMyWinApp theApp;

extern void printlpEntries(AFX_MSGMAP_ENTRY *lpEntry);

BOOL CCmdTarget::OnCmdMsg(UINT nID, int nCode)
{
    // Now look through message map to see if it applies to us
    AFX_MSGMAP *pMessageMap;
    AFX_MSGMAP_ENTRY *lpEntry;
    for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
         pMessageMap = pMessageMap->pBaseMessageMap)
    {
        lpEntry = pMessageMap->lpEntries;
        printlpEntries(lpEntry);
    }

    return FALSE; // not handled
}

// in implementation file
static char szCObject[] = "CObject";
struct CRuntimeClass CObject::classCObject =
    {szCObject, sizeof(CObject), 0xffff, NULL, NULL};
static AFX_CLASSINIT _init_CObject(&CObject::classCObject);
CRuntimeClass *CObject::GetRuntimeClass() const
{
    return &CObject::classCObject;
}
CRuntimeClass *CRuntimeClass::pFirstClass = NULL;

AFX_CLASSINIT::AFX_CLASSINIT(CRuntimeClass *pNewClass) // 标准的串行队列添加方式
{
    pNewClass->m_pNextClass = CRuntimeClass::pFirstClass;
    CRuntimeClass::pFirstClass = pNewClass;
}

BOOL CObject::IsKindOf(const CRuntimeClass *pClass) const
{
    CRuntimeClass *pClassThis = GetRuntimeClass();
    while (pClassThis != NULL)
    {
        if (pClassThis == pClass)
            return TRUE;
        pClassThis = pClassThis->m_pBaseClass;
    }
    return FALSE; // walked to the top, no match
}

CObject *CRuntimeClass::CreateObject()
{
    if (m_pfnCreateObject == NULL)
    {
        TRACE1("Error: Trying to create object which is not "
               "DECLARE_DYNCREATE \nor DECLARE_SERIAL: %hs.\n",
               m_lpszClassName);
        return NULL;
    }
    CObject *pObject = NULL;
    pObject = (*m_pfnCreateObject)();
    return pObject;
}

CRuntimeClass *PASCAL CRuntimeClass::Load()
{
    char szClassName[64];
    CRuntimeClass *pClass;
    // JJHOU : instead of Load from file, we Load from cin.
    cout << "enter a class name... ";
    cin >> szClassName;
    for (pClass = pFirstClass; pClass != NULL; pClass = pClass->m_pNextClass)
    {
        if (strcmp(szClassName, pClass->m_lpszClassName) == 0)
            return pClass;
    }
    TRACE1("Error: Class not found: %s \n", szClassName);
    return NULL; // not found
}

BOOL CWnd::Create()
{
    return TRUE;
}

BOOL CWnd::CreateEx()
{
    PreCreateWindow();
    return TRUE;
}

BOOL CWnd::PreCreateWindow()
{
    return TRUE;
}

LRESULT CWnd::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    AFX_MSGMAP *pMessageMap;
    AFX_MSGMAP_ENTRY *lpEntry;

    if (nMsg == WM_COMMAND) // special case for commands
    {
        if (OnCommand(wParam, lParam))
            return 1L; // command handled
        else
            return (LRESULT)DefWindowProc(nMsg, wParam, lParam);
    }

    pMessageMap = GetMessageMap();

    for (; pMessageMap != NULL;
         pMessageMap = pMessageMap->pBaseMessageMap)
    {
        lpEntry = pMessageMap->lpEntries;
        printlpEntries(lpEntry);
    }
    return 0; // add by JJHou. if find, should call lpEntry->pfn,
    // otherwise should call DefWindowProc.
}
LRESULT CWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}

BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // ...
    return OnCmdMsg(0, 0);
}

BOOL CFrameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // ...
    // route as normal command
    return CWnd::OnCommand(wParam, lParam);
}


BOOL CFrameWnd::Create()
{
    CreateEx();
    return TRUE;
}

BOOL CFrameWnd::PreCreateWindow()
{
    return TRUE;
}

CView *CFrameWnd::GetActiveView() const
{
    return m_pViewActive;
}

BOOL CFrameWnd::OnCmdMsg(UINT nID, int nCode)
{
    // pump through current view FIRST
    CView *pView = GetActiveView();
    if (pView->OnCmdMsg(nID, nCode))
        return TRUE;

    // then pump through frame
    if (CWnd::OnCmdMsg(nID, nCode))
        return TRUE;
    // last but not least, pump through app
    CWinApp *pApp = AfxGetApp();
    if (pApp->OnCmdMsg(nID, nCode))
        return TRUE;

    return FALSE;
}

BOOL CDocument::OnCmdMsg(UINT nID, int nCode)
{
    if (CCmdTarget::OnCmdMsg(nID, nCode))
        return TRUE;
    return FALSE;
}

BOOL CView::OnCmdMsg(UINT nID, int nCode)
{
    if (CWnd::OnCmdMsg(nID, nCode))
        return TRUE;

    BOOL bHandled = FALSE;
    bHandled = m_pDocument->OnCmdMsg(nID, nCode);
    return bHandled;
}

IMPLEMENT_DYNAMIC(CCmdTarget, CObject)
IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget)
IMPLEMENT_DYNAMIC(CWinApp, CWinThread)
IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd)
IMPLEMENT_DYNAMIC(CDocument, CCmdTarget)
IMPLEMENT_DYNAMIC(CView, CWnd)

CWinApp *AfxGetApp()
{
    return theApp.m_pCurrentWinApp;
}

AFX_MSGMAP *CCmdTarget::GetMessageMap() const
{
    return &CCmdTarget::messageMap;
}

AFX_MSGMAP CCmdTarget::messageMap =
    {
        NULL,
        &CCmdTarget::_messageEntries[0]};

AFX_MSGMAP_ENTRY CCmdTarget::_messageEntries[] =
    {
        // { 0, 0, 0, 0, AfxSig_end, 0 } // nothing here
        {0, 0, CCmdTargetid, 0, AfxSig_end, 0}

};

BEGIN_MESSAGE_MAP(CWnd, CCmdTarget)
ON_COMMAND(CWndid, 0)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFrameWnd, CWnd)
ON_COMMAND(CFrameWndid, 0)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CDocument, CCmdTarget)
ON_COMMAND(CDocumentid, 0)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CView, CWnd)
ON_COMMAND(CViewid, 0)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CWinApp, CCmdTarget)
ON_COMMAND(CWinAppid, 0)
END_MESSAGE_MAP()

LRESULT AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam,
                   CWnd *pWnd) // last parameter pWnd is added by JJHou.
{
    //...
    return AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

LRESULT AfxCallWndProc(CWnd *pWnd, HWND hWnd, UINT nMsg,
                       WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = pWnd->WindowProc(nMsg, wParam, lParam);
    return lResult;
}
