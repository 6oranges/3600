#include "Scanner.h"
#include "Debug.h"
int main() {
	ScannerClass scanner("Text.txt");
	while (true) {
		TokenClass tc = scanner.GetNextToken();
		int l = scanner.GetLineNumber(), c = scanner.GetColumn();
		std::cout << "<" << l << ":" << c-tc.GetLexeme().length() << ">: " << tc << std::endl;
		if (tc.GetTokenType() == ENDFILE_TOKEN) {
			break;
		}
	}
	return 0;
}