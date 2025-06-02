/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/Config.h"
#include "../Utils/String.h"
#include "../Users/User.h"
#include "../Users/UserFactory.h"
#include "../IdManager/IdContainer.h"
#include "../FileHandlers/UserFileHandler.h"
#include "../FileHandlers/MessageFileHandler.h"
#include "SystemVerifier.h"
#include "../Components/Message/Message.h"
#include <exception>

class System {
	User* user;
	UserFileHandler& userFileHandler;
	MessageFileHandler& messageFileHandler;
	SystemVerifier verifier;
	IdContainer idContainer;
	
	System();
	System(const System& other) = delete;
	System& operator=(const System& other) = delete;

public:
	static System& getInstance();
	~System();

	unsigned addUser(UserType type, const String& name, const String& password);
	void deleteUser(unsigned id);
	void login(unsigned id, const String& password);
	void logout();
	void changePassword(const String& password);
};