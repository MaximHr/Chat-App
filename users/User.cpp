/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "User.h"

//for simplcity i am not hashing the password, but just reversing it
User::User(const String& name, const String& password, int idCounter):
	name(name), hashedPassword(password.reverse()), id(idCounter)
	{
};

const String& User::getName() const {
	return name;
}

unsigned User::getId() const {
	return id;
}

const String& User::getHashedPassword() const {
	return hashedPassword;
}
