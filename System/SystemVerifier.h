/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Users/User.h"
#include "../FileHandlers/UserFileHandler.h"

class SystemVerifier {
private:
	UserFileHandler& userFileHandler;
public:
	SystemVerifier();

	void requireLogged(const User* user) const;
	void requireLoggedOut(const User* user) const;
	void requireAdmin(const User* user) const;
	void requireClient(const User* user) const;
	void requireUniqueUsername(const String& name);
	void requireNotAdmin(unsigned id);
};