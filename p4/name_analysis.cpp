#include "ast.hpp"
#include "errors.hpp"

namespace cminusminus{



bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol * temp = new SemSymbol(0, getTypeNode()->getType(), ID()->getName());
	if (temp->isValid()) {
		symTab->insertSymbol(ID()->getName(), temp);
	}
	else {
		std::cerr << myPos->span() << ": Invalid type in declaration\n";
		nameAnalysisOk = false;
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	std::list<FormalDeclNode *> * formals = getFormals();
	std::string funct_type = "";
	bool firstFormal = true;
	for (auto formal : *formals) {
		if (firstFormal) { firstFormal = false; }
		else { funct_type += ", "; }
		funct_type += formal->getTypeNode()->getType();
	}
	funct_type += "->";
	funct_type += getTypeNode()->getType();
	SemSymbol * temp = new SemSymbol(1, funct_type, ID()->getName());
	symTab->insertSymbol(ID()->getName(), temp);
	symTab->addScopeTable();
	for (auto formal : *formals) {
		nameAnalysisOk = formal->nameAnalysis(symTab);
	}
	std::list<StmtNode *> * body = getBody();
	for (auto stmt : *body) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); 
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	symTab->addScopeTable();
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); 
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool IfStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myCond->nameAnalysis(symTab);
	symTab->addScopeTable();
	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); 
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = myCond->nameAnalysis(symTab);
	symTab->addScopeTable();
	for (auto stmt : *myBodyTrue) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents();
	symTab->removeScopeTable();

	symTab->addScopeTable();
	for (auto stmt : *myBodyFalse) {
		nameAnalysisOk = stmt->nameAnalysis(symTab);
	}
	symTab->printContents(); 
	symTab->removeScopeTable();
	return nameAnalysisOk;
}

bool ReadStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myDst->nameAnalysis(symTab);
}

bool WriteStmtNode::nameAnalysis(SymbolTable * symTab) {
	return mySrc->nameAnalysis(symTab);
}

bool PostDecStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLVal->nameAnalysis(symTab);
}

bool PostIncStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myLVal->nameAnalysis(symTab);
}

bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myCallExp->nameAnalysis(symTab);
}

bool CallExpNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myID->nameAnalysis(symTab);
	for (auto arg : *myArgs) {
		nameAnalysisOk = arg->nameAnalysis(symTab);
	}
	return nameAnalysisOk;
}

bool AssignStmtNode::nameAnalysis(SymbolTable * symTab) {
	return getExp()->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = myDst->nameAnalysis(symTab);
	nameAnalysisOk = mySrc->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool LValNode::nameAnalysis(SymbolTable* symTab) {
	std::cout <<"Lval called somehow\n";
	return false; 

}

bool DerefNode::nameAnalysis(SymbolTable* symTab) {
    bool nameAnalysisOk = myID->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool RefNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myID->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool IDNode::nameAnalysis(SymbolTable* symTab) {
	SemSymbol * symbol = symTab->findSymbol(name);
	if (symbol ==nullptr) {
		std::cerr << myPos->span() << "Undeclared identifier\n";
		return false;
	}
	attachSymbol(symbol);
	return true;
}

bool ExpNode::nameAnalysis(SymbolTable* symTab) {
	std::cout <<"How did it get here";
	return false;
}

bool BinaryExpNode::nameAnalysis(SymbolTable* symTab) {
	bool nameAnalysisOk = myExp1->nameAnalysis(symTab);
	nameAnalysisOk = myExp2->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool UnaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

}
