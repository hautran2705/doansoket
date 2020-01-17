#pragma once
#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"

#include <iostream>
#include <string>
using namespace std;

// Kiem tra chuoi co giong nhau khong
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

// Tao chuoi char tu chuoi string va char
char* makeMess(string us, char* mess)
{
	string temp = us.c_str();
	temp = temp + ": ";
	int len = temp.length();
	int i;
	int lenmess = strlen(mess);
	char* kq = new char[len + lenmess + 1];
	for (i = 0; i < len; i++)
	{
		kq[i] = temp.at(i);
	}
	for (; i < lenmess + len; i++)
	{
		kq[i] = mess[i-len];
	}
	kq[i] = '\0';
	return kq;
}