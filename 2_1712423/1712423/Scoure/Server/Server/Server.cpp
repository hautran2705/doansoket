// Server.cpp : Defines the entry point for the console application.
//

#include "Server.h"
#include "stdafx.h"
#include "afxsock.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;
sockcheck * client = new sockcheck[100];
int i = readuser(client);
CSocket* sockClient = new CSocket[100];
int update = 0;
int exitapp = 0;

// Thread lang nghe moi client
DWORD WINAPI ThreadListen(LPVOID temp)
{
	AfxSocketInit();
	int t = *((int*)temp);
	int t1, t2 = 0, t3 = 0;
	int MsgSize;
	char *ServerMsg;
	string name, pass;
	do {
		int len = 0;
		// Lay username va password
		sockClient[t].Receive((char*)&t1, sizeof(int), 0);
		sockClient[t].Receive(&name, t1, 0);
		sockClient[t].Receive((char*)&t1, sizeof(int), 0);
		sockClient[t].Receive(&pass, t1, 0);
		
		// Kiem tra va gui ket qua
		int check = checklogin(name, pass, client, i, t);
		if (check == 3) update = 1;
		sockClient[t].Send(&check, sizeof(check), 0);
		
		// Dang nhap thanh cong
		if (check == 1)
		{
			// Tao chuoi thong bao
			ServerMsg = makeMess(name," da dang nhap");
			t3 = strlen(ServerMsg);

			// Gui thong bao den nhung client khac
			for (int j = 0; j < i; j++)
			{
				int point = client[j].getsignin();
				if (point != -1 && point != t)
				{
					sockClient[point].CancelBlockingCall();
					sockClient[point].Send(&t3, sizeof(t3), 0);
					sockClient[point].Send(ServerMsg, t3, 0);
				}
			}

			// In thong bao tai server
			cout << ServerMsg << endl;
			break;
		}
		if (check == 3) update = 1;
	} while (1);
	do
	{
		// Nhan thong diep
		t3 = 0;
		sockClient[t].Receive((char*)&t3, sizeof(int), 0);
		if (t3==-1)
		{
			// Client thoat
			ServerMsg = makeMess(name, " da dang xuat");
			t3 = strlen(ServerMsg);

			// Gui thong bao den nhung client khac
			for (int j = 0; j < i; j++)
			{
				int point = client[j].getsignin();
				if (point != -1 && point != t)
				{
					sockClient[point].CancelBlockingCall();
					sockClient[point].Send(&t3, sizeof(t3), 0);
					sockClient[point].Send(ServerMsg, t3, 0);
				}
				else if (point == t) client[j].setsignin(-1);
			}

			// In thong bao len server
			cout << ServerMsg << endl;
			break;
		}
		else
		{
			ServerMsg = new char[t3 + 1];
			if (t3 != 0)
			{
				sockClient[t].Receive((char*)ServerMsg, t3, 0);
				// In thong diep
				ServerMsg[t3] = '\0';
				MsgSize = strlen(ServerMsg);
				cout << ServerMsg << endl;

				// Gui thong diep den client khac
				for (int j = 0; j < i; j++)
				{
					int point = client[j].getsignin();
					if (point != -1 && point != t)
					{
						sockClient[point].CancelBlockingCall();
						sockClient[point].Send(&t3, sizeof(t3), 0);
						sockClient[point].Send(ServerMsg, t3, 0);
					}
				}
			}
		}
		delete[] ServerMsg;
	} while (exitapp != 27);
	sockClient[t].Close();
	return 0;
}

DWORD WINAPI ThreadSignin(LPVOID sv)
{
	AfxSocketInit();
	CSocket* server = (CSocket*)sv;
	for (int j = 0; j < 100; j++)
	{
		// Tao thread cho moi client
		if (server->Accept(sockClient[j]))
		{
			int temp = j;
			DWORD dwThreadListen;
			HANDLE hThreadListen;
			hThreadListen = CreateThread(NULL, 0, ThreadListen, (LPVOID)&temp, 0, &dwThreadListen);
		}
	} 
	while (exitapp!=27)
	{
		
	}
	return 0;
}

DWORD WINAPI ThreadExit(LPVOID sv)
{
	AfxSocketInit();
	// Nghe lenh thoat (esc)
	exitapp = _getch();
	if (exitapp == 27)
	{
		char *ServerMsg;
		int len;
		ServerMsg = makeMess("", "Server ngat ke noi");
		len = strlen(ServerMsg);
		// Gui thong bao den tat ca client
		for (int j = 0; j < i; j++)
		{
			int point = client[j].getsignin();
			if (point != -1)
			{
				sockClient[point].CancelBlockingCall();
				sockClient[point].Send(&len, sizeof(len), 0);
				sockClient[point].Send(ServerMsg, len, 0);
				sockClient[point].Close();
			}
		}
		cout << ServerMsg << endl;
	}
	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	CSocket server;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		
		// Khoi tao thu vien Socket
		if (AfxSocketInit() == FALSE)
		{
			cout << "Khong the khoi tao Socket Libraray";
			return FALSE;
		}
		else
		{
			unsigned int port = 4000;
			AfxSocketInit(NULL);

			server.Create(port);
			server.Listen(5);

			// Goi thread cho client ket noi
			DWORD dwThreadSignin;
			HANDLE hThreadSignin;
			hThreadSignin = CreateThread(NULL, 0, ThreadSignin, (LPVOID)&server, 0, &dwThreadSignin);

			DWORD dwThreadExit;
			HANDLE hThreadExit;
			hThreadExit = CreateThread(NULL, 0, ThreadExit, 0, 0, &dwThreadExit);

			while (exitapp!=27)
			{
				// Co tai khoan moi
				if (update == 1)
				{
					// Dung thread
					SuspendThread(hThreadSignin);
					SuspendThread(hThreadExit);
					// Cap nhat lai du lieu
					i = readuser(client);
					update = 0;
					// Chay lai thread
					ResumeThread(hThreadSignin);
					ResumeThread(hThreadExit);
				}
			}
		}

		// Dong ket noi
		server.Close();
	}

	return nRetCode;
}
