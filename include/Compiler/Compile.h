#ifndef COMPILER_COMPILE_H
#define COMPILER_COMPILE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <Compiler/List.h>
#include <Compiler/Token.h>
#include <Compiler/Variable.h>

/*
	C_BUILD_MAC: Build the compiler for 64-bit Mac assembly output
	C_BUILD_LINUX: Build the compiler for 32-bit Linux assembly output
*/
#define C_BUILD_LINUX

namespace Compiler {

    enum SymError {
        S_MISSING_VAR,
    };

	class Compile {
	private:
		List<Token*>* tokenList;
		List<char*>* nameList;
		
		List<Variable*>* varList;
		
		FILE* out;
		
	public:
		Compile();
		~Compile();
		
		void start();
		void writeVariables();
		void serror(int err, Token* t);
		
		void setTokenList(List<Token*>* tokenList);
		void setNameList(List<char*>* nameList);
		
		void writeLabel(char* label);
		void retLiteral(int num);
		void retVariable(Variable* v);
		
	private:
		#if defined(C_BUILD_MAC)
			static constexpr char* const ASM_TEMPLATE = "global start\n\n"
				"section .data\n"
				"db 0x00\n\n"
				"section .text\n"
				"start:\n"
				"\tcall main\n"
				"\tmov rdi, rdx\n"
				"\tmov rax, 0x2000001\n"
				"\tsyscall\n\n";
		#elif defined(C_BUILD_LINUX)
			static constexpr char* const ASM_TEMPLATE = "global _start\n\n"
				"section .text\n"
				"_start:\n"
				"\tcall main\n"
				"\tmov ebx, edx\n"
				"\tmov eax, 1\n"
				"\tint 0x80\n\n";
		#endif
	};

}

#endif // COMPILER_COMPILE_H