#include "Check.h"

bool Check::checkFileEOF(const char* FileName) {
	fstream file(FileName);
	if (!file.is_open())  cout << endl << " Файл не открылся\n";
	else if (file.peek() != EOF) return true;
	return false;
}

void Check::setStr(char*& buf, Connection& connect) {
	char* str = new char[100];
	connect.RecvStr(str);
	for (int i = 0; str[i] != '\0'; i++)
		if (str[i] == ' ')
			str[i] = '_';
	copyStr(buf, str);
	delete[] str;
}
void Check::copyStr(char*& data_1, const char* data_2) {
	try {
		data_1 = new char[strlen(data_2) + 1];
	}
	catch (bad_alloc err) {
		cout << " Ошибка выделения памяти" << endl;
		exit(1);
	}
	strcpy(data_1, data_2);
}

int Check::check_int(Connection& connect) {
	int x, k = 0;
	char* l = new char[100];
	setStr(l, connect);
	for (int i = 0; l[i] != '\0'; i++) {
		if (isdigit(l[i]))
			k++;
	}
	if (strlen(l) != k || (l[0] == '0' && strlen(l) > 1))
		x = -1;
	else
		x = atoi(l);
	delete[] l;
	return x;
}
double Check::check_double(Connection& connect) {
	cout << "check_double " << endl;
	int k = 0, z = 0;
	double x;
	char* l = new char[100];
	setStr(l, connect);
	for (int i = 0; l[i] != '\0'; i++) {
		if (isdigit(l[i]))
			k++;
		if (l[i] == ',')
			z++;
	}
	if (strlen(l) != (k + z) || (l[0] == '0' && l[1] == '0') || l[0] == ',' || z > 1)
		x = -1;
	else x = atof(l);
	delete[] l;
	return x;
}

int Check::check_int_interval(int min, int max, Connection& connect) {
	connect.SendInt(min);
	connect.SendInt(max);
	int x = check_int(connect);
	connect.SendInt(x);
	while (x < min || x > max) {
		x = check_int(connect);
		connect.SendInt(x);
	}
	return x;
}
double Check::check_double_interval(double min, double max, Connection& connect) {
	cout << "check_double_interval " << endl;
	connect.SendDouble(min);
	connect.SendDouble(max);
	double x = check_double(connect);
	connect.SendDouble(x);
	while (x < min || x > max) {
		x = check_double(connect);
		connect.SendDouble(x);
	}
	return x;
}