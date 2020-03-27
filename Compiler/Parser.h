#include "Symbol.h"
#include "Scanner.h"
#include "Node.h"
class ParserClass{
    public:
        ParserClass(ScannerClass* sc, SymbolTableClass* stc);
        StartNode* Start();
    private:
        TokenClass Match(TokenType expectedType);
        ProgramNode* Program();
        BlockNode* Block();
        StatementGroupNode* StatementGroup();
        StatementNode* Statement();
        IfStatementNode* IfStatement();
        WhileNode* WhileStatement();
        DeclarationStatementNode* DeclarationStatement();
        AssignmentStatementNode* AssignmentStatement();
        CoutStatementNode* CoutStatement();
        ExpressionNode* Expression();
        ExpressionNode* Or();
        ExpressionNode* And();
        ExpressionNode* Relational();
        ExpressionNode* PlusMinus();
        ExpressionNode* TimesDivide();
        ExpressionNode* Factor();
        IdentifierNode* Identifier();
        IntegerNode* Integer();
        ScannerClass* mScanner;
        SymbolTableClass* mSymbolTable;
};