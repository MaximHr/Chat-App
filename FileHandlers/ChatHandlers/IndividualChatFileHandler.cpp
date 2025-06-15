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
	fs.write(chat.getMessagesCount());
	fs.file.flush();
}

int IndividualChatFileHandler::findChat(unsigned user1Id, unsigned user2Id) {
	if(!fileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(fileHandler->getFileSize() == 0) return -1;

	int index = fileHandler->setAtBeginning();
	IndividualChat chat = readChat();
	int result = 0;

	while (
		!(chat.getUsersIds()[0] == user1Id && chat.getUsersIds()[1] == user2Id)
		&& !(chat.getUsersIds()[0] == user2Id && chat.getUsersIds()[1] == user1Id)
	)
	{
		if(fileHandler->file.eof()) {
			fileHandler->file.clear();
			return -1;
		}
		result = fileHandler->file.tellg();
		chat = readChat();
	}

	fileHandler->file.clear();
	fileHandler->file.seekg(index);
	return result;
}

IndividualChat IndividualChatFileHandler::readChat() {
	IndividualChat chat;
	fileHandler->skipIds(sizeof(unsigned));
	fileHandler->read(chat.id);
	fileHandler->read(chat.usersIds[0]);
	fileHandler->read(chat.usersIds[1]);
	fileHandler->read(chat.messagesCount);

	return chat;
}