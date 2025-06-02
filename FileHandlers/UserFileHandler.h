/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/Config.h"
#include "FileHandler.h"
#include "../Utils/String.h"
#include <exception>
#include <iostream>
class User;

enum class FindType {
	byId,
	byIdAndPassword,
	byName,
};

class UserFileHandler: public FileHandler {
private:
	User* getUserMatcher(unsigned id, const String& hashedPassword, bool shouldCheckForPassword);
	int findUserMatcher(unsigned id, const String& hashedPassword, const String& name, FindType findType);
	void updateUserMatcher(unsigned id, const User* updatedUser);

	UserFileHandler(const String& str);
	UserFileHandler(const UserFileHandler& other) = delete;
	UserFileHandler& operator=(const UserFileHandler& other) = delete;
public:
	static UserFileHandler& getInstance(const String& str);
	~UserFileHandler() = default;
	UserFileHandler() = delete;

	void saveUser(const User* user, FileHandler& fs);
	User* readUser();
	User* readUser(int& sizeInBytes);
	User* getUser(unsigned id);
	User* getUserByPassword(unsigned id, const String& hashedPassword);
	int findUser(unsigned id);
	int findUserByName(const String& name);
	int findUserWithPassword(unsigned id, const String& hashedPassword);
	void deleteUser(unsigned id);
	void updateUser(unsigned id, const User* updatedUser);
};