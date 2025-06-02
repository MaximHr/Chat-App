/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/String.h"
#include "FileHandler.h"
class Message;

enum class MessageExecutionType {
	DELETE_MESSAGE,

};

class MessageFileHandler : public FileHandler {
	MessageFileHandler(const String& str);
	MessageFileHandler(const MessageFileHandler& other) = delete;
	MessageFileHandler& operator=(const MessageFileHandler& other) = delete;
public:
	static MessageFileHandler& getInstance(const String& str);
	~MessageFileHandler() = default;
	MessageFileHandler() = delete;

	void saveMessage(const Message& message, FileHandler& fs);
	void printMessages(unsigned recieverId);
	int findMessage(unsigned id);
	Message* readMessage();
	Message* readMessage(int& bytes);
	Message* getMessage(unsigned id);
	void deleteMessages(unsigned recieverId);
};