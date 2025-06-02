/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "Client.h"

UserType Client::getRole() const {
	return UserType::Client;
}

Client::Client(const String& name, const String& password, unsigned idCounter) : 
	User(name, password, idCounter) { }

User* Client::clone() const {
	return new Client(*this);
}