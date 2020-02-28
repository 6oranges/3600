#include <vector>
#include <string>
#include "Symbol.h"
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
};
class StartNode: public Node { 
    public: 
        virtual ~StartNode();
        StartNode(ProgramNode* pn);
    protected:
        ProgramNode* mProgramNode;
}; 
class ProgramNode: public Node { 
    public: 
        virtual ~ProgramNode();
        ProgramNode(BlockNode* bn);
    protected:
        BlockNode* mBlockNode;
};
class StatementNode: public Node { 
    public: 
        virtual ~StatementNode();
        StatementNode();
    protected:
};
class BlockNode: public StatementNode { 
    public: 
        virtual ~BlockNode();
        BlockNode(StatementGroupNode* sgn);
    protected:
        StatementGroupNode* mStatementGroupNode;
};
class StatementGroupNode: public Node { 
    public: 
        virtual ~StatementGroupNode();
        StatementGroupNode();
        void AddStatement(StatementNode* sn);
    protected:
        std::vector<StatementNode*> mStatementNodes;
};

class DeclarationStatementNode: public StatementNode { 
    public: 
        virtual ~DeclarationStatementNode();
        DeclarationStatementNode(IdentifierNode* in);
    protected:
        IdentifierNode* mIdentifierNode;
};
class AssignmentStatementNode: public StatementNode { 
    public: 
        virtual ~AssignmentStatementNode();
        AssignmentStatementNode(IdentifierNode* in, ExpressionNode* en);
    protected:
        IdentifierNode* mIdentifierNode;
        ExpressionNode* mExpressionNode;
};
class CoutStatementNode: public StatementNode { 
    public: 
        virtual ~CoutStatementNode();
        CoutStatementNode(ExpressionNode* en);
    protected:
        ExpressionNode* mExpressionNode;
};
class ExpressionNode{
    public:
        virtual ~ExpressionNode();
        virtual int Evaluate()=0;
        ExpressionNode();
};
class IntegerNode: public ExpressionNode{
    public:
        virtual int Evaluate();
        IntegerNode(int i);
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
    protected:
        std::string mLabel;
        SymbolTableClass* mSymbolTable;
};
class BinaryOperatorNode: public ExpressionNode{
    public:
        //virtual int Evaluate();
        virtual ~BinaryOperatorNode();
        BinaryOperatorNode(ExpressionNode* left,ExpressionNode* right);
    protected:
        ExpressionNode* mLeft;
        ExpressionNode* mRight;
};
class PlusNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        PlusNode(ExpressionNode* left,ExpressionNode* right);
};
class MinusNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        MinusNode(ExpressionNode* left,ExpressionNode* right);
};
class TimesNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        TimesNode(ExpressionNode* left,ExpressionNode* right);
};
class DivideNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        DivideNode(ExpressionNode* left,ExpressionNode* right);
};
class LessNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        LessNode(ExpressionNode* left,ExpressionNode* right);
};
class LessEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        LessEqualNode(ExpressionNode* left,ExpressionNode* right);
};
class GreaterNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        GreaterNode(ExpressionNode* left,ExpressionNode* right);
};
class GreaterEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        GreaterEqualNode(ExpressionNode* left,ExpressionNode* right);
};
class EqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        EqualNode(ExpressionNode* left,ExpressionNode* right);
};
class NotEqualNode: public BinaryOperatorNode{
    public:
        virtual int Evaluate();
        NotEqualNode(ExpressionNode* left,ExpressionNode* right);
};