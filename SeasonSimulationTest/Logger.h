#pragma once
// Logging class that will write out to the specified file
class Logger
{
public:
	Logger();
	~Logger();

	void WriteLogToFile(std::string szString);
	void SetCurrentFile(std::string szFileName);
private:
	std::map<std::string, std::ofstream*> fileMap;
	std::string currentFile;
};