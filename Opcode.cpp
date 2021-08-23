#include "Opcode.h"
#include <stdexcept>
#include <bitset>
#include <algorithm>

Opcode::Opcode(const string &full_input) {
    full = full_input;
    is_legacy = false;
    is_legacy_16 = false;
    is_legacy_32 = false;
    is_rex = false;
    rex.W = false;
    rex.R = false;
    rex.X = false;
    rex.B = false;
    fill_bytes(full);
    decide_if_legacy();
    decide_if_rex();
    decide_opcode_len(full);
    read_operation();
    if (byte_len == 1) {
        analyze_one_byte_opcode();
    }
}

int Opcode::get_offset(const string &input) const {
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
    return offset;
}

void Opcode::decide_if_legacy() {
    if (bytes_of_op[0].first != '6')
        return;
    if (bytes_of_op[0].second == '6') {
        is_legacy = true;
        is_legacy_16 = true;
    } else if (bytes_of_op[0].second == '7') {
        is_legacy = true;
        is_legacy_32 = true;
    }
}

void Opcode::decide_if_rex() {
    if (is_legacy) {
        if (bytes_of_op[1].first == '4') {
            is_rex = true;
            fill_rex(bytes_of_op[1].second);
        }
    } else {
        if (bytes_of_op[0].first == '4') {
            is_rex = true;
            fill_rex(bytes_of_op[0].second);
        }
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

    int offset = get_offset(input);
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


void Opcode::read_operation() {
    int offset = 0;
    if (is_legacy)
        offset++;
    if (is_rex)
        offset++;

    if (byte_len == 1) {
        operation = bytes_of_op[offset].first;
        operation += bytes_of_op[offset].second;
    } else if (byte_len == 2) {
        operation = bytes_of_op[offset + 1].first;
        operation += bytes_of_op[offset + 1].second;
    } else if (byte_len == 3) {
        operation = bytes_of_op[offset + 2].first;
        operation += bytes_of_op[offset + 2].second;
    }

}

void Opcode::fill_bytes(const string &input) {
    string tmp = input;
    // remove spaces.
    string::iterator end_pos = std::remove(tmp.begin(), tmp.end(), ' ');
    tmp.erase(end_pos, tmp.end());

    // this is to ensure even pairs in bytes - add dummy char at end.
    if (tmp.length() % 2 != 0)
        tmp += '-';
    Byte cur_byte = std::make_pair(' ', ' ');
    for (int i = 0; i < tmp.length() - 1; i += 2) {
        cur_byte.first = tmp[i];
        cur_byte.second = tmp[i + 1];
        bytes_of_op.push_back(cur_byte);
    }
}

void Opcode::analyze_one_byte_opcode() {
    int hex_val = std::stoi(operation, 0, 16);
    std::cout << tmp_op_table[hex_val].instr << std::endl;
}