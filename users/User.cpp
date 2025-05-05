#include "User.h"
#include "iostream"

FileHandler User::file("users.dat");

User::User() {
	_isLogged = false;
}

User::User(const String& name, const String& password):_isLogged(false) {
	this->name = name;
	this->password = password;
};

void User::logOut() {
	_isLogged = false;
}

void User::saveToFile() {
	file.write(name);
	file.write(password);
	file.file.flush();
	// file.write((const char*)isLogged, sizeof(isLogged));
}

User User::getUserFromFile() const {
	if (!file.isOpen()) throw std::runtime_error("File can not be opened");
	User user;
	file.read(user.name);
	file.read(user.password);
	return user;
}

int User::findFromFile(bool checkNameAndPassword) {
	unsigned index = file.file.tellg();
	file.file.seekg(0);
	User tempUser = getUserFromFile();
	int result = 0;
	while ((checkNameAndPassword && tempUser != *this) || (!checkNameAndPassword && tempUser.name != name)) {
		if (file.file.eof()) return -1;
	
		result = file.file.tellg();
		std::cout << tempUser.name << ' ' << tempUser.password << '\n';
		tempUser = getUserFromFile();
	}

	file.file.clear();
	file.file.seekg(index);
	return result;
}

bool User::isLogged() const {
	return _isLogged;
}

void User::logIn() {
	if (_isLogged) return;
	if (findFromFile(true) == -1) {
		throw std::runtime_error("Invalid credentials!");
	}
	_isLogged = true;
}

void User::signUp() {
	if (findFromFile(false) != -1) {
		throw std::runtime_error("Username is already taken!");
	}
	saveToFile();
	_isLogged = true;
}

bool operator==(const User& lhs, const User& rhs) {
	return (lhs.name == rhs.name && lhs.password == rhs.password);
}

bool operator!=(const User& lhs, const User& rhs) {
	return !operator==(lhs, rhs);
}