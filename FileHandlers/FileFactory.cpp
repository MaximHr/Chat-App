/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "FileFactory.h"
#include "BinaryHandler/BinaryFileHandler.h"
#include "TextHandler/TextFileHandler.h"

FileHandler* FileFactory::createFileHandler(FileExtension extension) {
	switch(extension) {
		case FileExtension::dat :
			return new BinaryFileHandler();
		case FileExtension::txt :
			return new TextFileHandler();
		default:
			throw std::runtime_error("Can not create file");
	}
}