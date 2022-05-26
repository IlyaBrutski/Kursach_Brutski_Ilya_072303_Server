#include "User.h"

const char* User::AdminFile = "FileAdmin.txt";
const char* User::UsersFile = "FileUsers.txt";
const char* User::LoginUserFile = "FileLoginUser.txt";

User::User() : surname(0), name(0), password(0), login(0) {
	cout << "System --> Конструктор по умолчанию User" << endl;
}
User::User(char* surname, char* name, char* login, char* password) {
	cdat.copyStr(this->surname, surname);
	cdat.copyStr(this->name, name);
	cdat.copyStr(this->login, login);
	cdat.copyStr(this->password, password);
	cout << "System --> Конструктор с параметрами User" << endl;
}
User::User(const User& obj) {
	cdat.copyStr(this->surname, obj.surname);
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->login, obj.login);
	cdat.copyStr(this->password, obj.password);
	cout << "System --> Конструктор копирования User" << endl;
}

vector<User> User::readUsers() {
	cout << "System --> Чтение пользователей" << endl;
	vector<User> user;
	ifstream fin(UsersFile);
	char* surname = new char[100];
	char* name = new char[100];
	char* login = new char[100];
	char* password = new char[100];
	while (fin >> surname >> name >> login >> password) {
		for (int i = 0; login[i] != '\0'; i++)
			login[i] -= 7;
		for (int j = 0; password[j] != '\0'; j++)
			password[j] -= 7;
		User obj(surname, name, login, password);
		user.push_back(obj);
	}
	delete[] surname; delete[] name;
	delete[] login; delete[] password;
	fin.close();
	return user;
}
void User::writeUsers(vector<User>& user) {
	cout << "System --> Запись пользователей" << endl;
	ofstream fout(UsersFile, ios_base::trunc);
	char* login = new char[100];
	char* password = new char[100];
	for (vector<User>::iterator p = user.begin(); p != user.end(); p++) {
		strcpy(login, p->getLogin());
		for (int i = 0; login[i] != '\0'; i++) login[i] += 7;
		strcpy(password, p->getPassword());
		for (int j = 0; password[j] != '\0'; j++) password[j] += 7;
		fout << p->getSurname() << " " << p->getName() << " " << login << " " << password << endl;
	}
	delete[] login; delete[] password;
	fout.close();
}
bool User::registration(const char* FileName, Connection& connect) {
	cout << "System --> Регистрация" << endl;
	char* surname = new char[100];
	char* name = new char[100];
	char* login = new char[100];
	char* password = new char[100];
	char* password_2 = new char[100];

	cdat.setStr(surname, connect);
	cdat.setStr(name, connect);

	int k = 0;
	vector<User> user = readUsers();

	do {
		k = 0;
		cdat.setStr(login, connect);
		if (strlen(login) < 5) k = 1;
		for (int i = 0; i < user.size(); i++) {
			if (strcmp(login, user[i].getLogin()) == 0) {
				k = 2;
				break;
			}
		}
		connect.SendInt(k);
	} while (k != 0);
	do {
		k = 0;
		cdat.setStr(password, connect);
		if (strlen(password) < 5) {
			k = 3;
			connect.SendInt(k);
			continue;
		}
		else connect.SendInt(k);
		cdat.setStr(password_2, connect);
		if (strcmp(password, password_2) != 0) {
			k = 4;
			connect.SendInt(k);
			continue;
		}
		else connect.SendInt(k);
	} while (k != 0);

	User obj(surname, name, login, password);
	user.push_back(obj);

	delete[] surname; delete[] name;
	delete[] login; delete[] password;  delete[] password_2;

	writeUsers(user);
	return true;
}
bool User::authorization(const char* FileName, Connection& connect) {
	cout << "System --> Авторизация" << endl;
	char* login = new char[100];
	char* password = new char[100];
	char* file_surname = new char[100];
	char* file_name = new char[100];
	char* file_login = new char[100];
	char* file_password = new char[100];

	int k = -1;
	bool result = false;

	ifstream file(FileName);

	while (k != 0) {
		k = -1;
		do {
			cdat.setStr(login, connect);
			if (strlen(login) < 5) connect.SendInt(1);
			else connect.SendInt(-1);
		} while (strlen(login) < 5);
		k = -1;
		do {
			cdat.setStr(password, connect);
			if (strlen(password) < 5) connect.SendInt(1);
			else connect.SendInt(-1);
		} while (strlen(password) < 5);
		for (int i = 0; login[i] != '\0'; i++) login[i] += 7;
		for (int i = 0; password[i] != '\0'; i++) password[i] += 7;
		file.clear();
		file.seekg(0, file.beg);
		while (!file.eof()) {
			file >> file_surname >> file_name >> file_login >> file_password;
			if (strcmp(login, file_login) == 0 && strcmp(password, file_password) == 0) {
				k = 0;
				result = true;
				ofstream fout(LoginUserFile, ios_base::trunc);
				fout << file_surname << " " << file_name << " " << file_login << endl;
				fout.close();
				break;
			}
		}
		connect.SendInt(k);

		if (result == false) {
			int choice = cdat.check_int_interval(0, 1, connect);
			switch (choice) {
			case 1: continue;
				break;
			case 0:
				result = false;
				k = 0;
				break;
			}
		}
	}
	file.close();
	delete[] file_surname; delete[] file_name;
	delete[] file_login; delete[] file_password;
	delete[] login; delete[] password;
	return result;
}
void User::viewUser(Connection& connect) {
	cout << "System --> Просмотр пользователей" << endl;
	vector<User> user = readUsers();
	char* str = new char[100];
	connect.SendInt(user.size());
	for (vector<User>::iterator p = user.begin(); p != user.end(); p++) {
		connect.SendStr(p->getSurname());
		connect.SendStr(p->getName());
		connect.SendStr(p->getLogin());
		connect.SendStr(p->getPassword());
	}
	delete[] str;
}
void User::deleteUser(Connection& connect) {
	cout << "System --> Удаление пользователя" << endl;
	vector<User> user = readUsers();
	int choice;
	do {
		viewUser(connect);
		choice = cdat.check_int_interval(0, user.size(), connect);
		if (choice != 0) {
			int del = cdat.check_int_interval(0, 1, connect);
			if (del == 1) {
				user.erase(user.cbegin() + choice - 1);
				writeUsers(user);
			}
		}
	} while (choice != 0);
}

void User::operator=(const User& obj) {
	delete[] this->surname; delete[] this->name;
	delete[] this->login; delete[] this->password;
	cdat.copyStr(this->surname, obj.surname);
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->login, obj.login);
	cdat.copyStr(this->password, obj.password);
}

User::~User() {
	delete[] surname;
	delete[] name;
	delete[] login;
	delete[] password;
	cout << "System --> Деструктор User" << endl;
}
