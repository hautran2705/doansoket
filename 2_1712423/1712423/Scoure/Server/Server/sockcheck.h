#pragma once
#include <iostream>
#include <string>
using namespace std;

class sockcheck
{
private:
	string username;
	string password;
	int signin;
public:
	
	// construction
	sockcheck() {
		username = "";
		password = "";
		signin = -1;
	}
	~sockcheck() {}

	// getter
	string getusername() { return username; }
	string getpassword() { return password; }
	int getsignin() { return signin; }

	// setter
	void setusername(string name) { username = name; }
	void setpassword(string pass) { password = pass; }
	void setsignin(int check) { signin = check; }
};

