#include "IndividualChatFileHandler.h"
#include "../../Components/Chats/IndividualChat.h"

IndividualChatFileHandler::IndividualChatFileHandler(const String& str) {
	fileHandler = FileFactory::createFileHandler(Config::fileExtension);
	fileHandler->open(str);
}

IndividualChatFileHandler& IndividualChatFileHandler::getInstance(const String& str) {
	static IndividualChatFileHandler instance(str);

	return instance;
}

IndividualChatFileHandler::~IndividualChatFileHandler() {
	delete fileHandler;
}

void IndividualChatFileHandler::saveChat(const IndividualChat& chat, FileHandler& fs)  {
	if(!fs.isOpen()) throw std::runtime_error("file can not be opened");

	fs.write((unsigned)chat.getType());
	fs.write(chat.getId());
	fs.write(chat.getUsersIds()[0]);
	fs.write(chat.getUsersIds()[1]);
	fs.file.flush();
}
