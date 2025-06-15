/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once

enum class ChatType: unsigned {
	Individual,
	Group
};

class BaseChat {
protected:
	unsigned id;
	unsigned messagesCount;
	// friend ChatFileHandler* ChatFileHandler::readChat();
public:
	BaseChat(unsigned id, unsigned membersCount, unsigned messagesCount);
	BaseChat() = default;
	virtual ~BaseChat() = default;

	unsigned getId() const;
	unsigned getMembersCount() const;
	unsigned getMessagesCount() const;
	void incrementMessagesCount();
	virtual ChatType getType() const = 0;
};