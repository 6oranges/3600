#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Symbol.h"
#include "Instructions.h"
class Node;
class StartNode;
class ProgramNode;
class BlockNode;
class StatementGroupNode;
class StatementNode;
class DeclarationStatementNode;
class AssignmentStatementNode;
class CoutStatementNode;
class ExpressionNode;
class IntegerNode;
class IdentifierNode;
class Node{
    public:
        virtual ~Node();
        friend std::ostream& operator<<(std::ostream& o, Node* n);
        virtual std::ostream& string(std::ostream& o)=0;
        virtual void interpret()=0;
        virtual void Code(InstructionsClass &machineCode)=0;
};
class StartNode: public Node { 
    public: 
        virtual ~StartNode();
        StartNode(ProgramNode* pn);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        ProgramNode* mProgramNode;
}; 
class ProgramNode: public Node { 
    public: 
        virtual ~ProgramNode();
        ProgramNode(BlockNode* bn);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        BlockNode* mBlockNode;
};
class StatementNode: public Node { 
    public: 
        virtual ~StatementNode();
        StatementNode();
    protected:
};
class IfStatementNode: public StatementNode { 
    public: 
        virtual ~IfStatementNode();
        IfStatementNode(StatementNode* sn, ExpressionNode* en);
        void AddElse(StatementNode* sn);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        StatementNode* mStatement;
        ExpressionNode* mExpression;
        StatementNode* mElse;

};
class WhileNode: public StatementNode { 
    public: 
        virtual ~WhileNode();
        WhileNode(StatementNode* sn, ExpressionNode* en);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        StatementNode* mStatement;
        ExpressionNode* mExpression;

};
class BlockNode: public StatementNode { 
    public: 
        virtual ~BlockNode();
        BlockNode(StatementGroupNode* sgn);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        StatementGroupNode* mStatementGroupNode;
};
class StatementGroupNode: public Node { 
    public: 
        virtual ~StatementGroupNode();
        StatementGroupNode();
        void AddStatement(StatementNode* sn);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        std::vector<StatementNode*> mStatementNodes;
};

class DeclarationStatementNode: public StatementNode { 
    public: 
        virtual ~DeclarationStatementNode();
        DeclarationStatementNode(IdentifierNode* in);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        IdentifierNode* mIdentifierNode;
};
class AssignmentStatementNode: public StatementNode { 
    public: 
        virtual ~AssignmentStatementNode();
        AssignmentStatementNode(IdentifierNode* in, ExpressionNode* en);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        IdentifierNode* mIdentifierNode;
        ExpressionNode* mExpressionNode;
};
class CoutStatementNode: public StatementNode { 
    public: 
        virtual ~CoutStatementNode();
        CoutStatementNode(ExpressionNode* en);
        virtual std::ostream& string(std::ostream& o);
        virtual void interpret();
        virtual void Code(InstructionsClass &machineCode);
    protected:
        ExpressionNode* mExpressionNode;
};
class ExpressionNode{
    public:
        virtual ~ExpressionNode();
        virtual int Evaluate()=0;
        ExpressionNode();
        friend std::ostream& operator<<(std::ostream& o, ExpressionNode* n);
        virtual std::ostream& string(std::ostream& o)=0;
        virtual void CodeEvaluate(InstructionsClass &machineCode)=0;
};
class IntegerNode: public ExpressionNode{
    public:
        virtual int Evaluate();
        IntegerNode(int i);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
    protected:
        int mI;
};
class IdentifierNode: public ExpressionNode{
    public:
        virtual int Evaluate();
        void DeclareVariable();
        void SetValue(int v);
        int GetIndex();
        IdentifierNode(std::string label, SymbolTableClass* st);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
    protected:
        std::string mLabel;
        SymbolTableClass* mSymbolTable;
};
class BinaryOperatorNode: public ExpressionNode{
    public:
        //virtual int Evaluate();
        virtual ~BinaryOperatorNode();
        BinaryOperatorNode(ExpressionNode* left,ExpressionNode* right);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
    protected:
        ExpressionNode* mLeft;
        ExpressionNode* mRight;
};
class PlusNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        PlusNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class MinusNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        MinusNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class TimesNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        TimesNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class DivideNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        DivideNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class LessNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        LessNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class AndNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        AndNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class OrNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        OrNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class LessEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        LessEqualNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class GreaterNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        GreaterNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class GreaterEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        GreaterEqualNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class EqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        EqualNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};
class NotEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        NotEqualNode(ExpressionNode* left,ExpressionNode* right);
        virtual std::ostream& string(std::ostream& o);
        virtual void CodeEvaluate(InstructionsClass &machineCode);
};