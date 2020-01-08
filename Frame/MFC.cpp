#include "MY.h" // 原本包含mfc.h 就好，但为了CMyWinApp 的定义，所以...
extern CMyWinApp theApp;

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

BOOL CFrameWnd::Create()
{

    CreateEx();
    return TRUE;
}

BOOL CFrameWnd::PreCreateWindow()
{

    return TRUE;
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
