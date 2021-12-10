//
// Created by maozunyao on 2021/12/6.
//

#ifndef CS323_PROJECT1_INTERMEDIATECODE_H
#define CS323_PROJECT1_INTERMEDIATECODE_H

#include <string>

enum class OperandType {
    VARIABLE,
    CONSTANT,
    ADDRESS,
    POINTER,
    JUMP_LABEL
};

enum class IntermediateCodeType {
    ASSIGN,
    CALL,
    CONSTANT,
    BINARY,
    MINUS,
    LABEL,
    GOTO,
    FUNCTION,
    RETURN, PARAM, ARG, READ, WRITE,
    DEC,
    IF_GOTO, IF_ELSE,
    ASSIGN_ADDRESS, ASSIGN_VALUE_IN_ADDRESS, COPY_VALUE_TO_ADDRESS
};

class Operand {
public:
    Operand(OperandType type);

    Operand(OperandType type, std::string &variableName);

    OperandType operandEnum;

    std::string variName;
    std::string constant;
    std::string jumpLabel;
};

class IntermediateCode {
public:
    IntermediateCodeType intermediateCodeEnum;
    Operand *result;
    Operand *op1;
    Operand *op2;
    std::string relation;

    IntermediateCode(IntermediateCodeType type);

    void print();
};

IntermediateCode *createCallFuntionCode(std::string &rtName, std::string &funtionName);

IntermediateCode *createReadCode(std::string &variable);

IntermediateCode *createAssignCode(std::string &left, std::string &right);

IntermediateCode *createWriteCode(std::string &variable);

IntermediateCode *createArgsCode(std::string &variable);

IntermediateCode *createReturnCode(std::string &returnValue);

IntermediateCode *
createIfGOTOCode(std::string &temp_left, std::string &temp_right, std::string &relation, std::string label);

IntermediateCode *createGOTOCode(std::string &label);

IntermediateCode *createLabelCode(std::string &label);

IntermediateCode *createConstantCode(std::string &place, std::string &value);

IntermediateCode *
createBinaryCode(std::string &result, std::string &oprand1, std::string &oprand2, std::string &binaryOperator);

IntermediateCode *createMinusCode(std::string &result, std::string &oprand1);

IntermediateCode *createFuntionDecCode(std::string &funtionName);

#endif //CS323_PROJECT1_INTERMEDIATECODE_H
