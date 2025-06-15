/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/String.h"
#include "../FileHandlers/UserHandler/UserFileHandler.h"
#include <stdint.h>

enum class UserType : unsigned {
	Admin,
	Client,
};

class User {
protected:
	String name;
	String hashedPassword;
	unsigned id;
	friend User* UserFileHandler::readUser();

public:
	User() = default;
	User(const String& name, const String& password, int idCounter);
	
	virtual UserType getRole() const = 0;
	virtual User* clone() const = 0;
	const String& getName() const;
	const String& getHashedPassword() const;
	unsigned getId() const;
};
