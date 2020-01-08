#pragma once
#include <iostream>
#include "mfc.h"

using namespace std;

class CMyWinApp : public CWinApp
{
public:
    CMyWinApp() {}
    ~CMyWinApp() {}

    virtual BOOL CMyWinApp::InitInstance();
};

class CMyFrameWnd : public CFrameWnd
{
    DECLARE_DYNAMIC(CMyFrameWnd) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyFrameWnd();
    ~CMyFrameWnd() {}
};

class CMyDoc : public CDocument
{
    DECLARE_DYNAMIC(CMyDoc) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyDoc() {}
    ~CMyDoc() {}
};

class CMyView : public CView
{
    DECLARE_DYNAMIC(CMyView) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyView() {}
    ~CMyView() {}
};
