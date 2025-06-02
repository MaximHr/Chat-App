/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "Admin.h"

Admin::Admin(const String& name, const String& password, unsigned idCounter) : 
	User(name, password, idCounter)
{ }

UserType Admin::getRole() const {
	return UserType::Admin;
}

User* Admin::clone() const {
	return new Admin(*this);
}