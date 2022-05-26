#pragma once
#include "Connection.h"
#include "Check.h"
#include "User.h"
#include "Supplier.h"

class Opinion : public User, public Supplier {
private:
	char* surname;
	char* name;
	char* log;
	int count;
	int** array;
public:
	static const char* OpinionFile;
	static const char* LogFile;

	Opinion();
	Opinion(char*, char*, char*, int, int**);
	Opinion(const Opinion&);

	char* getSurname() { return this->surname; }
	char* getName() { return this->name; }
	char* getLog() { return this->log; }
	int getCount() { return this->count; }
	int** getArray() { return this->array; }

	vector<Opinion> readOpinion();
	void writeOpinion(vector<Opinion>&);
	void addOpinion(Connection&);
	void viewOpinion(Connection&);
	void findAlternative(Connection&);

	void operator=(const Opinion&);

	~Opinion();
};