#include "Opcode.h"
#include <stdexcept>
#include <bitset>

void Opcode::decide_if_legacy(const string &input) {
    is_legacy = false;
    if (input[0] == '6')
        if (input[1] == '6' || input[1] == '7')
            is_legacy = true;
}

void Opcode::decide_if_rex(const string &input) {
    is_rex = false;
    if (is_legacy) {
        if (input[3] == '4')
            is_rex = true;
    } else {
        if (input[0] == '4')
            is_rex = true;
    }
    if (is_rex) {
        if (is_legacy)
            fill_rex(input[4]);
        else
            fill_rex(input[1]);
    }
}

void Opcode::fill_rex(const char &rex_value) {

    int asc_val = (int) rex_value;
    if (isdigit(rex_value))
        asc_val -= 48;
    else if (isupper(rex_value))
        asc_val -= 55;
    else if (islower(rex_value))
        asc_val -= 87;
    else
        throw std::invalid_argument("REX is wrong");

    std::string binary = std::bitset<4>(asc_val).to_string();
    rex.W = binary[0] == '1';
    rex.R = binary[1] == '1';
    rex.X = binary[2] == '1';
    rex.B = binary[3] == '1';
}

void Opcode::decide_opcode_len(const string &input) {

    int offset;
    if (is_legacy) {
        if (is_rex)
            offset = 6;
        else
            offset = 3;
    } else {
        if (is_rex)
            offset = 3;
        else
            offset = 0;
    }

    if (input[offset + 0] != '0' || input[offset + 1] != 'F') {
        byte_len = 1;
        return;
    }
    if ((input[offset + 3] == '3' && input[offset + 4] == '8') ||
        (input[offset + 3] == '3' && input[offset + 4] == 'A')) {
        byte_len = 3;
        return;
    }
    byte_len = 2;
}

Opcode::Opcode(const string &full_input) {
    full = full_input;
    rex.W = false;
    rex.R = false;
    rex.X = false;
    rex.B = false;
    decide_if_legacy(full);
    decide_if_rex(full);
    decide_opcode_len(full);
}
