#include "framework.h"
#include "WindowsProject2.h"
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
SOCKET acceptSocket;
sockaddr_in addr;
HWND hIP, hReceive, hSend,hReceiveButton,hSendButton,hStopButton;

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
		hSendButton = GetDlgItem(hWnd, IDC_SEND);
		hReceiveButton = GetDlgItem(hWnd, IDC_RECEIVE);
		hStopButton = GetDlgItem(hWnd, IDC_STOP);
		EnableWindow(hSend, FALSE);
		EnableWindow(hReceive, FALSE);
		EnableWindow(hSendButton, FALSE);
		EnableWindow(hReceiveButton, FALSE);
		EnableWindow(hStopButton, FALSE);
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
			EnableWindow(hSend, TRUE);
			EnableWindow(hReceive, TRUE);
			EnableWindow(hSendButton, TRUE);
			EnableWindow(hReceiveButton, TRUE);
			EnableWindow(hStopButton, TRUE);
		}
		else if (LOWORD(wp) == IDC_STOP) {
			closesocket(acceptSocket);
			closesocket(_socket);
			WSACleanup();
			system("pause");
		}
		else if (LOWORD(wp) == IDC_SEND) {
			char Text[200];
			wstring wbuf;
			GetWindowText(hSend, (LPWSTR)wbuf.data(), wbuf.size());
			WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), wbuf.size(), Text, wbuf.length(), NULL, NULL);
			send(acceptSocket, Text, strlen(Text), 0);
		}
		else if (LOWORD(wp) == IDC_RECIEVE) {
			char buf[MAXSTRLEN];
			wstring wbuf;
			int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
			buf[i] = '\0';

			//wbuf.resize(MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0));
			MultiByteToWideChar(CP_UTF8, 0, buf, -1, (LPWSTR)wbuf.data(), wbuf.size());
			if (strcmp(buf, "buy") == 0) {
				const char Products[100] = "\t 1.Телефон-7800 грн\n2.Планшет-9000 грн\n3.Ноутбук-26000 грн\nВведите номер желаемого продукта.";
				send(acceptSocket, Products, strlen(Products), 0);

				int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
				buf[i] = '\0';
				int Number = atoi(buf);
				if (Number > 0 && Number <= 3) {
					const char Products[40] = "\nВведите кол-во желаемого продукта.";
					send(acceptSocket, Products, strlen(Products), 0);
					i = recv(acceptSocket, buf, MAXSTRLEN, 0);
					buf[i] = '\0';
					int Quantity = atoi(buf);
					int Price;
					switch (Number) {
					case 1: {
					Price = 7800 * Quantity;
						break;
					}
					case 2: {
					Price = 9000 * Quantity;
					break;
					}
					case 3: {
					Price = 26000 * Quantity;
					break;
					}
						  char Final[20];
				     itoa(Price,Final,0);
					 send(acceptSocket, Final, strlen(Final), 0);
					}
			}
			SetWindowText(hReceive, wbuf.c_str());
		}

    }
    return TRUE;
    }
    return FALSE;
}



