#include <Compiler/Compile.h>

namespace Compiler {

	Compile::Compile() {
		this->out = fopen("out.s", "w");
		fprintf(this->out, "%s", ASM_TEMPLATE);
	}
	
	Compile::~Compile() {
		fclose(this->out);
	}
	
	void Compile::start() {
		Token* t;
		Token** tok = tokenList->getRawData();
		
		for (int i = 0; i < tokenList->getSize(); i++) {
			t = tok[i];
			
			// Function returning an integer
			if (t->getType() == TokenType::TYPE && t->getData() == TokenKeyword::INT &&
					tok[i+1]->getType() == TokenType::NAME &&
					tok[i+2]->getType() == TokenType::SPECIAL && tok[i+2]->getData() == TokenSpecial::LEFT_PAR &&
					tok[i+3]->getType() == TokenType::SPECIAL && tok[i+3]->getData() == TokenSpecial::RIGHT_PAR) {
				char* name = strdup(nameList->get(tok[i+1]->getData()));
				writeLabel(name);
			}
			
			// Return a number
			else if (t->getType() == TokenType::KEYWORD && t->getData() == TokenKeyword::RETURN &&
					tok[i+1]->getType() == TokenType::NUMBER &&
					tok[i+2]->getType() == TokenType::SPECIAL && tok[i+2]->getData() == TokenSpecial::SEMICOLON) {
				retLiteral(tok[i+1]->getData());
				//i += 2;
			}
		}
	}
	
	void Compile::writeLabel(char* label) {
		fprintf(this->out, "%s:\n", label);
	}
	
	void Compile::retLiteral(int num) {
	    #if defined(C_BUILD_MAC)
		    fprintf(this->out, "\tmov rdx, %d\n", num);
		#elif defined(C_BUILD_LINUX)
		    fprintf(this->out, "\tmov edx, %d\n", num);
		#endif
		fprintf(this->out, "%s", "\tret\n\n");
	}
	
	void Compile::setTokenList(List<Token*>* tokenList) {
		this->tokenList = tokenList;
	}
	
	void Compile::setNameList(List<char*>* nameList) {
		this->nameList = nameList;
	}

}