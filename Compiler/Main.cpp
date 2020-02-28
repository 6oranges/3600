#include "Scanner.h"
#include "Debug.h"
#include "Symbol.h"
#include "Node.h"
#include "Parser.h"
int main() {
	/*ScannerClass scanner("Text.txt");
	while (true) {
		TokenClass tc = scanner.GetNextToken();
		int l = scanner.GetLineNumber(), c = scanner.GetColumn();
		std::cout << "<" << l << ":" << c-tc.GetLexeme().length() << ">: " << tc << std::endl;
		if (tc.GetTokenType() == ENDFILE_TOKEN) {
			break;
		}
	}*/
	/*
	SymbolTableClass s;
	std::cout << s.Exists("banana")<<std::endl;
	s.AddEntry("banana");
	std::cout << s.Exists("banana")<<std::endl;
	std::cout<<s.GetCount();
	std::cout<<s.GetIndex("banana")<<std::endl;
	std::cout<<s.GetValue("banana")<<std::endl;
	s.SetValue("banana",5);
	std::cout<<s.GetValue("banana")<<std::endl;
	std::cout << s.Exists("barbaque")<<std::endl;
	std::cout<<s.GetIndex("barbaque")<<std::endl;
	s.GetValue("barbaque");
	*/
	SymbolTableClass st;
	StatementGroupNode* sgn = new StatementGroupNode();
	IdentifierNode* in = new IdentifierNode("sum",&st);
	DeclarationStatementNode* ds = new DeclarationStatementNode(in);
	sgn->AddStatement(ds);
	IdentifierNode* in2 = new IdentifierNode("sum",&st);
	IntegerNode* i1= new IntegerNode(35);
	IntegerNode* i2= new IntegerNode(400);
	ExpressionNode* en = new PlusNode(i1,i2);
	AssignmentStatementNode* as = new AssignmentStatementNode(in2,en);
	sgn->AddStatement(as);
	IdentifierNode* in3 = new IdentifierNode("sum",&st);
	CoutStatementNode* cs = new CoutStatementNode(in3);
	sgn->AddStatement(cs);
	BlockNode* bn = new BlockNode(sgn);
	ProgramNode* pn = new ProgramNode(bn);
	StartNode* sn = new StartNode(pn);
	delete(sn);


	ScannerClass sc("text.txt");
	SymbolTableClass stc = SymbolTableClass();
	ParserClass p = ParserClass(&sc,&stc);
	p.Start();
	return 0;
}