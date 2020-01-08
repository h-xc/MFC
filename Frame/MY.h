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
public:
    CMyFrameWnd();
    ~CMyFrameWnd() {}
};
// global function
void PrintAllClasses();