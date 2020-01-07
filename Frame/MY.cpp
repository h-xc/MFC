#include "my.h"

CMyWinApp theApp; // global object
//---------------------------------------------------------------
// main
//---------------------------------------------------------------
void main()
{
    cout << "main \n";
    CWinApp *pApp = AfxGetApp();
}