#pragma once
#include <iostream>
//#include <Windows.h>
#define BOOL int
#define TRUE 1
#define FALSE 0

typedef char *LPSTR;
#define LPCSTR LPSTR

#define UINT int
#define PASCAL _stdcal

// 类别型录结构体
class CObject;
struct CRuntimeClass
{
    // Attributes
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;                  // schema number of the loaded class
    CObject *(*m_pfnCreateObject)(); // NULL => abstract class
    CRuntimeClass *m_pBaseClass;

    // CRuntimeClass objects linked together in simple list
    static CRuntimeClass *pFirstClass; // start of class list
    CRuntimeClass *m_pNextClass;       // linked list of registered classes
};

// 类别型录（也就是各个CRuntimeClass 对象）定义，放在类里面
#define DECLARE_DYNAMIC(class_name)         \
public:                                     \
    static CRuntimeClass class##class_name; \
    virtual CRuntimeClass *GetRuntimeClass() const;

// 一个结构体，定义了虚构函数，函数将传入的类别型录插入到串行队列中
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

using namespace std;
class CObject
{
public:
    CObject::CObject() {}
    CObject::~CObject() {}

public:
    virtual CRuntimeClass *GetRuntimeClass() const;
    static CRuntimeClass classCObject;
};

class CCmdTarget : public CObject
{
    DECLARE_DYNAMIC(CCmdTarget)
public:
    CCmdTarget::CCmdTarget() {}
    CCmdTarget::~CCmdTarget() {}
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
};

class CDocument : public CCmdTarget
{
    DECLARE_DYNAMIC(CDocument)
public:
    CDocument::CDocument() {}
    CDocument::~CDocument() {}
};

class CWnd : public CCmdTarget
{
    DECLARE_DYNAMIC(CWnd)
public:
    CWnd::CWnd() {}
    CWnd::~CWnd() {}
    virtual BOOL Create();
    BOOL CreateEx();
    virtual BOOL PreCreateWindow();
};

// base Cwnd
class CFrameWnd : public CWnd
{
    DECLARE_DYNAMIC(CFrameWnd)
public:
    CFrameWnd::CFrameWnd() {}
    CFrameWnd::~CFrameWnd() {}
    BOOL Create();
    virtual BOOL PreCreateWindow();
};

class CView : public CWnd
{
    DECLARE_DYNAMIC(CView)
public:
    CView::CView() {}
    CView::~CView() {}
};

// global function
CWinApp *AfxGetApp();