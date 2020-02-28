
#include <vector>
#include <string>
#include "Symbol.h"
#include "Node.h"
#include "debug.h"
Node::~Node(){}
StartNode::StartNode(ProgramNode* pn){
    mProgramNode=pn;
}
StartNode::~StartNode(){
    MSG("deleting StartNode");
    delete(mProgramNode);
}
ProgramNode::ProgramNode(BlockNode* bn){
    mBlockNode=bn;
}
ProgramNode::~ProgramNode(){
    MSG("deleting ProgramNode");
    delete(mBlockNode);
}
BlockNode::BlockNode(StatementGroupNode* sgn){
    mStatementGroupNode=sgn;
}
BlockNode::~BlockNode(){
    MSG("deleting BlockNode");
    delete(mStatementGroupNode);
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
StatementNode::StatementNode(){}
StatementNode::~StatementNode(){}
DeclarationStatementNode::DeclarationStatementNode(IdentifierNode* in){
    mIdentifierNode=in;
}
DeclarationStatementNode::~DeclarationStatementNode(){
    MSG("deleting DeclarationStatementNode");
    delete(mIdentifierNode);
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
CoutStatementNode::CoutStatementNode(ExpressionNode* en){
    mExpressionNode=en;
}
CoutStatementNode::~CoutStatementNode(){
    MSG("deleting CoutStatementNode");
    delete(mExpressionNode);
}
ExpressionNode::ExpressionNode(){}
ExpressionNode::~ExpressionNode(){}
IntegerNode::IntegerNode(int i){
    int mI=i;
}
int IntegerNode::Evaluate(){
    return mI;
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
MinusNode::MinusNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int MinusNode::Evaluate(){
    return mLeft->Evaluate()-mRight->Evaluate();
}
TimesNode::TimesNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int TimesNode::Evaluate(){
    return mLeft->Evaluate()*mRight->Evaluate();
}
DivideNode::DivideNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int DivideNode::Evaluate(){
    return mLeft->Evaluate()/mRight->Evaluate();
}
LessNode::LessNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int LessNode::Evaluate(){
    return mLeft->Evaluate()<mRight->Evaluate();
}
LessEqualNode::LessEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int LessEqualNode::Evaluate(){
    return mLeft->Evaluate()<=mRight->Evaluate();
}
GreaterNode::GreaterNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int GreaterNode::Evaluate(){
    return mLeft->Evaluate()>mRight->Evaluate();
}
GreaterEqualNode::GreaterEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int GreaterEqualNode::Evaluate(){
    return mLeft->Evaluate()>=mRight->Evaluate();
}
EqualNode::EqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int EqualNode::Evaluate(){
    return mLeft->Evaluate()==mRight->Evaluate();
}
NotEqualNode::NotEqualNode(ExpressionNode * left, ExpressionNode * right) 
: BinaryOperatorNode(left, right) 
{
}
int NotEqualNode::Evaluate(){
    return mLeft->Evaluate()!=mRight->Evaluate();
}