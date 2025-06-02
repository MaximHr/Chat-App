/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "SystemVerifier.h"

SystemVerifier::SystemVerifier() : 
	userFileHandler(UserFileHandler::getInstance(Config::getFile(0))) 
{ }

void SystemVerifier::requireLogged(const User* user) const {
	if(user == nullptr) {
		throw std::runtime_error("Access denied: user not logged in.");
	}
}

void SystemVerifier::requireLoggedOut(const User* user) const {
	if(user != nullptr) {
		throw std::runtime_error("You must logout first.");
	}
}

void SystemVerifier::requireAdmin(const User* user) const {
	if(user->getRole() != UserType::Admin) {
		throw std::runtime_error("Access denied: requires Admin role.");
	}
}

void SystemVerifier::requireClient(const User* user) const {
	if(user->getRole() != UserType::Client) {
		throw std::runtime_error("Access denied: requires Client role.");
	}
}

void SystemVerifier::requireUniqueUsername(const String& name) {
	if(userFileHandler.findUserByName(name) != -1) {
		throw std::runtime_error("User with that name already exists");
	}
}

void SystemVerifier::requireNotAdmin(unsigned id) {
	User* searchedUser = userFileHandler.getUser(id);

	if(searchedUser->getRole() == UserType::Admin) {
		delete searchedUser;
		throw std::runtime_error("Admin can not be deleted.");
	}
}