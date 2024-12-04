#ifndef CMINUSMINUS_SYMBOL_TABLE_HPP
#define CMINUSMINUS_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>


template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace cminusminus{


class SemSymbol {
	
	public:
		SemSymbol(int kind, std::string type, std::string name) 
		: myKind(kind), myType(type), myName(name)  {
			if (myType == "void") {
				valid = false;
			}
			else {
				valid = true;
			}
		}
		void setKind(int k);
		void setType(std::string t);
		void setName(std::string n);
		int getKind();
		std::string getType();
		std::string getName();
		bool isValid() {return valid;}
		void printContents();
	private:
		int myKind;
		std::string myType;
		std::string myName; 
		bool valid;
};


class ScopeTable {
	public:
		ScopeTable();
		void addSemSymbol(std::string symbName, SemSymbol * symbol);
		SemSymbol * findSymbol(std::string symbName);
		void printContents();
		
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		void addScopeTable();
		void removeScopeTable();
		SemSymbol* findSymbol(std::string symbName);
		bool isInCurrentScope(std::string symbName) {
			SemSymbol* temp = scopeTableChain->front()->findSymbol(symbName);
			if (temp != nullptr) return true;
			return false;
		}
		void insertSymbol(std::string symbName, SemSymbol* symbol);
		void printContents();
		
	private:
		std::list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
