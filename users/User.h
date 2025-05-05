#pragma once
#include "../utils/String.h"
#include "fstream"
#include "../files/FileHandler.h"
#include "exception"

class User {

private:
	String name;
	String password;
	bool _isLogged;
	static FileHandler file;

	void saveToFile();
	User getUserFromFile() const;
	int findFromFile(bool checkNameAndPassword);
public:
	User();
	User(const String& name, const String& password);
	void logIn();
	void signUp();
	void logOut();
	bool isLogged() const;
	friend bool operator==(const User& lhs, const User& rhs);
};

bool operator==(const User& lhs, const User& rhs);
bool operator!=(const User& lhs, const User& rhs);