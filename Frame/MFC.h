#pragma once

#define TRUE 1
#define FALSE 0

typedef char *LPSTR;
typedef const char *LPCSTR;

typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef int INT;
typedef unsigned int UINT;
typedef long LONG;

typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;
typedef int HWND;

#define PASCAL
#define TRACE1 printf

#define WM_COMMAND 0x0111
#define WM_CREATE 0x0001
#define WM_PAINT 0x000F
#define WM_NOTIFY 0x004E

#define CObjectid 0xffff
#define   CCmdTargetid 1
#define     CWinThreadid 11
#define       CWinAppid 111
#define         CMyWinAppid 1111
#define  CWndid 12
#define    CFrameWndid 121
#define      CMyFrameWndid 1211
#define  CViewid 122
#define    CMyViewid 1221
#define  CDocumentid 13
#define    CMyDocid 131

#include <stdio.h>
#include <string.h>
#include <iostream>

// Message map signature values and macros in separate header
#include "AFXMSG_.h"
/////////////////////////////////////////////////////////////////
// Window message map handling
struct AFX_MSGMAP_ENTRY; // declared below after CWnd

struct AFX_MSGMAP
{
    AFX_MSGMAP *pBaseMessageMap;
    AFX_MSGMAP_ENTRY *lpEntries;
};

#define DECLARE_MESSAGE_MAP()                  \
    static AFX_MSGMAP_ENTRY _messageEntries[]; \
    static AFX_MSGMAP messageMap;              \
    virtual AFX_MSGMAP *GetMessageMap() const;

#define BEGIN_MESSAGE_MAP(theClass, baseClass)              \
    AFX_MSGMAP *theClass::GetMessageMap() const             \
    {                                                       \
        return &theClass::messageMap;                       \
    }                                                       \
    AFX_MSGMAP theClass::messageMap =                       \
        {&(baseClass::messageMap),                          \
         (AFX_MSGMAP_ENTRY *)&(theClass::_messageEntries)}; \
    AFX_MSGMAP_ENTRY theClass::_messageEntries[] =          \
        {

#define END_MESSAGE_MAP()                   \
           {                                       \
               0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 \
           }                                       \
        };



// 类别型录结构体
class CObject;
struct CRuntimeClass
{
    // Attributes
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;                         // schema number of the loaded class
    CObject *(PASCAL *m_pfnCreateObject)(); // NULL => abstract class
    CRuntimeClass *m_pBaseClass;

    CObject *CreateObject();
    static CRuntimeClass *PASCAL Load();

    // CRuntimeClass objects linked together in simple list
    static CRuntimeClass *pFirstClass; // start of class list
    CRuntimeClass *m_pNextClass;       // linked list of registered classes
};

// 类别型录（也就是各个CRuntimeClass 对象）定义，放在类里面
#define DECLARE_DYNAMIC(class_name)         \
public:                                     \
    static CRuntimeClass class##class_name; \
    virtual CRuntimeClass *GetRuntimeClass() const;

// 带动态生成的类别型录
#define DECLARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name)       \
    static CObject *PASCAL CreateObject();

// 一个结构体，定义了虚构函数，将传入的类别型录插入到串行队列中
struct AFX_CLASSINIT
{
    AFX_CLASSINIT(CRuntimeClass *pNewClass);
};

// 获取到类别型录的指针
#define RUNTIME_CLASS(class_name) \
    (&class_name::class##class_name)

//
#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
    static char _lpsz##class_name[] = #class_name;                            \
    CRuntimeClass class_name::class##class_name = {                           \
        _lpsz##class_name, sizeof(class_name), wSchema, pfnNew,               \
        RUNTIME_CLASS(base_class_name), NULL};                                \
    static AFX_CLASSINIT _init_##class_name(&class_name::class##class_name);  \
    CRuntimeClass *class_name::GetRuntimeClass() const                        \
    {                                                                         \
        return &class_name::class##class_name;                                \
    }

// 类别型录的内容指定以及串接工作
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)

// 带动态生成的类别型录的内容指定以及串接工作
#define IMPLEMENT_DYNCREATE(class_name, base_class_name)         \
    CObject *PASCAL class_name::CreateObject()                   \
    {                                                            \
        return new class_name;                                   \
    }                                                            \
    _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
                            class_name::CreateObject)

using namespace std;
class CObject
{
public:
    CObject::CObject() {}
    CObject::~CObject() {}

public:
    virtual CRuntimeClass *GetRuntimeClass() const;
    static CRuntimeClass classCObject;
    BOOL IsKindOf(const CRuntimeClass *pClass) const;

    virtual void SayHello() { cout << "Hello CObject \n"; }
};

class CCmdTarget : public CObject
{
    DECLARE_DYNAMIC(CCmdTarget)
public:
    CCmdTarget::CCmdTarget() {}
    CCmdTarget::~CCmdTarget() {}

    virtual BOOL OnCmdMsg(UINT nID, int nCode);
    DECLARE_MESSAGE_MAP() // base class - no {{ }} macros
};
typedef void (CCmdTarget::*AFX_PMSG)(void);

struct AFX_MSGMAP_ENTRY // MFC 4.0
{
    UINT nMessage; // windows message
    UINT nCode;    // control code or WM_NOTIFY code
    UINT nID;      // control ID (or 0 for windows messages)
    UINT nLastID;  // used for entries specifying a range of control id's
    UINT nSig;     // signature type (action) or pointer to message #
    AFX_PMSG pfn;  // routine to call (or special value)
};

class CWinThread : public CCmdTarget
{
    DECLARE_DYNAMIC(CWinThread)
public:
    CWinThread::CWinThread() {}
    CWinThread::~CWinThread() {}

    virtual BOOL InitInstance()
    {
        return TRUE;
    }
    virtual int Run()
    {
        return 1;
    }
};

class CWnd;
class CWinApp : public CWinThread
{
    DECLARE_DYNAMIC(CWinApp)
public:
    CWinApp *m_pCurrentWinApp;
    CWnd *m_pMainWnd;

public:
    CWinApp::CWinApp()
    {
        m_pCurrentWinApp = this;
    }
    CWinApp::~CWinApp() {}

    virtual BOOL InitApplication()
    {
        return TRUE;
    }
    virtual BOOL InitInstance()
    {
        return TRUE;
    }
    virtual int Run()
    {
        return CWinThread::Run();
    }
    DECLARE_MESSAGE_MAP()
};
typedef void (CWnd::*AFX_PMSGW)(void); // like 'AFX_PMSG' but for CWnd derived classes only

class CDocument : public CCmdTarget
{
    DECLARE_DYNAMIC(CDocument)
public:
    CDocument::CDocument() {}
    CDocument::~CDocument() {}

    virtual BOOL OnCmdMsg(UINT nID, int nCode);
    DECLARE_MESSAGE_MAP()
};

class CWnd : public CCmdTarget
{
    DECLARE_DYNCREATE(CWnd)
public:
    CWnd::CWnd() { cout << "CWnd Constructor \n"; }
    CWnd::~CWnd() {}
    virtual BOOL Create();
    BOOL CreateEx();
    virtual BOOL PreCreateWindow();
    void SayHello() { cout << "Hello CWnd \n"; }

    virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};


// base Cwnd
class CView;
class CFrameWnd : public CWnd
{
    DECLARE_DYNCREATE(CFrameWnd)
public:
    CView *m_pViewActive; // current active view

public:
    CFrameWnd::CFrameWnd() { cout << "CFrameWnd Constructor \n"; }
    CFrameWnd::~CFrameWnd() {}
    BOOL Create();
    CView* GetActiveView() const;
    virtual BOOL PreCreateWindow();
    void SayHello() { cout << "Hello CFrameWnd \n"; }

    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCmdMsg(UINT nID, int nCode);

    DECLARE_MESSAGE_MAP()
    friend CView;
};

class CView : public CWnd
{
    DECLARE_DYNAMIC(CView)
public:
    CDocument* m_pDocument;
public:
    CView::CView() {}
    CView::~CView() {}

    virtual BOOL OnCmdMsg(UINT nID, int nCode);
    DECLARE_MESSAGE_MAP()
    friend CFrameWnd;
};

// global function
CWinApp *AfxGetApp();

LRESULT AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam,
                   CWnd *pWnd); // last param. pWnd is added by JJHOU.
LRESULT AfxCallWndProc(CWnd *pWnd, HWND hWnd, UINT nMsg, WPARAM wParam,
                       LPARAM lParam);