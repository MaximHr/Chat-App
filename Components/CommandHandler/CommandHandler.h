/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../../System/System.h"
#include "../../Utils/String.h"

class CommandHandler {
	System& system;
	void addUser();
	void deleteUser();
	void help();
	void login();
	void callCommand(const String& str);
	void changePassword();

public:
	void start();
	CommandHandler();
};