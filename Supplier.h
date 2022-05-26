#pragma once
#include "Connection.h"
#include "Check.h"
#include <algorithm>

class Supplier {
private:
	char* name;
	char* type;
	char* supplier;
	double price;
	int number;
	int day;
	int fail;
	double transportation;
public:
	static const char* SupplierFile;
	static const char* OpinionFile;

	Supplier();
	Supplier(char*, char*, char*, double, int, int, int, double);
	Supplier(const Supplier& obj);

	char* getName() { return this->name; }
	char* getType() { return this->type; }
	char* getSupplier() { return this->supplier; }
	double getPrice() { return this->price; }
	int getNumber() { return this->number; }
	int getDay() { return this->day; }
	int getFail() { return this->fail; }
	double getTransportation() { return this->transportation; }

	void setName(char*);
	void setType(char*);
	void setSupplier(char*);
	void setPrice(double);
	void setNumber(int);
	void setDay(int);
	void setFail(int);
	void setTransportation(double);

	vector<Supplier> readSupplier();
	void writeSupplier(vector<Supplier>&);
	void typeOfGoods(char*&, Connection&);
	void addSupplier(Connection&);
	void viewSupplier(Connection&);
	void redactSupplier(Connection&);
	void deleteSupplier(Connection&);
	void searchSupplier(Connection&);
	void sortSupplier(Connection&);
	void filtrSupplier(Connection&);

	template <typename  T>
	void selection_sort(T* a, T& b, int left, int right, int(*pfun_compare));

	void operator=(const Supplier& obj);

	~Supplier();
};