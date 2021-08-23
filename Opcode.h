#ifndef GENERALCPPPROJECT_OPCODE_H
#define GENERALCPPPROJECT_OPCODE_H

#include <iostream>
#include <vector>
#include <utility>
#include "OpcodesTable.h"
using std::string;

struct REX {
    bool W, R, X, B;
};

struct ModRM {
    bool mod[2];
    bool reg[3];
    bool rm[3];
};

class Opcode {
public:
    string full;
    std::vector<Byte> bytes_of_op{};
    string operation;
    int byte_len{};
    bool is_legacy;
    bool is_legacy_16;
    bool is_legacy_32;
    bool is_rex{};
    REX rex{};
    ModRM modRM{};

    explicit Opcode(const string &full_input);

    int get_offset(const string &input) const;

    void decide_if_legacy();

    void decide_if_rex();

    void decide_opcode_len(const string &input);

    void fill_rex(const char &rex_value);

    void fill_bytes(const string &input);

    void read_operation();

    void analyze_one_byte_opcode();
};


#endif //GENERALCPPPROJECT_OPCODE_H
