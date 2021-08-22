#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "Opcode.h"

using std::string;
int main() {

    while(true) {
        string input;
        std::getline(std::cin, input, '\n');
        for (auto & c: input) c = toupper(c);
        Opcode new_op(input);
        if(new_op.is_legacy)
            std::cout << "legacy\n";
        else
            std::cout << "not legacy\n";
        if(new_op.is_rex)
            std::cout << "rex\n";
        else
            std::cout << "not rex\n";
        std::cout << "opcode len = " << new_op.byte_len << "\n";
    }
    return 0;
}