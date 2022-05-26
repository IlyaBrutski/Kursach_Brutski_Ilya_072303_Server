#define _CRT_SECURE_NO_WARNINGS
#pragma once
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Connection {
private:
	SOCKET connection;
	SOCKADDR_IN addr;
public:
	void acept();

	void SendStr(const char*);
	void RecvStr(char*&);

	void SendInt(int);
	int RecvInt();

	void SendDouble(double);
	double RecvDouble();
};