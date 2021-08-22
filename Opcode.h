#ifndef GENERALCPPPROJECT_OPCODE_H
#define GENERALCPPPROJECT_OPCODE_H

#include <iostream>

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
    string operation;
    string left_operand;
    string right_operand;
    int byte_len{};
    bool is_legacy{};
    bool is_rex{};
    REX rex{};
    ModRM modRM{};

    explicit Opcode(const string &full_input);

    int get_offset(const string &input);

    void decide_if_legacy(const string &input);

    void decide_if_rex(const string &input);

    void decide_opcode_len(const string &input);

    void fill_rex(const char &rex_value);

    void read_operation(const string &input);
};


#endif //GENERALCPPPROJECT_OPCODE_H
