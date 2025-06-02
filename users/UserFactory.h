/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "User.h"
#include "./Admin/Admin.h"
#include "./Client/Client.h"
#include <exception>

class UserFactory {
public:
	UserFactory() = delete;

	static User* createUser(UserType type);
	static User* createUser(UserType type, const String& name, const String& password, unsigned id);
};