/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "Message.h"

Message::Message(unsigned id, const String& text, unsigned chatId, unsigned senderId) : 
	id(id), text(text), chatId(chatId), senderId(senderId) 
{
	setTime();
}

const String& Message::getText() const {
	return text;
}

unsigned Message::getSenderId() const {
	return senderId;
}

unsigned Message::getChatId() const {
	return chatId;
}

unsigned Message::getId() const {
	return id;
}

const String& Message::getFormattedTime() const {
	return formattedTime;
}

void Message::setTime() {
	time_t now;
	time(&now);
	formattedTime = ctime(&now);
	formattedTime = formattedTime.remove(16, 3); //removes the seconds
}

std::ostream& operator<<(std::ostream& output, const Message& message) {
	output << message.getFormattedTime() << "Id " << message.getSenderId() << " : " << message.getText() << "\n______________________\n";
	return output;
}