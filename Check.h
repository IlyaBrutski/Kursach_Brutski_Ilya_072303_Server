#pragma once
#include "Connection.h"

class Check {
public:
	static bool checkFileEOF(const char*);
	void setStr(char*&, Connection&);
	void copyStr(char*&, const char*);
	int check_int(Connection&);
	int check_int_interval(int, int, Connection&);
	double check_double(Connection&);
	double check_double_interval(double, double, Connection&);
};

static Check cdat;