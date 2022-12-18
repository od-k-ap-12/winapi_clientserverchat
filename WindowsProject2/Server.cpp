#include "framework.h"
#include "WindowsProject2.h"
#include <iostream>
#include <string>
#include "WinSock2.h" //здесь находятся объявления, необходимые
//для Winsock 2 API.
#include <ws2tcpip.h> //содержит функции для работы с адресами
 //напр. inet_pton
#pragma comment(lib, "Ws2_32.lib") //линкуем библиотеку Windows Sockets
//#define _UNICODE
//#define UNICODE
using namespace std;


#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

const int MAXSTRLEN = 255;
WSADATA wsaData;//структура для хранения информацию
                //о инициализации сокетов
SOCKET _socket; //дескриптор слушающего сокета
SOCKET acceptSocket;//дескриптор сокета, который связан с клиентом 
sockaddr_in addr; //локальный адрес и порт
HWND hIP, hReceive, hSend;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_SERVER), NULL, (DLGPROC)DlgProc);
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
		hSend = GetDlgItem(hWnd, IDC_SENDMESSAGE); 
		hReceive = GetDlgItem(hWnd, IDC_RECEIVEMESSAGE);
        return TRUE;
    }

    case WM_COMMAND: {
		if (LOWORD(wp) == IDC_START) {
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			addr.sin_family = AF_INET;

			inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

			addr.sin_port = htons(20000);

			bind(_socket, (SOCKADDR*)&addr, sizeof(addr));

			listen(_socket, 1);

			acceptSocket = accept(_socket, NULL, NULL);
		}
		else if (LOWORD(wp) == IDC_STOP) {
			closesocket(acceptSocket);
			closesocket(_socket);
			WSACleanup();
			system("pause");
		}
		else if (LOWORD(wp) == IDC_SEND) {
			//const char Text[200]="Hello World";
			char Text[200];
			GetWindowText(hSend, (LPWSTR)Text, 100);
			send(acceptSocket, Text, strlen(Text), 0);
		}
		else if (LOWORD(wp) == IDC_RECIEVE) {
			char buf[MAXSTRLEN];
			wstring wbuf;
			int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
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
