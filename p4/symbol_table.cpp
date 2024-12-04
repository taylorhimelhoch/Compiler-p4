#include "symbol_table.hpp"
#include <iostream>
namespace cminusminus{

// ----SYMBOL TABLE BEGINS---- //
SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
	addScopeTable(); 
}

void SymbolTable::addScopeTable() {
	scopeTableChain->push_front(new ScopeTable());
}

void SymbolTable::removeScopeTable() {
	scopeTableChain->pop_front();
}

void SymbolTable::insertSymbol(std::string symbName, SemSymbol* symbol) {
	scopeTableChain->front()->addSemSymbol(symbName, symbol);
}

SemSymbol* SymbolTable::findSymbol(std::string symbName) {
	std::string temp = symbName;
	for (auto scpTable : *scopeTableChain) {
		SemSymbol * temp = scpTable->findSymbol(symbName);
		if (temp != nullptr) return temp;
	}
	return nullptr;
}

void SymbolTable::printContents() {
	for (auto scpTable : *scopeTableChain) {
		std::cout << "--Scope Table--" << std::endl;
		scpTable->printContents();
		std::cout << "\n";
	}
}

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}
void ScopeTable::addSemSymbol(std::string symbName, SemSymbol * symbol) {
	symbols->insert(std::make_pair(symbName, symbol)); 
}
SemSymbol* ScopeTable::findSymbol(std::string symbName) {
	try 
	{
		return symbols->at(symbName);
	}
	catch (std::out_of_range e) {
		return nullptr;
	}
}
void ScopeTable::printContents() {
	for (auto i : *symbols) {
		i.second->printContents();
	}
}

void SemSymbol::printContents() {
	std::cout << "\tName: " << myName << std::endl;
	std::cout << "\tKind: ";
	if (myKind == 0) std::cout << "var\n";
	else std::cout << "fn\n";
	std::cout << "\tType: " << myType << "\n\n";
}

void SemSymbol::setKind(int k) {
	myKind = k;
}
void SemSymbol::setType(std::string t) {
	myType = t;
}
void SemSymbol::setName(std::string n) {
	myName = n;
}

int SemSymbol::getKind() {
	return myKind;
}
std::string SemSymbol::getType() {
	return myType;
}
std::string SemSymbol::getName() {
	return myName;
}
}
