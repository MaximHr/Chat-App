/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "../Utils/Config.h"
#include "System.h"

System::System() : 
	user(nullptr), 
	userFileHandler(UserFileHandler::getInstance(Config::getFile(0))),
	messageFileHandler(MessageFileHandler::getInstance(Config::getFile(2))),
	idContainer(Config::getFile(1), Config::fileNames, Config::numberOfFiles) 
{ }

System& System::getInstance() {
	static System instance;
	return instance;
}

unsigned System::addUser(UserType type, const String& name, const String& password) {
	verifier.requireLoggedOut(user);
	verifier.requireUniqueUsername(name);

	unsigned id = idContainer.getId(Config::getFile(0));
	User* newUser = UserFactory::createUser(type, name, password, id);

	userFileHandler.saveUser(newUser, userFileHandler);
	idContainer.increment(Config::getFile(0));
	delete newUser;

	return id;
}

void System::login(unsigned id, const String& password) {
	verifier.requireLoggedOut(user);
	user = userFileHandler.getUserByPassword(id, password.reverse());
}

void System::deleteUser(unsigned id) {
	verifier.requireLogged(user);
	verifier.requireAdmin(user);
	verifier.requireNotAdmin(id);
	
	int fileSize = userFileHandler.getFileSize();
	userFileHandler.deleteUser(id);
	
	if(fileSize != userFileHandler.getFileSize()) {
		std::cout << "User deleted succefully!" << '\n';
	} else {
		std::cout << "No user with that id found. " << '\n';
	}
}

void System::changePassword(const String& password) {
	verifier.requireLogged(user);

	const User* updatedUser = UserFactory::createUser(user->getRole(), user->getName(), password, user->getId());
	userFileHandler.updateUser(user->getId(), updatedUser);
	delete updatedUser;
}

void System::logout() {
	delete user;
	user = nullptr;
}

System::~System() {
	delete user;
	user = nullptr;
}
