#pragma once
#include "Connection.h"
#include "Check.h"

class User {
private:
	char* surname;
	char* name;
	char* login;
	char* password;
public:
	static const char* AdminFile;
	static const char* UsersFile;
	static const char* LoginUserFile;

	User();
	User(char*, char*, char*, char*);
	User(const User&);

	char* getSurname() { return this->surname; }
	char* getName() { return this->name; }
	char* getLogin() { return this->login; }
	char* getPassword() { return this->password; }

	static vector<User> readUsers();
	void writeUsers(vector<User>&);
	bool registration(const char*, Connection&);
	bool authorization(const char*, Connection&);
	void viewUser(Connection&);
	void deleteUser(Connection&);

	void operator=(const User&);

	~User();
};