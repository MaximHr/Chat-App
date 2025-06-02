/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "UserFactory.h"

User* UserFactory::createUser(UserType type){
	switch(type) {
		case UserType::Admin:
			return new Admin();
		case UserType::Client:
			return new Client();
		default:
			throw std::runtime_error("Can not create user");
	}
}

User* UserFactory::createUser(UserType type, const String& name, const String& password, unsigned idCounter){
	switch(type) {
		case UserType::Client :
			return new Client(name, password, idCounter);
		case UserType::Admin :
			return new Admin(name, password, idCounter);
		default:
			throw std::runtime_error("Can not create user");
	}
}