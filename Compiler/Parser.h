#include "Symbol.h"
#include "Scanner.h"
class ParserClass{
    public:
        ParserClass(ScannerClass* sc, SymbolTableClass* stc);
        void Start();
    private:
        TokenClass Match(TokenType expectedType);
        void Program();
        void Block();
        void StatementGroup();
        bool Statement();
        void DeclarationStatement();
        void AssignmentStatement();
        void CoutStatement();
        void Expression();
        void Relational();
        void PlusMinus();
        void TimesDivide();
        void Factor();
        void Identifier();
        void Integer();
        ScannerClass* mScanner;
        SymbolTableClass* mSymbolTable;
};