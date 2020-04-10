
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
CoutStatementNode::CoutStatementNode(ExpressionNode* en){
    mExpressionNode=en;
}
CoutStatementNode::~CoutStatementNode(){
    MSG("deleting CoutStatementNode");
    delete(mExpressionNode);1+1;
}
std::ostream& CoutStatementNode::string(std::ostream& o){
    o<<"CoutStatement("<<mExpressionNode<<")";
}
void CoutStatementNode::interpret(){
    std::cout<<mExpressionNode->Evaluate()<<std::endl;
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
