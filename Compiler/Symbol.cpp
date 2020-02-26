#include "Symbol.h"
#include <iostream>

bool SymbolTableClass::Exists(const std::string & s){
	return GetIndex(s)>=0;
}
// returns true if <s> is already in the symbol table.
void SymbolTableClass::AddEntry(const std::string & s){
	int i =GetIndex(s);
	if (i>-1){
		std::cout<<"AddEntry for string: "<<s<<" already exists"<<std::endl;
		exit(1);
	}
	mLabels.push_back(s);
	mValues.push_back(0);
}
// adds <s> to the symbol table, 
// or quits if it was already there
int SymbolTableClass::GetValue(const std::string & s){
	int i =GetIndex(s);
	if (i==-1){
		std::cout<<"GetValue for string: "<<s<<" does not have variable of that name"<<std::endl;
		exit(1);
	}
	return mValues[i];
}
// returns the current value of variable <s>, when
// interpreting. Meaningless for Coding and Executing.
// Prints a message and quits if variable s does not exist.
void SymbolTableClass::SetValue(const std::string & s, int v){
	int i =GetIndex(s);
	if (i==-1){
		std::cout<<"SetValue for string: "<<s<<" does not have variable of that name"<<std::endl;
		exit(1);
	}
	mValues[i]=v;
}
// sets variable <s> to the given value, when interpreting.
// Meaningless for Coding and Executing.
// Prints a message and quits if variable s does not exist.
int SymbolTableClass::GetIndex(const std::string & s){
	for (int i=0;i<mLabels.size();i++){
		if (mLabels[i]==s){
			return i;
		}
	}
	return -1;
}
// returns the index of where variable <s> is.
// returns -1 if variable <s> is not there.
int SymbolTableClass::GetCount(){
	return mLabels.size();
}
// returns the current number of variables in the symbol
// table.  