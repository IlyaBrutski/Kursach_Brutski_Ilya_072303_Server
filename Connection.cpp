#include "Connection.h"

void Connection::acept() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "System --> Ошибка загрузки библиотеки" << endl;
		exit(1);
	}
	int SizeOfAddr = sizeof(addr);
	this->addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->addr.sin_port = htons(1111);
	this->addr.sin_family = AF_INET;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&this->addr, sizeof(this->addr));
	listen(sListen, SOMAXCONN);
	cout << "System --> Прослушивание сокета" << endl;
	this->connection = accept(sListen, (SOCKADDR*)&addr, &SizeOfAddr);
	if (this->connection == 0) {
		cout << "System --> Ошибка подключения" << endl;
		exit(1);
	}
	else cout << "System --> Клиент успешно подключился" << endl;
}

void Connection::SendStr(const char* buf) {
	int msg_size = strlen(buf);
	send(this->connection, (char*)&msg_size, sizeof(int), NULL);
	send(this->connection, buf, msg_size, NULL);
	cout << "Сервер --> " << buf << endl;
}
void Connection::RecvStr(char*& data) {
	int msg_size = 0;
	recv(this->connection, (char*)&msg_size, sizeof(int), NULL);
	char* msg = new char[msg_size + 1]; msg[msg_size] = '\0';
	recv(this->connection, msg, msg_size, NULL);
	strcpy(data, msg); delete[] msg;
	cout << "Клиент --> " << data << endl;
}

void Connection::SendInt(int buf) {
	char* str = new char[20]; itoa(buf, str, 10);
	int msg_size = strlen(str);
	send(this->connection, (char*)&msg_size, sizeof(int), NULL);
	send(this->connection, str, msg_size, NULL);
	delete[] str;
	cout << "Сервер --> " << buf << endl;
}
int Connection::RecvInt() {
	int msg_size = 0;
	recv(this->connection, (char*)&msg_size, sizeof(int), NULL);
	char* msg = new char[msg_size + 1]; msg[msg_size] = '\0';
	recv(this->connection, msg, msg_size, NULL);
	int buf = atoi(msg); delete[] msg;
	cout << "Клиент --> " << buf << endl;
	return buf;
}

void Connection::SendDouble(double buf) {
	char* str = new char[50]; sprintf(str, "%.2f", buf);
	int msg_size = strlen(str);
	send(this->connection, (char*)&msg_size, sizeof(int), NULL);
	send(this->connection, str, msg_size, NULL);
	delete[] str; cout << "Сервер --> " << buf << endl;
}
double Connection::RecvDouble() {
	int msg_size = 0;
	recv(this->connection, (char*)&msg_size, sizeof(int), NULL);
	char* msg = new char[msg_size + 1]; msg[msg_size] = '\0';
	recv(this->connection, msg, msg_size, NULL);
	double buf = atof(msg); delete[] msg;
	cout << "Клиент --> " << buf << endl;
	return buf;
}

