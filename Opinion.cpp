#include "Opinion.h"

const char* Opinion::OpinionFile = "FileOpinion.txt";
const char* Opinion::LogFile = "FileLoginUser.txt";

Opinion::Opinion() : surname(0), name(0), log(0), count(0), array(0) {
	cout << "System --> Конструктор по умолчанию Opinion" << endl;
}
Opinion::Opinion(char* surname, char* name, char* log, int count, int** array) {
	cdat.copyStr(this->surname, surname);
	cdat.copyStr(this->name, name);
	cdat.copyStr(this->log, log);
	this->count = count;
	this->array = new int* [this->count];
	for (int i = 0; i < this->count; i++)
		this->array[i] = new int[this->count];
	for (int i = 0; i < this->count; i++) {
		for (int j = 0; j < this->count; j++)
			this->array[i][j] = array[i][j];
	}
	cout << "System --> Конструктор с параметрами Opinion" << endl;
}
Opinion::Opinion(const Opinion& obj) {
	cdat.copyStr(this->surname, obj.surname);
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->log, obj.log);
	this->count = obj.count;
	this->array = new int* [this->count];
	for (int i = 0; i < this->count; i++)
		this->array[i] = new int[this->count];
	for (int i = 0; i < this->count; i++) {
		for (int j = 0; j < this->count; j++)
			this->array[i][j] = obj.array[i][j];
	}
	cout << "System --> Конструктор копирования Opinion" << endl;
}

vector<Opinion> Opinion::readOpinion() {
	cout << "System --> Чтение ранжировак" << endl;
	vector<Opinion> opinion;
	char* surname = new char[100];
	char* name = new char[100];
	char* log = new char[100];
	int count;
	ifstream fin(OpinionFile);
	if (fin >> surname >> name >> log >> count) {
		int** array = new int* [count];
		for (int i = 0; i < count; i++)
			array[i] = new int[count];
		fin.seekg(0, ios_base::beg);
		while (fin >> surname >> name >> log >> count) {
			for (int i = 0; i < count; i++) {
				for (int j = 0; j < count; j++)
					fin >> array[i][j];
			}
			Opinion obj(surname, name, log, count, array);
			opinion.push_back(obj);
		}
		for (int i = 0; i < count; i++)
			delete[] array[i];
		delete[] array;
	}
	delete[] surname; delete[] name; delete[] log;
	fin.close();
	return opinion;
}
void Opinion::writeOpinion(vector<Opinion>& opinion) {
	cout << "System --> Запись ранжировак" << endl;
	if (opinion.size() > 0) {
		ofstream fout(OpinionFile, ios_base::trunc);
		for (vector<Opinion>::iterator p = opinion.begin(); p != opinion.end(); p++) {
			fout << p->getSurname() << " " << p->getName() << " " << p->getLog() << " " << p->getCount();;
			for (int i = 0; i < p->getCount(); i++) {
				for (int j = 0; j < p->getCount(); j++)
					fout << " " << p->getArray()[i][j];
			}
			fout << endl;
		}
		fout.close();
	}
}
void Opinion::addOpinion(Connection& connect) {
	cout << "System --> Добавление ранжировки" << endl;
	vector<Supplier> suppl = readSupplier();
	connect.SendInt(suppl.size());
	if (suppl.size() > 0) {
		viewSupplier(connect);

		vector<Opinion> opinion = readOpinion();

		char* surname = new char[100];
		char* name = new char[100];
		char* log = new char[100];
		int** array = new int* [suppl.size()];
		for (int i = 0; i < suppl.size(); i++)
			array[i] = new int[suppl.size()]{ 0 };
		int* F = new int[suppl.size()]{ 0 };
		int k = 0, finding = 0;
		//int N = suppl.size() * (suppl.size() - 1);
		//connect.SendInt(N);



		for (int i = 0; i < suppl.size(); i++) {
			for (int j = 0; j < suppl.size(); j++) {
				if (i != j) {
					array[i][j] = cdat.check_int_interval(0, 20, connect);
					array[j][i] = 20 - array[i][j];
				}
			}
		}
		for (int i = 0; i < suppl.size(); i++) {
			for (int j = 0; j < suppl.size(); j++) {
				connect.SendInt(array[i][j]);
			}
		}

		ifstream fin(LogFile);
		fin >> surname >> name >> log;;
		fin.close();

		Opinion obj(surname, name, log, suppl.size(), array);
		if (opinion.size() > 0) {
			for (int i = 0; i < opinion.size(); i++) {
				if (strcmp(opinion[i].getLog(), log) == 0) {
					opinion[i] = obj;
					k = 1;
					break;
				}
			}
		}
		if (k == 0) {
			opinion.push_back(obj);
		}

		for (int i = 0; i < suppl.size(); i++)
			delete[] array[i];
		delete[] array; delete[] log; delete[] F;

		writeOpinion(opinion);
	}
}
void Opinion::viewOpinion(Connection& connect) {
	cout << "System --> Просмотр ранжировак" << endl;
	vector<Opinion> opinion = readOpinion();
	vector<Supplier> suppl = readSupplier();

	connect.SendInt(opinion.size());
	if (opinion.size() > 0) {
		int k = -1;

		char* log = new char[100];
		int* F = new int[suppl.size()]{ 0 };
		double* Q = new double[suppl.size()]{ 0 };
		int finding = 0;

		ifstream fin(LogFile);
		fin >> log >> log >> log;
		fin.close();

		for (int i = 0; i < opinion.size(); i++) {
			if (strcmp(opinion[i].getLog(), log) == 0) {
				k = i;
				break;
			}
		}
		connect.SendInt(k);

		if (k != -1) {
			viewSupplier(connect);
			int N = suppl.size() * (suppl.size() - 1);
			connect.SendInt(suppl.size());
			//connect.SendInt(N);
			for (int i = 0; i < suppl.size(); i++) {
				for (int j = 0; j < suppl.size(); j++) {
					connect.SendInt(opinion[k].getArray()[i][j]);
					F[i] += opinion[k].getArray()[i][j];
				}
			}

			for (int i = 0; i < suppl.size(); i++) {
				connect.SendInt(F[i]);
				Q[i] = F[i];
			}

			for (int i = 0; i < suppl.size(); i++) {
				Q[i] /= (N * 20);
				//connect.SendDouble(Q[i]);
				if (Q[finding] < Q[i]) finding = i;
			}
			connect.SendInt(finding + 1);

			delete[] F; delete[] Q;
		}
		delete[] log;
	}
}
void Opinion::findAlternative(Connection& connect) {
	cout << "System --> Поиск альтернативы" << endl;
	vector<Opinion> opinion = readOpinion();
	vector<Supplier> suppl = readSupplier();

	connect.SendInt(opinion.size());
	if (opinion.size() > 0) {
		viewSupplier(connect);
		connect.SendInt(suppl.size());
		int N = suppl.size() * (suppl.size() - 1);
		//connect.SendInt(N);
		int* F = new int[suppl.size()]{ 0 };
		int finding = 0;
		double* Q = new double[suppl.size()]{ 0 };

		for (int i = 0; i < opinion.size(); i++) {
			for (int j = 0; j < suppl.size(); j++) {
				for (int k = 0; k < suppl.size(); k++) {
					F[j] += opinion[i].getArray()[j][k];
				}
			}
		}
		/*for (int i = 0; i < suppl.size(); i++) {
			connect.SendInt(F[i]);
			Q[i] = F[i];
		}*/
		for (int i = 0; i < suppl.size(); i++) {
			Q[i] = F[i];
			Q[i] /= (N * 20);
			connect.SendDouble(Q[i]);
			if (Q[finding] < Q[i])
				finding = i;
		}
		connect.SendInt(finding + 1);
		delete[] F; delete[] Q;

		int choice = cdat.check_int_interval(0, 1, connect);
		if (choice == 1) {
			for (vector<Opinion>::iterator p = opinion.begin(); p != opinion.end(); p++) {
				connect.SendStr(p->getSurname());
				connect.SendStr(p->getName());

				for (int i = 0; i < suppl.size(); i++) {
					for (int j = 0; j < suppl.size(); j++) {
						connect.SendInt(p->getArray()[i][j]);
					}
				}
			}
		}

	}
}

void Opinion::operator=(const Opinion& obj) {
	delete[] this->surname;
	delete[] this->name;
	delete[] this->log;
	for (int i = 0; i < this->count; i++)
		delete[] this->array[i];
	delete[] this->array;
	cdat.copyStr(this->surname, obj.surname);
	cdat.copyStr(this->name, obj.name);
	cdat.copyStr(this->log, obj.log);
	this->count = obj.count;
	this->array = new int* [this->count];
	for (int i = 0; i < this->count; i++) {
		this->array[i] = new int[this->count];
		for (int j = 0; j < this->count; j++)
			this->array[i][j] = obj.array[i][j];
	}
}

Opinion::~Opinion() {
	delete[] surname;
	delete[] name;
	delete[] log;
	for (int i = 0; i < this->count; i++)
		delete[] array[i];
	delete[] array;
	cout << "System --> Деструктор Opinion" << endl;
}
