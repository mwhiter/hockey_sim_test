#include "stdafx.h"
#include "Logger.h"

Logger::Logger():
	fileMap(),
	currentFile()
{
}

Logger::~Logger()
{
	std::map<std::string, std::ofstream*>::iterator it;
	for (it = fileMap.begin(); it != fileMap.end(); it++)
	{
		// Close the file.
		it->second->close();
		// Delete the pointer.
		delete it->second;
	}

	fileMap.clear();
}

// Write text to a file. If it doesn't exist in the map, this will fail, so make sure to clal SetCurrentFile() before writing.
void Logger::WriteLogToFile(std::string szString)
{
	// The file map should never be empty unless SetCurrentFile() was not called.
	if (fileMap.empty())
	{
		std::cout << "Error! No current file loaded! Use SetCurrentFile()" << std::endl;
		return;
	}

	std::ofstream* pFile = fileMap.find(currentFile)->second;
	*pFile << szString << "\n";
	return;
}

// Set the current file we're writing to. If the file doesn't exist, create it
void Logger::SetCurrentFile(std::string szFileName)
{
	// Search the map for the file
	std::map<std::string, std::ofstream*>::iterator it = fileMap.find(szFileName);
	
	// Didn't find the file - create a new one!
	if (it == fileMap.end())
	{
		std::ofstream* pNewFile = new std::ofstream();
		pNewFile->open("../logs/" + szFileName);

		// Insert into the map a key (file name) and value (file pointer)
		fileMap.insert(std::pair<std::string, std::ofstream*>(szFileName, pNewFile));
	}
	
	// Set the current file to the new file
	currentFile = szFileName;
}