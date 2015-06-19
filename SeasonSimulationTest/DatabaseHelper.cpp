#include "stdafx.h"
#include "DatabaseHelper.h"

// Wrapper function to Database::LoadResults()
static int LoadResultsWrapper(void *data, int argc, char** column_fields, char** column_names)
{
	CORE.GetDatabase().LoadResults(data, argc, column_fields,	column_names);
	return 0;
}

Database::DatabaseUtility::DatabaseUtility():
	table_map()
{
}

// Opens the database to be used.
bool Database::DatabaseUtility::OpenDatabase(const char* file_name)
{
	int rc = sqlite3_open(file_name, &pDatabase);
	if (rc)	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDatabase));
		return false;
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
		return true;
	}
}

// Load the database into DatabaseResults
void Database::DatabaseUtility::LoadDatabase()
{
	OpenDatabase("../database/hockey_db");
	LoadTable("Teams");
	LoadTable("Skaters");
	LoadTable("Goalies");
}

// Load in the database table by table.
void Database::DatabaseUtility::FetchDatabase()
{
	CacheTable(CORE.GetTeamInfo(), "Teams");
	CacheTable(CORE.GetSkaterInfo(), "Skaters");
	CacheTable(CORE.GetGoalieInfo(), "Goalies");
}

sqlite3* Database::DatabaseUtility::GetDatabase()
{
	return pDatabase;
}

// Accepts a name and searches "table" for a column named "Type". If one exists, return the index. If nothing is found, return -1
int Database::DatabaseUtility::FindIndexTypeInTable(std::string name, std::string table)
{
	std::vector<Database::DatabaseResults*>* pResults = GetResults(table);
	if (pResults)
	{
		for (std::vector<DatabaseResults*>::iterator it = pResults->begin(); it != pResults->end(); it++)
		{
			std::string type = (*it)->GetText("Type");
			if (type == name)
				return (*it)->GetInt("ID");
		}
	}
	return -1;
}

// Loads a row in the database. Called from LoadResultsWrapper, a wrapper function to here.
void Database::DatabaseUtility::LoadResults(void *data, int argc, char** column_fields, char** column_names)
{
	Database::DatabaseResults* pResults = new Database::DatabaseResults();
	pResults->LoadResults(argc, column_names, column_fields);

	// Find the table in the table map ((const char*)) is the table name and push the new row at the end of the vector
	table_map.find((const char*) data)->second->push_back(pResults);
}

// Loads a table into the table map in <column_name, Results> format so it can be referenced at any time
int Database::DatabaseUtility::LoadTable(std::string table_name)
{
	std::string sqlcommand = "Select * from " + table_name;
	char* errormsg = NULL;
	const char* data = table_name.c_str();

	// Create the entry into the table map first, so that we can reference it and add each new row to it.
	table_map.insert(std::pair<std::string, std::vector<Database::DatabaseResults*>*>((const char*)data, new std::vector<Database::DatabaseResults*>()));

	// Actual sql command to execute
	sqlite3_exec(pDatabase, sqlcommand.c_str(), LoadResultsWrapper, (void*)data, &errormsg);

	return 0;
}

// Retrieves all the rows in a table of the database, if the table exists
std::vector<Database::DatabaseResults*>* Database::DatabaseUtility::GetResults(std::string table_name)
{
	try
	{
		return table_map.at(table_name);
	}
	catch (std::exception ex)
	{
		std::cout << "Could not retrieve results. Table " << table_name << " does not exist in the loaded database." << std::endl;
		return NULL;
	}
}

////////////////////////////////////////////
// DATABASE RESULTS
////////////////////////////////////////////

Database::DatabaseResults::DatabaseResults():
	data_map()
{

}
Database::DatabaseResults::~DatabaseResults()
{

}

// Load the results in a map in <column name, column data> format.
void Database::DatabaseResults::LoadResults(int num_columns, char** column_names, char** column_data)
{
	for (int i = 0; i < num_columns; i++)
	{
		data_map.insert(std::pair<std::string, std::string>(column_names[i], column_data[i]));
	}
}

// Looks through the data map for column name and returns associated value, if it exists (-1 if doesn't)
int Database::DatabaseResults::GetInt(std::string column_name)
{
	std::string value;
	try	{
		return atoi(data_map.at(column_name).c_str());
	}
	catch (std::exception ex)	{
		std::cout << "'" << column_name << "' is an invalid column name!" << std::endl;
		return -1;
	}
}

// Looks through the data map for column name and returns associated value, if it exists (false if doesn't)
bool Database::DatabaseResults::GetBool(std::string column_name)
{
	try	{
		return (data_map.at(column_name) == "true" ? true : false);
	}
	catch (std::exception ex)	{
		std::cout << "'" << column_name << "' is an invalid column name!" << std::endl;
		return false;
	}
}

// Looks through the data map for column name and returns associated value, if it exists ("" if doesn't)
std::string Database::DatabaseResults::GetText(std::string column_name)
{
	std::string value;
	try	{
		return data_map.at(column_name);
	}
	catch (std::exception ex)	{
		std::cout << "'" << column_name << "' is an invalid column name!" << std::endl;
		return "";
	}
}