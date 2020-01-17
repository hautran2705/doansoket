#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "sockcheck.h"

using namespace std;

// Doc file
int readuser(sockcheck* A)
{
	fstream fin;
	fin.open("user.txt");
	int i = 0;
	if (fin.is_open())
		while (!fin.eof())
		{
			string p1, p2;
			getline(fin, p1, ' ');
			getline(fin, p2);
			A[i].setusername(p1);
			A[i].setpassword(p2);
			i++;
		}
	fin.close();
	return i;
}

// Ghi vao file
void writeuser(sockcheck* A, int& n, string _a, string _b)
{
	ofstream fout;
	fout.open("user.txt");
	int i = 0;
	while (i < n)
	{
		string name = A[i].getusername();
		string pass = A[i].getpassword();
		if (name != "" && pass != "")
		{
			if (i != 0) fout << endl;
			fout << name << " " << pass;
		}
		i++;
	}
	if (_a != ""&&_b != "")
	{
		fout << endl << _a << " " << _b;
		n++;
	}
	fout.close();
}

// check cac truong hop
int checklogin(string _name, string _pass, sockcheck* A, int& n, int i)
{
	string a = _name.c_str();
	string b = _pass.c_str();
	if (a != "" && b != "")
	{
		for (int j = 0; j < n; j++)
		{
			string name = A[j].getusername().c_str();
			string pass = A[j].getpassword().c_str();
			int sign = A[j].getsignin();
			if (a == name)
			{
				if (b == pass)
				{
					// Dang nhap thanh cong
					if (sign == -1)
					{
						A[j].setsignin(i);
						return 1;
					}
					// Da dang nhap
					return 2;
				}
				// Sai mat khau
				return 0;
			}
		}
		// Tao tai khoan
		writeuser(A, n, a, b);
		return 3;
	}
	return -1;
}

// Kiem tra chuoi giong nhau
int isSame(char* ch, string st)
{
	int i = strlen(ch);
	if (i == st.length())
	{
		for (int j = 0; j < i; j++)
		{
			if (ch[j] != st.at(j)) return 0;
		}
		return 1;
	}
	return 0;
}

// Tao chuoi char bang 2 chuoi string
char* makeMess(string us, string mess)
{
	string temp = us.c_str();
	temp = temp + mess.c_str();
	int len = temp.length();

	int i;
	char* kq = new char[len + 1];
	for (i = 0; i < len; i++)
	{
		kq[i] = temp.at(i);
	}
	kq[len] = '\0';
	return kq;
}