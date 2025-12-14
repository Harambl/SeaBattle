#ifndef USER_H
#define USER_H

#define MAX_LEN			128
#define MAX_HASH_LEN		128

#include "abstractData.h"
#include <stdexcept>
#include <string>

using std::string;


struct User {

	string name;
	string passHash;	
	string winCount;
	string loseCount;
	string matchCount;

	static vector<pair<string, string>> getTableStructure()
	{
		char type1[32] {""}, type2[32] {""},
		     type3[32] {""}, type4[32] {""}, type5[32] {""};
		sprintf(type1, "VARCHAR(%d) UNIQUE NOT NULL", MAX_LEN);
		sprintf(type2, "INT");
		sprintf(type3, "INT");
		sprintf(type4, "INT");
		sprintf(type5, "INT");

		return {
	   	      	{"name", type1},
			{"passHash", type2},
			{"winCount", type3},
			{"loseCount", type4},
			{"matchCount", type5}
		};
	}

	static User initialize(int argc, char** argv)
	{
		if(argc != 6 + 1)
			throw std::runtime_error("invalid argument count");
		User u;
		u.name = argv[1];
		u.passHash = argv[2];
		u.winCount = std::stoi(argv[3]);
		u.loseCount = std::stoi(argv[4]);
		u.matchCount = std::stoi(argv[5]);
		return u;	
	}

	vector<pair<string, Type>> getValues() const
	{
		return { {name, Type::STR},
			 {passHash, Type::INT},
			 {winCount, Type::INT},
			 {loseCount, Type::INT},
			 {matchCount, Type::INT}
		};
	}

	string formated()
	{
		string s {""};
		s +=    "name: " + name;
		s += " | password hash: " + passHash;
		s += " | winCount" + winCount;
		s += " | loseCount" + loseCount;
		s += " | matchCount" + matchCount;
		return s;
	}

	bool operator==(const User& other)
	{
		bool cond1 = (strcmp(name.c_str(), other.name.c_str()) == 0);
	       	bool cond2 = (strcmp(passHash.c_str(), other.passHash.c_str()) == 0);	
	       	bool cond3 = (strcmp(winCount.c_str(), other.winCount.c_str()) == 0);	
	       	bool cond4 = (strcmp(loseCount.c_str(), other.loseCount.c_str()) == 0);	
	       	bool cond5 = (strcmp(matchCount.c_str(), other.matchCount.c_str()) == 0);	
		return cond1 && cond2 && cond3 && cond4 && cond5;
	}
};


#endif
