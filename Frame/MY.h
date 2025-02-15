#pragma once
#include <iostream>
#include "MFC.h"

using namespace std;

class CMyWinApp : public CWinApp
{
public:
    CMyWinApp() {}
    ~CMyWinApp() {}

    virtual BOOL CMyWinApp::InitInstance();
    DECLARE_MESSAGE_MAP()
};

class CMyFrameWnd : public CFrameWnd
{
    DECLARE_DYNCREATE(CMyFrameWnd) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyFrameWnd();
    ~CMyFrameWnd() {}
    void SayHello() { cout << "Hello CMyFrameWnd \n"; }
    DECLARE_MESSAGE_MAP()
};

class CMyDoc : public CDocument
{
    DECLARE_DYNCREATE(CMyDoc) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyDoc() {cout << "CMyDoc Constructor \n";}
    ~CMyDoc() {}
    void SayHello() { cout << "Hello CMyDoc \n"; }
    DECLARE_MESSAGE_MAP()
};

class CMyView : public CView
{
    DECLARE_DYNCREATE(CMyView) // 在MFC 程序中这里其实是DECLARE_DYNCREATE()
public:
    CMyView() {cout << "CMyView Constructor \n";}
    ~CMyView() {}
    void SayHello() { cout << "Hello CMyView \n"; }
    DECLARE_MESSAGE_MAP()
};
// global function
void AfxPrintAllClasses();