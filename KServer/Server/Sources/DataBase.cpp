#include "DataBase.h"

template <typename T>
int DataBase<T>::_code {0};

template <typename T>
void DataBase<T>::check_open(int code, sqlite3* DB)
{
	if(code != SQLITE_OK) {
		cout << "Error: " << sqlite3_errmsg(DB) << endl;	
		throw std::runtime_error("database opening error");
	} cout << " -- DataBase opened successfully" << endl;
}

template <typename T>
void DataBase<T>::check_exec(int code, sqlite3* DB, const char* errMsg)
{
	if(code != SQLITE_OK) {
		cout << "Error: ";
		throw std::runtime_error(errMsg);
	} cout << " -- success execution" << endl;
}

template <typename T>
void DataBase<T>::execute(string& sql, void* data,
		int callback(void*, int, char**, char**))
{
	sqlite3* DB;
	int rc, exit;
	char* errMsg;

	exit = sqlite3_open(baseName.c_str(), &DB);
	check_open(exit, DB);

	rc = sqlite3_exec(DB, sql.c_str(), callback, data, &errMsg);	
	check_exec(rc, DB, errMsg);

	sqlite3_close(DB);
}

template <typename T>
string DataBase<T>::secure(string& s)
{
	string new_s {s}, unSecQuotes{"\""}, secQuotes {"\"\""};
	for(size_t i {0}; i < new_s.length(); ++i) {
		if(new_s[i] == '"')
			new_s.replace(i++, unSecQuotes.length(), secQuotes);	
	}
	return new_s;
}

template <typename T>
DataBase<T>::DataBase(string _baseName, string mainTableName) :
	baseName{_baseName}, params{T::getTableStructure()}
{

	tables = {mainTableName};
	string sql = "CREATE TABLE IF NOT EXISTS " + mainTableName + " (";
	sql += "id INTEGER PRIMARY KEY AUTOINCREMENT, ";
	for(size_t i{0}; i < params.size(); ++i) {
		sql += params[i].first;
	       	sql += " " + params[i].second;
		sql += ((i != params.size() - 1) ? ", " : ");");
	}
	execute(sql);
	cout << " -- Table " + mainTableName + " CREATED" << endl;
}

template <typename T>
void DataBase<T>::writeObj(const T& obj, string tableName)
{
	vector<pair<string, Type>> values {obj.getValues()};
	string sql {"INSERT INTO " + tableName + " ("}, ch;

	for(size_t i {0}; i < params.size(); ++i) {
		sql += params[i].first;
		sql += (i != params.size() - 1) ? ", " : ") ";
	}	

	sql += "VALUES (";
	for(size_t i {0}; i < params.size(); ++i) {
		ch = (values[i].second == Type::STR) ? "\"" : "";	
		sql += ch + secure(values[i].first) + ch;
		sql += (i < params.size() - 1) ? ", " : ");";
	}

	execute(sql);
}

template <typename T>
int DataBase<T>::findObj_callback(void* notUsed, int argc, char** argv, char** azColName)
{
	long long res;
	if(argc <= 0) throw std::runtime_error("Argument count error");
	res = std::stoll(argv[0]);
	_code = (res >= 1) ? 1 : 0;
	return 0;
}

template <typename T>
int DataBase<T>::getAllObj_callback(void* data, int argc, char** argv, char** azColName)
{
	vector<T>* pObjects {static_cast<vector<T>*>(data)};
	T obj {T::initialize(argc, argv)};
	pObjects->push_back(obj);
	return 0;
}

template <typename T>
bool DataBase<T>::findObj(const T* obj, vector<size_t> findParams, string tableName)
{
	vector<pair<string, Type>> values {obj->getValues()};
	string sql {"SELECT COUNT(*) FROM " + tableName + " WHERE "}, ch;

	for(size_t i {0}; i < findParams.size(); ++i) {
		ch = (values[findParams[i]].second == Type::STR) ? "\"" : "";	
		sql += params[findParams[i]].first + " = " + ch + secure(values[findParams[i]].first) + ch;	
		sql += (i < findParams.size() - 1) ? " AND " : ";";
	}

	execute(sql, nullptr, findObj_callback);
	return _code == 1;
}

template <typename T>
void DataBase<T>::getAllObj(vector<T>* pObjects, string tableName)
{
	string sql {"SELECT * FROM " + tableName + ";"};
	execute(sql, pObjects, getAllObj_callback);
}

template <typename T>
T DataBase<T>::getObj(string keyValue, size_t parametrInd, string tableName)
{
	T _Obj;
	vector<T> Objects; string ch;
	vector<pair<string, Type>> values {_Obj.getValues()};

	string sql {"SELECT * FROM " + tableName + " WHERE "};
	ch = (values[parametrInd].second == Type::STR) ? "\"" : "";	
	sql += params[parametrInd].first + " = " + ch + secure(keyValue) + ch + ";";
	
	execute(sql, &Objects, getAllObj_callback);
	return Objects[0];
}

template <typename T>
void DataBase<T>::updateObj(vector<pair<size_t, string>>& updatePairs,
		string keyValue, size_t parametrInd, string tableName)
{
	T _Obj; string ch;
	vector<pair<string, string>> structure {_Obj.getTableStructure()};
	vector<pair<string, Type>> values {_Obj.getValues()};
	string sql {"UPDATE " + tableName + " SET "};

	for(size_t i {0}; i < updatePairs.size(); ++i) {
		sql += structure[updatePairs[i].first].first + " = ";
		sql += ch + updatePairs[i].second + ch;
		sql += (i < updatePairs.size() - 1) ? ", " : " ";
		ch = (values[updatePairs[i].first].second == Type::STR) ? "\"" : "";
	}

	sql += "WHERE " + structure[parametrInd].first + " = ";
	ch = (values[parametrInd].second == Type::STR) ? "\"" : "";
	sql += ch + keyValue + ch + ";";

	execute(sql);
}



