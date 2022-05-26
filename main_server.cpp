#include "Connection.h"
#include "Check.h"
#include "User.h"
#include "Supplier.h"
#include "Opinion.h"
//server

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int k = -1, choice;
	User user;
	Supplier supplier;
	Opinion opinion;
	Connection connect;
	connect.acept();

	if (!Check::checkFileEOF(User::AdminFile)) {
		connect.SendInt(k = 1);
		if (user.registration(User::AdminFile, connect) == false)
			return 0;
	}
	else connect.SendInt(k = 0);

	while (1) {
		choice = cdat.check_int_interval(0, 3, connect);
		switch (choice) {
		case 1:
			if (user.authorization(User::AdminFile, connect)) {
				while (1) {
					choice = cdat.check_int_interval(0, 9, connect);
					switch (choice) {
					case 1:
						supplier.addSupplier(connect);
						break;
					case 2:
						supplier.viewSupplier(connect);
						break;
					case 3:
						supplier.redactSupplier(connect);
						break;
					case 4:
						supplier.deleteSupplier(connect);
						break;
					case 5:
						supplier.searchSupplier(connect);
						break;
					case 6:
						supplier.sortSupplier(connect);
						break;
					case 7:
						supplier.filtrSupplier(connect);
						break;
					case 8:
						opinion.findAlternative(connect);
						break;
					case 9:
						while (1) {
							choice = cdat.check_int_interval(0, 3, connect);
							switch (choice) {
							case 1:
								user.registration(User::UsersFile, connect);
								break;
							case 2:
								user.viewUser(connect);
								break;
							case 3:
								user.deleteUser(connect);
								break;
							default:
								break;
							}
							if (choice == 0)
								break;
						}
						choice = -1;
						break;
					default:
						break;
					}
					if (choice == 0)
						break;
				}
			}
			break;
		case 2:
			if (Check::checkFileEOF(User::UsersFile)) {
				connect.SendInt(0);
				if (user.authorization(User::UsersFile, connect) == true) {
					while (1) {
						choice = cdat.check_int_interval(0, 6, connect);
						switch (choice) {
						case 1:
							supplier.viewSupplier(connect);
							break;
						case 2:
							supplier.searchSupplier(connect);
							break;
						case 3:
							supplier.sortSupplier(connect);
							break;
						case 4:
							supplier.filtrSupplier(connect);
							break;
						case 5:
							opinion.addOpinion(connect);
							break;
						case 6:
							opinion.viewOpinion(connect);
							break;
						default:
							break;
						}
						if (choice == 0)
							break;
					}
				}
			}
			else connect.SendInt(1);
			break;
		case 3:
			user.registration(User::UsersFile, connect);
			break;
		case 0:
			return 0;
			break;
		}
	}
}