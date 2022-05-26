#include "Supplier.h"

const char* Supplier::SupplierFile = "FileSupplier.txt";
const char* Supplier::OpinionFile = "FileOpinion.txt";

Supplier::Supplier() : name(0), type(0), supplier(0), price(0), number(0), day(0), fail(0), transportation(0) {
	cout << "System --> Конструктор по умолчанию Supplier" << endl;
}
Supplier::Supplier(char* name, char* type, char* supplier, double price, int number, int day, int fail, double transportation) {
	cdat.copyStr(this->name, name);
	cdat.copyStr(this->type, type);
	cdat.copyStr(this->supplier, supplier);
	this->price = price;
	this->number = number;
	this->day = day;
	this->fail = fail;
	this->transportation = transportation;
	cout << "System --> Коструктор с параметрами Supplier" << endl;
}
Supplier::Supplier(const Supplier& obj) {
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->type, obj.type);
	cdat.copyStr(this->supplier, obj.supplier);
	this->price = obj.price;
	this->number = obj.number;
	this->day = obj.day;
	this->fail = obj.fail;
	this->transportation = obj.transportation;
	cout << "System --> Коструктор копирования Supplier" << endl;
}

bool compName(Supplier& a, Supplier& b) {
	return a.getName() > b.getName();
};
bool compType(Supplier& a, Supplier& b) { return a.getType() < b.getType(); };
bool compSupplier(Supplier& a, Supplier& b) { return a.getSupplier() < b.getSupplier(); };
bool compPrice(Supplier& a, Supplier& b) { return a.getPrice() < b.getPrice(); };
bool compNumber(Supplier& a, Supplier& b) { return a.getNumber() < b.getNumber(); };
bool compDay(Supplier& a, Supplier& b) { return a.getDay() < b.getDay(); };
bool compFail(Supplier& a, Supplier& b) { return a.getFail() < b.getFail(); };
bool compTransportation(Supplier& a, Supplier& b) { return a.getTransportation() < b.getTransportation(); };


void Supplier::setName(char* name) {
	delete[] this->name;
	cdat.copyStr(this->name, name);
}
void Supplier::setType(char* type) {
	delete[] this->type;
	cdat.copyStr(this->type, type);
}
void Supplier::setSupplier(char* supplier) {
	delete[] this->supplier;
	cdat.copyStr(this->supplier, supplier);
}
void Supplier::setPrice(double price) {
	this->price = price;
}
void Supplier::setNumber(int number) {
	this->number = number;
}
void Supplier::setDay(int day) {
	this->day = day;
}
void Supplier::setFail(int fail) {
	this->fail = fail;
}
void Supplier::setTransportation(double transportation) {
	this->transportation = transportation;
}


vector<Supplier> Supplier::readSupplier() {
	cout << "System --> Чтение поставщиков" << endl;
	vector<Supplier> suppl;
	ifstream file(SupplierFile, ios_base::in);
	char* name = new char[100];
	char* type = new char[100];
	char* supplier = new char[100];
	double price;
	int number;
	int day;
	int fail;
	double tranportation;
	while (file >> name >> type >> supplier >> price >> number >> day >> fail >> tranportation) {
		Supplier obj(name, type, supplier, price, number, day, fail, tranportation);
		suppl.push_back(obj);
	}
	delete[] name; delete[] type; delete[] supplier;
	file.close();
	return suppl;
}
void Supplier::writeSupplier(vector<Supplier>& suppl) {
	cout << "System --> Запись поставщиков" << endl;
	if (suppl.size() > 0) {
		ofstream file(SupplierFile, ios_base::trunc);
		for (vector<Supplier>::iterator p = suppl.begin(); p != suppl.end(); p++) {
			file << p->getName() << " "
				<< p->getType() << " "
				<< p->getSupplier() << " "
				<< p->getPrice() << " "
				<< p->getNumber() << " "
				<< p->getDay() << " "
				<< p->getFail() << " "
				<< p->getTransportation() << endl;
		}
		file.close();
	}
}
void Supplier::typeOfGoods(char*& type, Connection& connect) {
	int choice = cdat.check_int_interval(1, 7, connect);
	switch (choice) {
	case 1: strcpy(type, "Электротехника"); break;
	case 2: strcpy(type, "Одежда"); break;
	case 3: strcpy(type, "Мебель"); break;
	case 4: strcpy(type, "Продукты"); break;
	case 5: strcpy(type, "Стройматериалы"); break;
	case 6: strcpy(type, "Аксессуары"); break;
	default: strcpy(type, "Другое"); break;
	}
}
void Supplier::addSupplier(Connection& connect) {
	cout << "System --> Добавление поставщика" << endl;
	vector<Supplier> suppl = readSupplier();

	int k = -1, choice = -1;
	if (!Check::checkFileEOF(OpinionFile)) k = 0;
	else k = 1;

	connect.SendInt(k);
	if (k > 0) {
		choice = cdat.check_int_interval(0, 1, connect);
		if (choice == 1) {
			ofstream file(OpinionFile, ios_base::trunc);
			file.close();
		}
	}
	if (choice != 0) {
		char* name = new char[256];
		char* type = new char[100];
		char* supplier = new char[100];
		double price;
		int number;
		int day;
		int fail;
		double transportation;

		cdat.setStr(name, connect);
		typeOfGoods(type, connect);
		cdat.setStr(supplier, connect);
		price = cdat.check_double_interval(0.01, 100000.00, connect);
		number = cdat.check_int_interval(1, 100000, connect);
		day = cdat.check_int_interval(1, 120, connect);
		fail = cdat.check_int_interval(0, 100, connect);
		transportation = cdat.check_double_interval(0.00, 100000.00, connect);

		Supplier obj(name, type, supplier, price, number, day, fail, transportation);
		suppl.push_back(obj);

		delete[] name; delete[] type; delete[] supplier;
		writeSupplier(suppl);
	}
}
void Supplier::viewSupplier(Connection& connect) {
	cout << "System --> Просмотр поставщиков" << endl;
	vector<Supplier> suppl;
	suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		for (vector<Supplier>::iterator p = suppl.begin(); p != suppl.end(); p++) {
			connect.SendStr(p->getName());
			connect.SendStr(p->getType());
			connect.SendStr(p->getSupplier());
			connect.SendDouble(p->getPrice());
			connect.SendInt(p->getNumber());
			connect.SendInt(p->getDay());
			connect.SendInt(p->getFail());
			connect.SendDouble(p->getTransportation());
		}
	}
}
void Supplier::redactSupplier(Connection& connect) {
	cout << "System --> Редактирование поставщика" << endl;
	vector<Supplier> suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		int choice;
		do {
			viewSupplier(connect);
			choice = cdat.check_int_interval(0, suppl.size(), connect);
			if (choice != 0) {
				char* name = new char[256];
				char* type = new char[100];
				char* supplier = new char[100];
				double price;
				int number;
				int day;
				int fail;
				double transportation;
				Supplier obj(suppl[choice - 1]);
				int edit = cdat.check_int_interval(0, 8, connect);
				switch (edit) {
				case 1:
					cdat.setStr(name, connect);
					obj.setName(name);
					break;
				case 2:
					typeOfGoods(type, connect);
					obj.setType(type);
					break;
				case 3:
					cdat.setStr(supplier, connect);
					obj.setSupplier(supplier);
					break;
				case 4:
					price = cdat.check_double_interval(0.01, 100000.00, connect);
					obj.setPrice(price);
					break;
				case 5:
					number = cdat.check_int_interval(1, 100000, connect);
					obj.setNumber(number);
					break;
				case 6:
					day = cdat.check_int_interval(1, 120, connect);
					obj.setDay(day);
					break;
				case 7:
					fail = cdat.check_int_interval(0, 100, connect);
					obj.setFail(fail);
					break;
				case 8:
					transportation = cdat.check_double_interval(0.00, 100000.00, connect);
					obj.setTransportation(transportation);
					break;
				}
				suppl[choice - 1] = obj;
				delete[] name; delete[] type; delete[] supplier;
				writeSupplier(suppl);
			}
		} while (choice != 0);
	}
}
void Supplier::deleteSupplier(Connection& connect) {
	cout << "System --> Удаление поставщика" << endl;
	vector<Supplier> suppl = readSupplier();

	int k = -1, choice = -1;
	if (!Check::checkFileEOF(OpinionFile)) k = 0;
	else k = 1;

	connect.SendInt(k);
	if (k > 0) {
		choice = cdat.check_int_interval(0, 1, connect);
		if (choice == 1) {
			ofstream file(OpinionFile, ios_base::trunc);
			file.close();
		}
	}
	if (choice != 0) {
		connect.SendInt(suppl.size());
		if (suppl.size() > 0) {
			do {
				viewSupplier(connect);
				choice = cdat.check_int_interval(0, suppl.size(), connect);
				if (choice != 0) {
					int del = cdat.check_int_interval(0, 1, connect);
					if (del == 1) {
						suppl.erase(suppl.cbegin() + choice - 1);
						writeSupplier(suppl);
					}
				}
			} while (choice != 0);
		}
	}
}
void Supplier::searchSupplier(Connection& connect) {
	cout << "System --> Поиск поставщика" << endl;
	vector<Supplier> suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		char* name = new char[256];
		char* type = new char[100];
		char* supplier = new char[100];
		double price = 0;
		int number = 0;
		int day = 0;
		int fail = 0;
		double transportation = 0;
		int choice = cdat.check_int_interval(1, 8, connect);
		switch (choice) {
		case 1:
			cdat.setStr(name, connect);
			break;
		case 2:
			typeOfGoods(type, connect);
			break;
		case 3:
			cdat.setStr(supplier, connect);
			break;
		case 4:
			price = cdat.check_double_interval(0.01, 100000.00, connect);
			break;
		case 5:
			number = cdat.check_int_interval(1, 100000, connect);
			break;
		case 6:
			day = cdat.check_int_interval(1, 120, connect);
			break;
		case 7:
			fail = cdat.check_int_interval(0, 100, connect);
			break;
		case 8:
			transportation = cdat.check_double_interval(0.00, 100000.00, connect);
			break;
		}
		connect.SendInt(suppl.size());
		for (vector<Supplier>::iterator it = suppl.begin(); it != suppl.end(); it++) {
			int k = 0;
			switch (choice) {
			case 1:
				if (strcmp(it->getName(), name) == 0) k = 1;
				break;
			case 2:
				if (strcmp(it->getType(), type) == 0) k = 1;
				break;
			case 3:
				if (strcmp(it->getSupplier(), supplier) == 0) k = 1;
				break;
			case 4:
				if (it->getPrice() == price) k = 1;
				break;
			case 5:
				if (it->getNumber() == number) k = 1;
				break;
			case 6:
				if (it->getDay() == day) k = 1;
				break;
			case 7:
				if (it->getFail() == fail) k = 1;
				break;
			case 8:
				if (it->getTransportation() == transportation) k = 1;
				break;
			}
			connect.SendInt(k);
			if (k == 1) {
				connect.SendStr(it->getName());
				connect.SendStr(it->getType());
				connect.SendStr(it->getSupplier());
				connect.SendDouble(it->getPrice());
				connect.SendInt(it->getNumber());
				connect.SendInt(it->getDay());
				connect.SendInt(it->getFail());
				connect.SendDouble(it->getTransportation());
			}
		}
		delete[] name; delete[] type; delete[] supplier;
		writeSupplier(suppl);
	}
}
void Supplier::sortSupplier(Connection& connect) {
	cout << "System --> Сортировка поставщиков" << endl;
	vector<Supplier> suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		/*char* name = new char[256];
		char* type = new char[100];
		char* supplier = new char[100];
		double price = 0;
		int number = 0;
		int day = 0;
		int fail = 0;
		double transportation = 0;*/
		int choice = cdat.check_int_interval(1, 8, connect);
		switch (choice) {
		case 1: sort(suppl.begin(), suppl.end(), compName);
			break;
		case 2: sort(suppl.begin(), suppl.end(), compType);
			break;
		case 3: sort(suppl.begin(), suppl.end(), compSupplier);
			break;
		case 4:	sort(suppl.begin(), suppl.end(), compPrice);
			break;
		case 5:	sort(suppl.begin(), suppl.end(), compNumber);
			break;
		case 6:	sort(suppl.begin(), suppl.end(), compDay);
			break;
		case 7:	sort(suppl.begin(), suppl.end(), compFail);
			break;
		case 8:	sort(suppl.begin(), suppl.end(), compTransportation);
			break;
		}
		for (vector<Supplier>::iterator p = suppl.begin(); p != suppl.end(); p++) {
			connect.SendStr(p->getName());
			connect.SendStr(p->getType());
			connect.SendStr(p->getSupplier());
			connect.SendDouble(p->getPrice());
			connect.SendInt(p->getNumber());
			connect.SendInt(p->getDay());
			connect.SendInt(p->getFail());
			connect.SendDouble(p->getTransportation());
		}
		//delete[] name; delete[] type; delete[] supplier;
	}
}
void Supplier::filtrSupplier(Connection& connect) {
	cout << "System --> Фильтрация поставщиков" << endl;
	vector<Supplier> suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		int minI = 0, maxI = 0;
		double minD = 0, maxD = 0;
		int choice = cdat.check_int_interval(1, 6, connect);
		switch (choice) {
		case 1:
			minD = cdat.check_double_interval(0.01, 100000.00, connect);
			maxD = cdat.check_double_interval(minD, 100000.00, connect);
			break;
		case 2:
			minI = cdat.check_int_interval(1, 100000, connect);
			maxI = cdat.check_int_interval(minI, 100000, connect);
			break;
		case 3:
			minD = cdat.check_int_interval(1, 120, connect);
			maxD = cdat.check_int_interval(minI, 120, connect);

			break;
		case 4:
			minD = cdat.check_int_interval(0, 100, connect);
			maxD = cdat.check_int_interval(minI, 100, connect);
			break;
		case 5:
			minD = cdat.check_double_interval(0.00, 100000.00, connect);
			maxD = cdat.check_double_interval(minD, 100000.00, connect);
			break;
		}
		connect.SendInt(suppl.size());
		for (vector<Supplier>::iterator it = suppl.begin(); it != suppl.end(); it++) {
			int k = 0;
			switch (choice) {
			case 1:
				if (it->getPrice() >= minD && it->getPrice() <= maxD) k = 1;
				break;
			case 2:
				if (it->getNumber() >= minI && it->getNumber() <= maxI) k = 1;
				break;
			case 3:
				if (it->getDay() >= minI && it->getDay() <= maxI) k = 1;
				break;
			case 4:
				if (it->getFail() >= minI && it->getFail() <= maxI) k = 1;
				break;
			case 5:
				if (it->getTransportation() >= minD && it->getTransportation() <= maxD) k = 1;
				break;
			}
			connect.SendInt(k);
			if (k == 1) {
				connect.SendStr(it->getName());
				connect.SendStr(it->getType());
				connect.SendStr(it->getSupplier());
				connect.SendDouble(it->getPrice());
				connect.SendInt(it->getNumber());
				connect.SendInt(it->getDay());
				connect.SendInt(it->getFail());
				connect.SendDouble(it->getTransportation());
			}
		}
	}
}


template <typename  T>
void selection_sort(T* a, T& b, int left, int right, int(*pf_compare)) {
	for (int i = left + 1; i < right; i++)
		for (int j = i; j > left; j--)
			if (pf_compare(a, b) > 0) {
				T temp;
				temp = a;
				a = b;
				b = temp;
			}
}


void Supplier::operator=(const Supplier& obj) {
	delete[] this->name;
	delete[] this->type;
	delete[] this->supplier;
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->type, obj.type);
	cdat.copyStr(this->supplier, obj.supplier);
	this->price = obj.price;
	this->number = obj.number;
	this->day = obj.day;
	this->fail = obj.fail;
	this->transportation = obj.transportation;
}

Supplier::~Supplier() {
	delete[] this->name;
	delete[] this->type;
	delete[] this->supplier;
	cout << "System --> Деструктор Supplier" << endl;
}