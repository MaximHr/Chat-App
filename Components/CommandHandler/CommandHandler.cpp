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
			system.viewChats();
		} else if(command == "view_all_chats") {
			system.viewAllChats();
		} else if(command == "select_chat") {
			selectChat();
		} else if(command == "create_group") {
			createGroup();
		} else if(command == "leave_group") {
			leaveGroup();
		} else if(command == "add_to_group") {
			addToGroup();
		} else if(command == "kick_from_group") {
			kickFromGroup();	
		} else if(command == "set_group_admin") {
			setGroupAdmin();
		} else if(command == "group_stats") {
			showGroupStats();
		} else if(command == "delete_group") {
			deleteGroup();
		} else if(command == "change_password") {
			changePassword();
		} else if(command == "message_individual") {
			messageIndividual();
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
	std::cout << "message_individual" << '\n';
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

void CommandHandler::deleteGroup() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	system.deleteGroup(chatId);
	std::cout << "Group deleted successfully" << '\n';
}

void CommandHandler::showGroupStats() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	system.showGroupStats(chatId);
}

void CommandHandler::setGroupAdmin() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	std::cout << "New admin id: ";
	unsigned adminId = getValidInfo<unsigned>();
	system.setGroupAdmin(chatId, adminId);
	std::cout << "Group admin set successfully" << '\n';
}

void CommandHandler::kickFromGroup() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	std::cout << "Member id: ";
	unsigned memberId = getValidInfo<unsigned>();
	system.kickFromGroup(chatId, memberId);
	std::cout << "User kicked successfully" << '\n';
}

void CommandHandler::addToGroup() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	std::cout << "Member id: ";
	unsigned memberId = getValidInfo<unsigned>();
	system.addToGroup(chatId, memberId);
	std::cout << "User added successfully" << '\n';
}

void CommandHandler::leaveGroup() {
	std::cout << "Group chat id: ";
	unsigned chatId = getValidInfo<unsigned>();
	system.leaveGroup(chatId);
	std::cout << "You left the chat successfully" << '\n';
}

void CommandHandler::messageIndividual() {
	String name, message;
	std::cout << "Username: ";
	name = getValidInfo<String>();
	unsigned chatId = system.messageIndividual(name);
	message = enterMessage();
	system.sendIndividualMessage(chatId, message);
	std::cout << "message sent succefully" << '\n';
}

void CommandHandler::createGroup() {
	String name;
	std::cout << "Group name: ";
	name = getValidInfo<String>();

	std::cout << "Add members (type exit to stop)\n";
	String command;
	String members[Config::maxGroupMembers];
	unsigned length = 1; // the first user is the creator of the group
	do {
		std::cout << "Name: ";
		command = getValidInfo<String>();
		if(command == "exit") {
			break;
		}

		bool userExists = system.doesUserExist(command);
		if(userExists) {
			members[length] = command;
			length++;
		} else {
			std::cout << "user with that name does not exist" << '\n';
		}
	} while(length <= Config::maxGroupMembers);

	unsigned id = system.createGroup(name, members, length);
	std::cout << "Group created successfully with an id of " << id << '\n';
}

const String CommandHandler::enterMessage() {
	String message;
	std::cout << "Enter message: ";
	message = getValidInfo<String>();
	return message;
}

void CommandHandler::selectChat() {
	String message;
	unsigned chatId;
	std::cout << "Chat id: ";
	chatId = getValidInfo<unsigned>();
	system.printGroupMessages(chatId);
	message = enterMessage();
	system.sendGroupMessage(chatId, message);
	std::cout << "message sent succefully" << '\n';
}