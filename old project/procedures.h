#include <winsock.h>
//Dialog procedures
BOOL CALLBACK DlgProc(HWND hdwnd, UINT Message, WPARAM wParam, LPARAM
  lParam);
BOOL CALLBACK ConnectDlgProc(HWND hdwnd, UINT Message, WPARAM wParam, LPARAM
  lParam);
BOOL CALLBACK ListenDlgProc(HWND hdwnd, UINT Message, WPARAM wParam, LPARAM
  lParam);
    
int TryConnect(long hostname, int PortNo);
int ListenOnPort(int PortNo);

char Title[] = "ChatMate";

HINSTANCE hInst = NULL;
HWND hwnd, hStatus;

SOCKET s;
SOCKADDR_IN from;
int fromlen = sizeof(from);