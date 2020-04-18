
#include <vector>
#include <string>
#include <iostream>
#include "Symbol.h"
#include "Node.h"
#include "Debug.h"
std::ostream& operator<<(std::ostream& o, Node* n){
    n->string(o);
}
std::ostream& operator<<(std::ostream& o, ExpressionNode* n){
    n->string(o);
}
Node::~Node(){}
StartNode::StartNode(ProgramNode* pn){
    mProgramNode=pn;
}
StartNode::~StartNode(){
    MSG("deleting StartNode");
    delete(mProgramNode);
}
void StartNode::interpret(){
    mProgramNode->interpret();
}
std::ostream& StartNode::string(std::ostream& o){
    o<<"Start("<<mProgramNode<<")";
}
void StartNode::Code(InstructionsClass &machineCode)
{
	mProgramNode->Code(machineCode);
}
ProgramNode::ProgramNode(BlockNode* bn){
    mBlockNode=bn;
}
std::ostream& ProgramNode::string(std::ostream& o){
    o<<"Program("<<mBlockNode<<")";
}
ProgramNode::~ProgramNode(){
    MSG("deleting ProgramNode");
    delete(mBlockNode);
}
void ProgramNode::interpret(){
    mBlockNode->interpret();
}
void ProgramNode::Code(InstructionsClass &machineCode)
{
	mBlockNode->Code(machineCode);
}
IfStatementNode::~IfStatementNode(){
    delete(mStatement);
    delete(mExpression);
    if (mElse!=NULL){
        delete(mElse);
        mElse=NULL;
    }
}
IfStatementNode::IfStatementNode(StatementNode* sn, ExpressionNode* en)
: mStatement(sn),mExpression(en),mElse(NULL){
}
void IfStatementNode::AddElse(StatementNode* sn){
    if(mElse==NULL){
        mElse=sn;
    }
    else{
        std::cerr<<"Cannot have an 'else' after an 'else'";
        exit(1);
    }
}
std::ostream& IfStatementNode::string(std::ostream& o){
    o<<"If("<<mExpression<<","<<mStatement<<")";
    if (mElse!=NULL){
        o<<"Else("<<mElse<<")";
    }
    return o;
}
void IfStatementNode::interpret(){
    if (mExpression->Evaluate()){
        mStatement->interpret();
    }
    else if (mElse!=NULL){
        mElse->interpret();
    }
    
}
void IfStatementNode::Code(InstructionsClass &machineCode)
{
	mExpression->CodeEvaluate(machineCode);
	unsigned char * InsertAddress = machineCode.SkipIfZeroStack();
	unsigned char * address1 = machineCode.GetAddress();
	mStatement->Code(machineCode); // The True Case
    if (mElse!=NULL){
        unsigned char * InsertAddress2 = machineCode.Jump(); // Jump past False Case
        unsigned char * address2 = machineCode.GetAddress();
        machineCode.SetOffset(InsertAddress, (int)(address2-address1));
        mElse->Code(machineCode);
        unsigned char * address3 = machineCode.GetAddress();
        machineCode.SetOffset(InsertAddress2, (int)(address3-address2));
    }
    else{
        unsigned char * address2 = machineCode.GetAddress();
        machineCode.SetOffset(InsertAddress, (int)(address2-address1));
    }
    
}
WhileNode::~WhileNode(){
    delete(mStatement);
    delete(mExpression);
}
WhileNode::WhileNode(StatementNode* sn, ExpressionNode* en)
:    mStatement(sn),mExpression(en){
}
std::ostream& WhileNode::string(std::ostream& o){
    return o<<"While("<<mExpression<<","<<mStatement<<")";
}
void WhileNode::interpret(){
    while (mExpression->Evaluate()){
        mStatement->interpret();
    }
}
void WhileNode::Code(InstructionsClass &machineCode)
{
    unsigned char * address1 = machineCode.GetAddress();
	mExpression->CodeEvaluate(machineCode);
	unsigned char * InsertAddressSkip = machineCode.SkipIfZeroStack();
	unsigned char * address2 = machineCode.GetAddress();
	mStatement->Code(machineCode);

    unsigned char * InsertAddressJump = machineCode.Jump();
    unsigned char * address3 = machineCode.GetAddress();
    machineCode.SetOffset(InsertAddressSkip, (int)(address3-address2));
    machineCode.SetOffset(InsertAddressJump, (int)(address1-address3));
    
}
BlockNode::BlockNode(StatementGroupNode* sgn){
    mStatementGroupNode=sgn;
}
BlockNode::~BlockNode(){
    MSG("deleting BlockNode");
    delete(mStatementGroupNode);
}
std::ostream& BlockNode::string(std::ostream& o){
    return o<<"Block("<<mStatementGroupNode<<")";
}
void BlockNode::interpret(){
    mStatementGroupNode->interpret();
}
void BlockNode::Code(InstructionsClass &machineCode){
    mStatementGroupNode->Code(machineCode);
}
StatementGroupNode::StatementGroupNode(){
}
StatementGroupNode::~StatementGroupNode(){
    MSG("deleting StatementGroupNode");
    for (int i =0;i<mStatementNodes.size();i++){
        delete(mStatementNodes[i]);
    }
}
void StatementGroupNode::AddStatement(StatementNode* sn){
    mStatementNodes.push_back(sn);
}
std::ostream& StatementGroupNode::string(std::ostream& o){
    o<<"StatementGroup(";
    if (mStatementNodes.size()>0){
        o<<mStatementNodes[0];
    }
    for (int i=1;i<mStatementNodes.size();i++){
        o<<","<<mStatementNodes[i];
    }
    return o<<")";
}
void StatementGroupNode::interpret(){
    for (int i =0;i<mStatementNodes.size();i++){
        mStatementNodes[i]->interpret();
    }
}
void StatementGroupNode::Code(InstructionsClass &machineCode)
{
	for (int i =0;i<mStatementNodes.size();i++){
        mStatementNodes[i]->Code(machineCode);
    }
}
StatementNode::StatementNode(){}
StatementNode::~StatementNode(){}
DeclarationStatementNode::DeclarationStatementNode(IdentifierNode* in){
    mIdentifierNode=in;
}
DeclarationStatementNode::~DeclarationStatementNode(){
    MSG("deleting DeclarationStatementNode");
    delete(mIdentifierNode);
}
std::ostream& DeclarationStatementNode::string(std::ostream& o){
    return o<<"DeclarationStatement("<<mIdentifierNode<<")";
}
void DeclarationStatementNode::interpret(){
    mIdentifierNode->DeclareVariable();
}
void DeclarationStatementNode::Code(InstructionsClass &machineCode){
    mIdentifierNode->DeclareVariable();
}
AssignmentStatementNode::AssignmentStatementNode(IdentifierNode* in,ExpressionNode* en){
    mIdentifierNode=in;
    mExpressionNode=en;
}
AssignmentStatementNode::~AssignmentStatementNode(){
    MSG("deleting AssignmentStatementNode");
    delete(mIdentifierNode);
    delete(mExpressionNode);
}
std::ostream& AssignmentStatementNode::string(std::ostream& o){
    o<<"AssignmentStatement("<<mIdentifierNode<<","<<mExpressionNode<<")";
}
void AssignmentStatementNode::interpret(){
    mIdentifierNode->SetValue(mExpressionNode->Evaluate());
}
void AssignmentStatementNode::Code(InstructionsClass &machineCode){
    mExpressionNode->CodeEvaluate(machineCode);
    int index = mIdentifierNode->GetIndex();
    machineCode.PopAndStore(index);
}
PlusEqualNode::PlusEqualNode(IdentifierNode* in,ExpressionNode* en):AssignmentStatementNode(in,en){
    mIdentifierNode=in;
    mExpressionNode=en;
}
PlusEqualNode::~PlusEqualNode(){
    MSG("deleting PlusEqualNode");
    delete(mIdentifierNode);
    delete(mExpressionNode);
}
std::ostream& PlusEqualNode::string(std::ostream& o){
    o<<"PlusEqualNode("<<mIdentifierNode<<","<<mExpressionNode<<")";
}
void PlusEqualNode::interpret(){
    mIdentifierNode->SetValue(mIdentifierNode->Evaluate()+mExpressionNode->Evaluate());
}
void PlusEqualNode::Code(InstructionsClass &machineCode){
    machineCode.PushVariable(mIdentifierNode->GetIndex());
    mExpressionNode->CodeEvaluate(machineCode);
    machineCode.PopPopAddPush();
    int index = mIdentifierNode->GetIndex();
    machineCode.PopAndStore(index);
}
MinusEqualNode::MinusEqualNode(IdentifierNode* in,ExpressionNode* en):AssignmentStatementNode(in,en){
}
MinusEqualNode::~MinusEqualNode(){
    MSG("deleting MinusEqualNode");
    delete(mIdentifierNode);
    delete(mExpressionNode);
}
std::ostream& MinusEqualNode::string(std::ostream& o){
    o<<"MinusEqualNode("<<mIdentifierNode<<","<<mExpressionNode<<")";
}
void MinusEqualNode::interpret(){
    mIdentifierNode->SetValue(mIdentifierNode->Evaluate()-mExpressionNode->Evaluate());
}
void MinusEqualNode::Code(InstructionsClass &machineCode){
    machineCode.PushVariable(mIdentifierNode->GetIndex());
    mExpressionNode->CodeEvaluate(machineCode);
    machineCode.PopPopSubPush();
    int index = mIdentifierNode->GetIndex();
    machineCode.PopAndStore(index);
}
CoutStatementNode::CoutStatementNode(){
}
void CoutStatementNode::addExpression(ExpressionNode* en){
    mExpressionNodes.push_back(en);
}
CoutStatementNode::~CoutStatementNode(){
    MSG("deleting CoutStatementNode");
    for (int i =0;i<mExpressionNodes.size();i++){
        if (mExpressionNodes[i]!=NULL){
            delete(mExpressionNodes[i]);
        }
    }
}
std::ostream& CoutStatementNode::string(std::ostream& o){
    o<<"CoutStatement(";
    if (mExpressionNodes.size()>0){
        if (mExpressionNodes[0]!=NULL){
            o<<mExpressionNodes[0];
        }
        else{
            o<<"endl";
        }
    }
    for (int i=1;i<mExpressionNodes.size();i++){
        if (mExpressionNodes[i]!=NULL){
            o<<","<<mExpressionNodes[i];
        }
        else{
            o<<","<<"endl";
        }
    }
    return o<<")";
}
void CoutStatementNode::interpret(){
    for (int i=1;i<mExpressionNodes.size();i++){
        if (mExpressionNodes[i]!=NULL){
            std::cout<<mExpressionNodes[i]->Evaluate()<<" ";
        }
        else{
            std::cout<<std::endl;
        }
    }
}
void CoutStatementNode::Code(InstructionsClass &machineCode){
    for (int i=1;i<mExpressionNodes.size();i++){
        if (mExpressionNodes[i]!=NULL){
            mExpressionNodes[i]->CodeEvaluate(machineCode);
            machineCode.PopAndWrite();
        }
        else{
            machineCode.WriteEndl();
        }
    }
}
ExpressionNode::ExpressionNode(){}
ExpressionNode::~ExpressionNode(){}
IntegerNode::IntegerNode(int i){
    mI=i;
}

int IntegerNode::Evaluate(){
    return mI;
}
std::ostream& IntegerNode::string(std::ostream& o){
    o<<mI;
}
void IntegerNode::CodeEvaluate(InstructionsClass &machineCode){
    machineCode.PushValue(mI);
}
IdentifierNode::IdentifierNode(std::string label, SymbolTableClass* st){
    mLabel=label;
    mSymbolTable=st;
}
int IdentifierNode::Evaluate(){
    return mSymbolTable->GetValue(mLabel);
}
void IdentifierNode::DeclareVariable(){
    mSymbolTable->AddEntry(mLabel);
}
void IdentifierNode::SetValue(int v){
    mSymbolTable->SetValue(mLabel,v);
}
int IdentifierNode::GetIndex(){
    mSymbolTable->GetIndex(mLabel);
}
void IdentifierNode::CodeEvaluate(InstructionsClass &machineCode){
    machineCode.PushVariable(GetIndex());
}
std::ostream& IdentifierNode::string(std::ostream& o){
    o<<mLabel;
}
BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right)
: mLeft(left),mRight(right)
{
}
BinaryOperatorNode::~BinaryOperatorNode(){
    MSG("deleting BinaryOperatorNode");
    delete(mLeft);
    delete(mRight);
}
PlusNode::PlusNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int PlusNode::Evaluate(){
    return mLeft->Evaluate()+mRight->Evaluate();
}
std::ostream& PlusNode::string(std::ostream& o){
    o<<"("<<mLeft<<"+"<<mRight<<")";
}
void PlusNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopAddPush();
}
AndNode::AndNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int AndNode::Evaluate(){
    return mLeft->Evaluate() && mRight->Evaluate();
}
std::ostream& AndNode::string(std::ostream& o){
    o<<"("<<mLeft<<"&&"<<mRight<<")";
}
void AndNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopAndPush();
}
OrNode::OrNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int OrNode::Evaluate(){
    return mLeft->Evaluate() || mRight->Evaluate();
}
std::ostream& OrNode::string(std::ostream& o){
    o<<"("<<mLeft<<"||"<<mRight<<")";
}
void OrNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopOrPush();
}
MinusNode::MinusNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int MinusNode::Evaluate(){
    return mLeft->Evaluate()-mRight->Evaluate();
}
std::ostream& MinusNode::string(std::ostream& o){
    o<<"("<<mLeft<<"-"<<mRight<<")";
}
void MinusNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopSubPush();
}
TimesNode::TimesNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int TimesNode::Evaluate(){
    return mLeft->Evaluate()*mRight->Evaluate();
}
std::ostream& TimesNode::string(std::ostream& o){
    o<<"("<<mLeft<<"*"<<mRight<<")";
}
void TimesNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopMulPush();
}
DivideNode::DivideNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int DivideNode::Evaluate(){
    return mLeft->Evaluate()/mRight->Evaluate();
}
std::ostream& DivideNode::string(std::ostream& o){
    o<<"("<<mLeft<<"/"<<mRight<<")";
}
void DivideNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopDivPush();
}
LessNode::LessNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int LessNode::Evaluate(){
    return mLeft->Evaluate()<mRight->Evaluate();
}
std::ostream& LessNode::string(std::ostream& o){
    o<<"("<<mLeft<<"<"<<mRight<<")";
}
void LessNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopLessPush();
}
LessEqualNode::LessEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int LessEqualNode::Evaluate(){
    return mLeft->Evaluate()<=mRight->Evaluate();
}
std::ostream& LessEqualNode::string(std::ostream& o){
    o<<"("<<mLeft<<"<="<<mRight<<")";
}
void LessEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopLessEqualPush();
}
GreaterNode::GreaterNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int GreaterNode::Evaluate(){
    return mLeft->Evaluate()>mRight->Evaluate();
}
std::ostream& GreaterNode::string(std::ostream& o){
    o<<"("<<mLeft<<">"<<mRight<<")";
}
void GreaterNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopGreaterPush();
}
GreaterEqualNode::GreaterEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int GreaterEqualNode::Evaluate(){
    return mLeft->Evaluate()>=mRight->Evaluate();
}
std::ostream& GreaterEqualNode::string(std::ostream& o){
    o<<"("<<mLeft<<">="<<mRight<<")";
}
void GreaterEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopGreaterEqualPush();
}
EqualNode::EqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int EqualNode::Evaluate(){
    return mLeft->Evaluate()==mRight->Evaluate();
}
std::ostream& EqualNode::string(std::ostream& o){
    o<<"("<<mLeft<<"=="<<mRight<<")";
}
void EqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopEqualPush();
}
NotEqualNode::NotEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int NotEqualNode::Evaluate(){
    return mLeft->Evaluate()!=mRight->Evaluate();
}
std::ostream& NotEqualNode::string(std::ostream& o){
    o<<"("<<mLeft<<"!="<<mRight<<")";
}
void NotEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
	mLeft->CodeEvaluate(machineCode);
	mRight->CodeEvaluate(machineCode);
	machineCode.PopPopNotEqualPush();
}