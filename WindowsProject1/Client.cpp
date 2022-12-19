#include "framework.h"
#include "WindowsProject1.h"
#include <iostream>
#include <string>
#include "WinSock2.h"
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib")
using namespace std;


#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
sockaddr_in addr;
HWND hIP,hReceive,hSend,hSendButton,hReceiveButton,hStopButton;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_CLIENT), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{
    TCHAR str[100];

    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;

    case  WM_INITDIALOG: {
        hIP = GetDlgItem(hWnd, IDC_IP);
        hSend = GetDlgItem(hWnd, IDC_SENDMESSAGE);
        hReceive = GetDlgItem(hWnd, IDC_RECEIVEMESSAGE);
        hSendButton = GetDlgItem(hWnd, IDC_SEND);
        hReceiveButton = GetDlgItem(hWnd, IDC_RECEIVE);
        hStopButton= GetDlgItem(hWnd, IDC_STOP);
        EnableWindow(hSend, FALSE);
        EnableWindow(hReceive, FALSE);
        EnableWindow(hSendButton, FALSE);
        EnableWindow(hReceiveButton, FALSE);
        EnableWindow(hStopButton, FALSE);
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        addr.sin_family = AF_INET; 
        return TRUE;
    }

    case WM_COMMAND: {
        if (LOWORD(wp) == IDC_CONNECT) {

            char IP[200];
            wstring wbuf;
            GetWindowText(hIP, (LPWSTR)wbuf.data(), wbuf.size());
            WideCharToMultiByte(CP_ACP, 0, wbuf.data(), wbuf.size(), IP, wbuf.length(), 0, 0);

            inet_pton(AF_INET, IP, &addr.sin_addr);
            addr.sin_port = htons(20000);
            connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
            EnableWindow(hSend, TRUE);
            EnableWindow(hReceive, TRUE);
            EnableWindow(hSendButton, TRUE);
            EnableWindow(hReceiveButton, TRUE);
            EnableWindow(hStopButton, TRUE);
        }
        else if (LOWORD(wp) == IDC_SEND) {
            //const char Text[200] = "text";
            char Text[200];
            wstring wbuf;
            GetWindowText(hSend, (LPWSTR)wbuf.data(), wbuf.size());
            WideCharToMultiByte(CP_ACP, 0, wbuf.data(), wbuf.size(), Text, wbuf.length(), 0, 0);
            send(_socket, Text, strlen(Text), 0);

        }
        else if (LOWORD(wp) == IDC_STOP) {
            closesocket(_socket);
            WSACleanup();
        }
        else if (LOWORD(wp) == IDC_RECIEVE) {
            char buf[MAXSTRLEN];
            wstring wbuf;
            int i = recv(_socket, buf, MAXSTRLEN, 0);
            buf[i] = '\0';

            //wbuf.resize(MultiByteToWideChar(CP_UTF8, 0, buf, -1, 0, 0));
            MultiByteToWideChar(CP_ACP, 0, buf, -1, (LPWSTR)wbuf.data(), wbuf.size());
            SetWindowText(hReceive, wbuf.c_str());
        }

    }
        return TRUE;
    }
    return FALSE;
}
