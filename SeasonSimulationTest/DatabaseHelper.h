#pragma once

#include "sqlite3.h"
#include <map>

// Foreward declaration
class Core;

namespace Database
{
	class DatabaseResults;
	class DatabaseUtility
	{
	public:
		DatabaseUtility();
		sqlite3* GetDatabase();

		bool OpenDatabase(const char* file_name);
		
		int FindIndexTypeInTable(std::string name, std::string table);

		void LoadDatabase();
		void FetchDatabase();

		int LoadTable(std::string table_name);
		void CacheResults();

		template<class T>
		bool CacheTable(std::vector<T*>&, const char*);

		std::vector<Database::DatabaseResults*>* GetResults(std::string table_name);
		void LoadResults(void *data, int argc, char** column_fields, char** column_name);
	private:
		std::map<std::string, std::vector<Database::DatabaseResults*>*> table_map;
		sqlite3* pDatabase;
	};

	class DatabaseResults
	{
	public:
		DatabaseResults();
		~DatabaseResults();

		void LoadResults(int num_columns, char** column_names, char** column_data);

		int GetInt(std::string column_name);
		bool GetBool(std::string column_name);
		std::string GetText(std::string column_name);
	private:
		std::map<std::string, std::string> data_map;
	};
}



// inline template definition
template<class T>
// Loads a table 
bool Database::DatabaseUtility::CacheTable(std::vector<T*>& collection, const char* tablename)
{
	std::vector<Database::DatabaseResults*>* table = GetResults(tablename);

	if (table != NULL)
	{
		// Go row-by-row in the table
		for (std::vector<Database::DatabaseResults*>::iterator it = table->begin(); it != table->end(); it++)
		{
			// Instantiate a new object of type T, lookup its columns (CacheResults()), and push back the new object.
			T* kInfo = new T();
			kInfo->CacheResults(*it);
			collection.push_back(kInfo);
		}
	}
	else
	{
		std::cout << "Error reading table " << table << ". Aborting." << std::endl;
		return false;
	}
	return true;
}