/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "CommandHandler.h"

CommandHandler::CommandHandler() : system(System::getInstance()) {}

namespace {
	template <typename T>
	T getValidInfo() {
		T number;
		while(true) {
			std::cin >> number;

			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
			} else {
				std::cin.ignore();
				return number;
			}
		}
	}

	template <>
	String getValidInfo<String>() {
		String str;
		do {
			std::cin >> str;
		} while(str.getSize() == 0);
		return str;
	}
}

void CommandHandler::start() {
	std::cout << "Welcome to Chat App Blah-Blah" << '\n';
	std::cout << "Type help to view all commands" << '\n';
	String str;
	do {
		std::cin >> str;
		callCommand(str);
	} while(str != "exit");
}

void CommandHandler::callCommand(const String& str) {
	String command = str.trim();
	try {
		if(command == "help") {
			help();
		} else if(command == "login") {
			login();
		} else if(command == "create_account") {
			addUser();
		} else if(command == "delete_user") {
			deleteUser();
		} else if(command == "logout") {
			system.logout();
		} else if(command == "view_chats") {

		} else if(command == "select_chat") {

		} else if(command == "create_group") {

		} else if(command == "leave_group") {

		} else if(command == "add_to_group") {

		} else if(command == "kick_from_group") {

		} else if(command == "set_group_admin") {

		} else if(command == "group_stats") {

		} else if(command == "delete_group") {

		} else if(command == "change_password") {
			changePassword();
		}
	} catch(const std::exception& e) {
		std::cout << e.what() << '\n';
	}
}

void CommandHandler::login() {
	unsigned id;
	String pass;

	std::cout << "Id: ";
	id = getValidInfo<unsigned>();
	std::cout << "Password: ";
	pass = getValidInfo<String>();

	system.login(id, pass);
	std::cout << "Login succefull!" << std::endl;
}

void CommandHandler::addUser() {
	String name, password, strType;
	UserType type;
	bool isValidType = false;

	do {
		std::cout << "Choose role: (admin or client): ";
		std::cin >> strType;
		if(strType == "admin") {
			type = UserType::Admin;
			isValidType = true;
		} else if(strType == "client") {
			type = UserType::Client;
			isValidType = true;
		}
	} while(!isValidType);
	
	std::cout << "username: ";
	name = getValidInfo<String>();
	std::cout << "password: ";
	password = getValidInfo<String>();

	unsigned id = system.addUser(type, name, password);
	std::cout << "Added " << strType << ' ' << name << " with ID " << id << '\n'; 
}

void CommandHandler::deleteUser() {
	std::cout << "id: ";
	unsigned id = getValidInfo<unsigned>();
	system.deleteUser(id);
}

void CommandHandler::help() {
	std::cout << "login" << '\n';
	std::cout << "create_account " << '\n';
	std::cout << "view_chats" << '\n';
	std::cout << "select_chat" << '\n';
	std::cout << "create_group" << '\n';
	std::cout << "leave_group" << '\n';
	std::cout << "add_to_group | Group admin" << '\n';
	std::cout << "kick_from_group | Group admin" << '\n';
	std::cout << "set_group_admin | Group admin" << '\n';
	std::cout << "group_stats " << '\n';
	std::cout << "delete_group | System admin" << '\n';
	std::cout << "delete_user | System admin" << '\n';
	std::cout << "change_password" << '\n';
	std::cout << "logout" << '\n';
	std::cout << "exit" << '\n';
}

void CommandHandler::changePassword() {
	String pass;
	std::cout << "New password: ";
	pass = getValidInfo<String>();
	system.changePassword(pass);
	std::cout << "Password changed succefully" << '\n';
}