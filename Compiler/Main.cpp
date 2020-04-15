#include "Scanner.h"
#include "Debug.h"
#include "Symbol.h"
#include "Node.h"
#include "Parser.h"
#include "Instructions.h"
#include<string>
void CodeAndExecute(std::string inputFile)
{
	// Create scanner, symbol table, and parser objects.
	ScannerClass scanner(inputFile);
	SymbolTableClass symbolTable;
	ParserClass parser(&scanner, &symbolTable);

	// Do the parsing, which results in a parse tree.
	StartNode * root = parser.Start();

	// Create the machine code instructions from the parse tree
	InstructionsClass machineCode;
	root->Code(machineCode);
	machineCode.Finish();
	machineCode.PrintAllMachineCodes();

	// Execute the machine code instructions previously created
	machineCode.Execute();

	// cleanup recursively
	delete root;
}
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
/* blue pink
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
	delete(sn);*/

/*
	ScannerClass sc("text.txt");
	//std::cout<<sc.PeekNextToken();
	SymbolTableClass stc = SymbolTableClass();
	ParserClass p = ParserClass(&sc,&stc);
	StartNode* s=p.Start();
	std::cout<<std::endl;
	std::cout<<s<<std::endl;
	s->interpret();
	delete s;
	return 0;*/
	
	
	/*InstructionsClass i;
	i.Finish();
	i.Execute();
	std::cout<<"Applesause"<<std::endl;*/
	/*
	InstructionsClass code;
	code.PushValue(1000);
	code.PopAndWrite(); // Should print 1000


	code.mData[10] = 2000; // Tenth variable in symbol table has value 2000
	code.PushVariable(10);
	code.PopAndWrite(); // Should print 2000

	code.mData[10] = 2000;
	code.PushVariable(10);
	code.PopAndStore(11);
	code.PushVariable(10);
	code.PushVariable(11);
	code.PopPopAddPush();
	code.PopAndWrite(); // Should print 2000
	code.Finish();
	code.PrintAllMachineCodes();
	code.Execute();
	*/
	std::string inputfile;
	std::cout<<"File: ";
	std::cin>>inputfile;
	CodeAndExecute(inputfile);
}