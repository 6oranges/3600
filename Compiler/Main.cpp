#include "Scanner.h"
#include "Debug.h"
#include "Symbol.h"
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
	return 0;
}