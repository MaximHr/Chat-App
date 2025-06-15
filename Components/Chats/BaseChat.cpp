#include "BaseChat.h"

BaseChat::BaseChat(unsigned id, unsigned membersCount, unsigned messagesCount) : 
	id(id), messagesCount(messagesCount) { }

unsigned BaseChat::getId() const {
	return id;
}

unsigned BaseChat::getMessagesCount() const {
	return messagesCount;
}

void BaseChat::incrementMessagesCount() {
	messagesCount++;
}