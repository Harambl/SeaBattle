#ifndef DATABASE
#define DATABASE

#if(0)
#include <QString>
#endif

#include "User.h"
#include <sqlite3.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;


template<typename T>
class DataBase {

	string baseName;	
	vector<pair<string, string>> params;
	vector<string> tables;

	static int _code;
	static vector<T> _objects;

	void check_open(int code, sqlite3* DB);
	void check_exec(int code, sqlite3* DB, const char* errMsg);
	void execute(string& sql, void* data = nullptr,
			int callback(void*, int, char**, char**) = NULL);
	string secure(string& s);
	static int findObj_callback(void*, int, char**, char**);
	static int getAllObj_callback(void*, int, char**, char**);

public:
	DataBase(string _baseName, string mainTableName);
	void writeObj(const T& obj, string tableName);
	bool findObj(const T* obj, vector<size_t> findParams, string tableName);
	void getAllObj(vector<T>* pObjects, string tableName);
	void updateObj(vector<pair<size_t, string>>& updPairs,
		       	string keyValue, size_t parametrInd, string tableName);
	T getObj(string keyValue, size_t parametrInd, string tableName);
};


template class DataBase<User>;

#endif
