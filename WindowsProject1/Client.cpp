#include "framework.h"
#include "WindowsProject1.h"
#include <iostream>
#include <string>
#include "WinSock2.h" //здесь находятся объявления, необходимые
//для Winsock 2 API.
#include <ws2tcpip.h> //содержит функции для работы с адресами
 //напр. inet_pton
#pragma comment(lib, "Ws2_32.lib") //линкуем библиотеку Windows Sockets
#define UNICODE
#define _UNICODE

using namespace std;


#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

const int MAXSTRLEN = 255;
WSADATA wsaData;//структура для хранения информацию
                //о инициализации сокетов
SOCKET _socket; //дескриптор сокета
sockaddr_in addr; //локальный адрес и порт
HWND hIP,hReceive,hSend;

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
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        //создаем потоковый сокет, протокол TCP
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        //Семейство адресов IPv4
        addr.sin_family = AF_INET; 
        return TRUE;
    }

    case WM_COMMAND: {
        if (LOWORD(wp) == IDC_CONNECT) {
            const char buffer[40]{};
            GetWindowText(hIP, (LPWSTR)buffer, 20);
            inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
            addr.sin_port = htons(20000);
            connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
        }
        else if (LOWORD(wp) == IDC_SEND) {
            //const char Text[200]="Hello World";
            char Text[200];
            wstring wbuf;
            GetWindowText(hSend, (LPWSTR)wbuf.data(), wbuf.size());
            send(_socket, wbuf.c_str(), strlen(Text), 0);

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

            wbuf.resize(MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0));
            MultiByteToWideChar(CP_UTF8, 0, buf, -1, (LPWSTR)wbuf.data(), wbuf.size());
            SetWindowText(hReceive, wbuf.c_str());
        }

    }
        return TRUE;
    }
    return FALSE;
}
