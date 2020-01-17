// Client.cpp : Defines the entry point for the console application.
//

#include "Client.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object
CWinApp theApp;
string user;
string* resp = new string[4];
int exitapp = 0;

DWORD WINAPI ThreadListen(LPVOID cl)
{
	int MsgSize;
	char *mess;
	CSocket* client = (CSocket*)cl;
	do {
		// Nhap thong diep tu server
		client->Receive((char*)&MsgSize, sizeof(MsgSize), 0);
		mess = new char[MsgSize + 1];
		if (MsgSize != 0)
		{
			client->Receive((char*)mess, MsgSize, 0);
			mess[MsgSize] = '\0';
			// In thong diep va kiem tra co thoat khong
			cout << mess << endl;
			int temp = isSame(mess, "Server ngat ke noi");
			if (temp)
			{
				exitapp = 1;
			}
		}
		delete[] mess;
	} while (exitapp != 1);
	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	resp[0] = "Sai mat khau";
	resp[1] = "Dang nhap thanh cong";
	resp[2] = "Dang dang nhap o may khac";
	resp[3] = "Tao tai khoan thanh cong";
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.

		unsigned int port = 4000;
		AfxSocketInit(NULL);
		sockaddr sockadd;
		// Tao socket
		CSocket client;
		client.Create();

		// Connect
		if (client.Connect(_T("127.0.0.1"), port))
		{
			int Size = 0;
			string pass;
			while (1)
			{
				// Nhap username va password
				user = "";
				pass = "";
				cout << "username: ";
				getline(cin, user);
				cout << "password: ";
				getline(cin, pass);

				// Kiem tra hop le
				std::size_t found1 = user.find(' ');
				std::size_t found2 = pass.find(' ');

				if (found1 != std::string::npos || found2 != std::string::npos)
				{
					cout << "username va password khong duoc co khoang trang\n";
					continue;
				}
				if (user == "" || pass == "")
				{
					cout << "username va password khong duoc de trong\n";
					continue;
				}

				// Gui den server
				Size = user.length();
				client.Send(&Size, sizeof(Size), 0);
				client.Send(&user, Size, 0);

				Size = pass.length();
				client.Send(&Size, sizeof(Size), 0);
				client.Send(&pass, Size, 0);

				// Nhap ket qua va in thong bao
				client.Receive((char*)&Size, sizeof(Size), 0);
				// Thanh cong thi tiep tuc
				if (Size == 1)
				{
					cout << resp[Size] << endl;
					break;
				}
				// That bai nhap lai
				else
				{
					cout << resp[Size] << endl;
					continue;
				}
			}

			// Tao thread lang nghe tu server
			DWORD dwThreadListen;
			HANDLE hThreadListen;
			hThreadListen = CreateThread(NULL, 0, ThreadListen, (LPVOID)&client, 0, &dwThreadListen);
			
			// Gui cac thong diep lien server
			char Msg[100];
			int MsgSize;
			char *temp;
			do {
				string mess;
				int len = 0;

				// Nhap thong diep
				cin.getline(Msg, 100);

				// Kiem tra server da thoat chua
				if (exitapp == 1) break;
				MsgSize = strlen(Msg);
				
				// Neu muon thoat
				if (isSame(Msg, "exit"))
				{
					// Thoat thread lang nghe
					TerminateThread(hThreadListen, dwThreadListen);
					// Huy cong viec dang lam va gui thong diep
					client.CancelBlockingCall();
					MsgSize = -1;
					client.Send(&MsgSize, sizeof(MsgSize), 0);;
					cout << "Da ngat ket noi" << endl;
					exitapp = 1;
					_getch();
					break;
				}
				else if (MsgSize != 0)
				{
					// Thoat thread lang nghe
					TerminateThread(hThreadListen, dwThreadListen);
					// Huy cong viec dang lam va gui thong diep
					client.CancelBlockingCall();
					char* mess = makeMess(user, Msg);
					MsgSize = strlen(mess);
					client.Send(&MsgSize, sizeof(MsgSize), 0);
					client.Send(mess, MsgSize, 0);
					hThreadListen = CreateThread(NULL, 0, ThreadListen, (LPVOID)&client, 0, &dwThreadListen);
				}
			} while (exitapp != 1);
		}
		else
		{
			cout << "Khong the ket noi den Server !!!" << endl;
		}

	// Dong ket noi
	client.Close();
	} 
}
