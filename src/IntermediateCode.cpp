//
// Created by maozunyao on 2021/12/6.
//

#include <iostream>
#include "IntermediateCode.h"

IntermediateCode::IntermediateCode(IntermediateCodeType type) {
    this->intermediateCodeEnum = type;
    this->result = nullptr;
    this->op1 = nullptr;
    this->op2 = nullptr;
}

void IntermediateCode::print() {
    switch (this->intermediateCodeEnum) {
        case IntermediateCodeType::ASSIGN: {
            std::cout << this->op1->var_name_ << " := " << this->op2->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::CALL: {
            if (this->result->var_name_ != "null") {
                std::cout << this->result->var_name_ << " := CALL " << this->op1->var_name_ << std::endl;
            } else {
                std::cout << "CALL " << this->op1->var_name_ << std::endl;
            }
            break;
        }
        case IntermediateCodeType::CONSTANT: {
            std::cout << this->op1->var_name_ << " := #" << this->op2->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::BINARY: {
            std::cout << this->result->var_name_ << " := " << this->op1->var_name_ << " " << this->relation << " "
                      << this->op2->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::MINUS: {
            std::cout << this->result->var_name_ << " := #0 - " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::LABEL: {
            std::cout << "LABEL " << this->op1->var_name_ << " :" << std::endl;
            break;
        }
        case IntermediateCodeType::GOTO: {
            std::cout << "GOTO " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::FUNCTION: {
            std::cout << "FUNCTION " << this->op1->var_name_ << " :" << std::endl;
            break;
        }
        case IntermediateCodeType::RETURN: {
            std::cout << "RETURN " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::ARG: {
            std::cout << "ARG " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::READ: {
            std::cout << "READ " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::WRITE: {
            std::cout << "WRITE " << this->op1->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::IF_GOTO: {
            std::cout << "IF " << this->op1->var_name_ << " " << this->relation << " " << this->op2->var_name_ << " GOTO "
                      << this->result->var_name_ << std::endl;
            break;
        }
        case IntermediateCodeType::PARAM: {
            std::cout << "PARAM " << this->op1->var_name_ << std::endl;
            break;
        }
        default: {
            std::cout << "error!" << std::endl;
            break;
        }
    }
}

Operand::Operand(OperandType type, std::string &variableName) {
    this->operandEnum = type;
    this->var_name_ = variableName;
}

IntermediateCode *createFuntionDecCode(std::string &funtionName) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::FUNCTION);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, funtionName);
    return intermediateCode;
}

IntermediateCode *createParamCode(std::string &varName) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::PARAM);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, varName);
    return intermediateCode;
}

IntermediateCode *createCallFuntionCode(std::string &rtValue, std::string &funtionName) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::CALL);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, funtionName);
    intermediateCode->result = new Operand(OperandType::VARIABLE, rtValue);
    return intermediateCode;
}

IntermediateCode *createReadCode(std::string &variable) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::READ);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, variable);
    return intermediateCode;
}

IntermediateCode *createWriteCode(std::string &variable) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::WRITE);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, variable);
    return intermediateCode;
}

IntermediateCode *createAssignCode(std::string &left, std::string &right) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::ASSIGN);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, left);
    intermediateCode->op2 = new Operand(OperandType::VARIABLE, right);
    return intermediateCode;
}

IntermediateCode *createArgsCode(std::string &variable) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::ARG);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, variable);
    return intermediateCode;
}

IntermediateCode *createReturnCode(std::string &returnValue) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::RETURN);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, returnValue);
    return intermediateCode;
}

IntermediateCode *
createIfGOTOCode(std::string &temp_left, std::string &temp_right, std::string &relation, std::string label) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::IF_GOTO);
    intermediateCode->relation = relation;
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, temp_left);
    intermediateCode->op2 = new Operand(OperandType::VARIABLE, temp_right);
    intermediateCode->result = new Operand(OperandType::VARIABLE, label);
    return intermediateCode;
}

IntermediateCode *createGOTOCode(std::string &label) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::GOTO);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, label);
    return intermediateCode;
}

IntermediateCode *createLabelCode(std::string &label) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::LABEL);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, label);
    return intermediateCode;
}

IntermediateCode *createConstantCode(std::string &place, std::string &value) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::CONSTANT);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, place);
    intermediateCode->op2 = new Operand(OperandType::CONSTANT, value);
    return intermediateCode;
}

IntermediateCode *
createBinaryCode(std::string &result, std::string &oprand1, std::string &oprand2, std::string &binaryOperator) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::BINARY);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, oprand1);
    intermediateCode->op2 = new Operand(OperandType::VARIABLE, oprand2);
    intermediateCode->result = new Operand(OperandType::VARIABLE, result);
    intermediateCode->relation = binaryOperator;
    return intermediateCode;
}

IntermediateCode *createMinusCode(std::string &result, std::string &oprand1) {
    IntermediateCode *intermediateCode = new IntermediateCode(IntermediateCodeType::MINUS);
    intermediateCode->op1 = new Operand(OperandType::VARIABLE, oprand1);
    intermediateCode->result = new Operand(OperandType::VARIABLE, result);
    return intermediateCode;
}

