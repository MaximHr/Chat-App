#include <iostream>
#include "users/User.h"
#include "files/FileHandler.h"

int main() {
	User max("Gosho", "8989");
	max.signUp();
	// max.logIn();
	std::cout << max.isLogged();

	return 0;
}