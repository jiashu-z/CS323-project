#include "ir_optimizer.h"
#include <map>
#include <algorithm>
#include <iostream>

IROptimizer::IROptimizer(const std::vector<IntermediateCode> &ir_vector) : original_ir_vector_(
    ir_vector) {}

void IROptimizer::GenerateBasicBlocks() {
  BasicBlock block;
  auto iter = original_ir_vector_.begin();
  block.ir_vector_.push_back(*iter);
  iter++;
  while (iter != original_ir_vector_.end()) {
    if (iter->intermediateCodeEnum == IntermediateCodeType::LABEL) {
      if (!block.ir_vector_.empty()) {
        basic_blocks_.push_back(block);
      }
      block = BasicBlock();
    }
    block.ir_vector_.push_back(*iter);
    if (iter != original_ir_vector_.end()
        && (iter->intermediateCodeEnum == IntermediateCodeType::GOTO
            || iter->intermediateCodeEnum == IntermediateCodeType::IF_GOTO)) {
      basic_blocks_.push_back(block);
      block = BasicBlock();
    }
    iter++;
  }
  basic_blocks_.push_back(block);
}

void IROptimizer::DoLocalOptimization() {
  for (auto & basic_block : basic_blocks_) {
    basic_block = PropagateConstant(basic_block);
  }
  for (auto & basic_block : basic_blocks_) {
    basic_block = EliminateLocalDeadCode(basic_block);
  }
}

std::vector<IntermediateCode> IROptimizer::GenerateOptimizedIR() {
  GenerateBasicBlocks();
  if (CheckIfTempVarCrossBasicBlock()) {
    return original_ir_vector_;
  }
  DoLocalOptimization();
  std::vector<IntermediateCode> optimized_ir_vec;
  for (const auto &iter: basic_blocks_) {
    for (const auto &ir_iter: iter.ir_vector_) {
      optimized_ir_vec.push_back(ir_iter);
    }
  }
  return optimized_ir_vec;
}

std::set<std::string> IROptimizer::ComputeDef(const BasicBlock &basic_block) {
  std::set<std::string> used;
  std::set<std::string> def;
  for (const auto &ir: basic_block.ir_vector_) {
    if (ir.op1 != nullptr) {
      used.insert(ir.op1->var_name_);
    }
    if (ir.op2 != nullptr) {
      used.insert(ir.op2->var_name_);
    }
    if (ir.result != nullptr && used.find(ir.result->var_name_) != used.end()) {
      def.insert(ir.result->var_name_);
    }
  }
  return def;
}

std::set<std::string> IROptimizer::ComputeUse(const BasicBlock &basic_block) {
  std::set<std::string> defined;
  std::set<std::string> use;
  for (const auto &ir: basic_block.ir_vector_) {
    if (ir.result != nullptr) {
      defined.insert(ir.result->var_name_);
    }
    if (ir.op1 != nullptr && defined.find(ir.op1->var_name_) != defined.end()) {
      use.insert(ir.op1->var_name_);
    }
    if (ir.op2 != nullptr && defined.find(ir.op2->var_name_) != defined.end()) {
      use.insert(ir.op2->var_name_);
    }
  }
  return use;
}

bool IROptimizer::CheckIfTempVarCrossBasicBlock() {
  std::map<size_t, std::set<std::string>> basic_block_var_names_map;
  for (size_t i = 0; i < basic_blocks_.size(); ++i) {
    auto &basic_block = basic_blocks_[i];
    std::set<std::string> var_name_set;
    for (const auto &ir: basic_block.ir_vector_) {
      if (ir.result != nullptr && ir.result->var_name_[0] == '_') {
        var_name_set.insert(ir.result->var_name_);
      }
      if (ir.op1 != nullptr && ir.op1->var_name_[0] == '_') {
        var_name_set.insert(ir.op1->var_name_);
      }
      if (ir.op2 != nullptr && ir.op2->var_name_[0] == '_') {
        var_name_set.insert(ir.op2->var_name_);
      }
    }
    basic_block_var_names_map.insert(std::make_pair(i, var_name_set));
  }

  for (const auto &iter1: basic_block_var_names_map) {
    for (const auto &iter2: basic_block_var_names_map) {
      if (iter1.first == iter2.first) {
        continue;
      }
      std::vector<std::string> intersection;
      std::set_intersection(iter1.second.begin(),
                            iter1.second.end(),
                            iter2.second.begin(),
                            iter2.second.end(),
                            std::back_inserter(intersection));
      if (!intersection.empty()) {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
        for (const auto &iter: intersection) {
          std::cout << iter << std::endl;
        }
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
        return true;
      }
    }
  }
  return false;
}

IROptimizer::BasicBlock IROptimizer::PropagateConstant(const BasicBlock &basic_block) {
  std::vector<IntermediateCode> ir_vec;
  std::map<std::string, int> const_map;
  for (const auto &ir: basic_block.ir_vector_) {
    switch (ir.intermediateCodeEnum) {
      case IntermediateCodeType::CONSTANT: {
        if (const_map.find(ir.op1->var_name_) == const_map.end()) {
          const_map.insert(std::make_pair(ir.op1->var_name_, std::stoi(ir.op2->var_name_)));
        } else {
          const_map[ir.op1->var_name_] = std::stoi(ir.op2->var_name_);
        }
        auto new_ir_ptr = createConstantCode(ir.op1->var_name_, ir.op2->var_name_);
        ir_vec.push_back(*new_ir_ptr);
        break;
      }
      case IntermediateCodeType::ASSIGN: {
        if (const_map.find(ir.op2->var_name_) != const_map.end()) {
          if (const_map.find(ir.op1->var_name_) == const_map.end()) {
            const_map.insert(std::make_pair(ir.op1->var_name_, const_map[ir.op2->var_name_]));
          } else {
            const_map[ir.op1->var_name_] = const_map[ir.op2->var_name_];
          }
          std::string val_str = std::to_string(const_map[ir.op1->var_name_]);
          auto new_ir_ptr = createConstantCode(ir.op1->var_name_, val_str);
          ir_vec.push_back(*new_ir_ptr);
        } else {
          if (const_map.find(ir.op1->var_name_) != const_map.end()) {
            const_map.erase(ir.op1->var_name_);
          }
          ir_vec.push_back(ir);
        }
        break;
      }
      case IntermediateCodeType::BINARY: {
        if (const_map.find(ir.op1->var_name_) != const_map.end()
            && const_map.find(ir.op2->var_name_) != const_map.end()) {
          int val_1 = const_map[ir.op1->var_name_];
          int val_2 = const_map[ir.op2->var_name_];
          int res = 0;
          if (ir.relation == "+") {
            res = val_1 + val_2;
          } else if (ir.relation == "-") {
            res = val_1 - val_2;
          } else if (ir.relation == "*") {
            res = val_1 * val_2;
          } else if (ir.relation == "/") {
            res = val_1 / val_2;
          } else {
            std::cerr << "Invalid binary operator " << ir.relation << "." << std::endl;
          }
          if (const_map.find(ir.result->var_name_) == const_map.end()) {
            const_map.insert(std::make_pair(ir.result->var_name_, res));
          } else {
            const_map[ir.result->var_name_] = res;
          }
          std::string val_str = std::to_string(res);
          auto new_ir_ptr = createConstantCode(ir.result->var_name_, val_str);
          ir_vec.push_back(*new_ir_ptr);
        } else {
          ir_vec.push_back(ir);
        }
        break;
      }
      default: {
        ir_vec.push_back(ir);
      }
    }
  }
  BasicBlock basic_block1;
  basic_block1.ir_vector_ = ir_vec;
  return basic_block1;
}

IROptimizer::BasicBlock IROptimizer::EliminateLocalDeadCode(const BasicBlock &basic_block) {
  typedef IntermediateCode IR;
  auto is_var_name = [](const std::string &str) -> bool {
    return !(str[0] == '-' || std::isdigit(str[0]));
  };
  std::set<size_t> eliminate_index;
  std::set<std::string> use;
  std::vector<IR> ir_vec;
  /*
   * 
    enum class IntermediateCodeType {
    ASSIGN,
    BINARY,
    MINUS,
    RETURN,
    ARG,
    WRITE,
    DEC,
    IF_GOTO,
    ARRAY_OFFSET,
    GET_VALUE_IN_ADDRESS,
    ASSIGN_VALUE_IN_ADDRESS,
    ADDRESS_ASSIGN_ADDRESS
};
   */
  for (size_t i = basic_block.ir_vector_.size(); i > 0; --i) {
    IR ir = basic_block.ir_vector_[i - 1];
    switch (ir.intermediateCodeEnum) {
      case IntermediateCodeType::CONSTANT:
      case IntermediateCodeType::ASSIGN: {
        if (is_var_name(ir.op1->var_name_)) {
//          lhs is a var name
          if (ir.op1->var_name_[0] != '_') {
//            lhs is not tmp
            if (is_var_name(ir.op2->var_name_)) {
              use.insert(ir.op2->var_name_);
            }
          } else {
//            lhs is tmp
            if (use.find(ir.op1->var_name_) != use.end()) {
//              lhs will be used
              use.erase(ir.op1->var_name_);
              if (is_var_name(ir.op2->var_name_)) {
                use.insert(ir.op2->var_name_);
              }
            } else {
//              lhs will not be used.
              use.erase(ir.op1->var_name_);
              eliminate_index.insert(i - 1);
            }
          }
        } else {
          std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
        }
        break;
      }
      case IntermediateCodeType::CALL: {
        use.insert(ir.result->var_name_);
        break;
      }
      case IntermediateCodeType::BINARY: {
        if (is_var_name(ir.result->var_name_)) {
          if (ir.result->var_name_[0] != '_') {
            if (is_var_name(ir.op1->var_name_)) {
              use.insert(ir.op1->var_name_);
            }
            if (is_var_name(ir.op2->var_name_)) {
              use.insert(ir.op2->var_name_);
            }
          } else {
            if (use.find(ir.result->var_name_) != use.end()) {
              use.erase(ir.result->var_name_);
              if (is_var_name(ir.op1->var_name_)) {
                use.insert(ir.op1->var_name_);
              }
              if (is_var_name(ir.op2->var_name_)) {
                use.insert(ir.op2->var_name_);
              } else {
                use.erase(ir.result->var_name_);
                eliminate_index.insert(i - 1);
              }
            }
          }
        } else {
          std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
        }
        break;
      };
      case IntermediateCodeType::MINUS: {
        if (is_var_name(ir.result->var_name_)) {
//          lhs is a var name
          if (ir.result->var_name_[0] != '_') {
//            lhs is not tmp
            if (is_var_name(ir.op1->var_name_)) {
              use.insert(ir.op1->var_name_);
            }
          } else {
//            lhs is tmp
            if (use.find(ir.result->var_name_) != use.end()) {
//              lhs will be used
              use.erase(ir.result->var_name_);
              if (is_var_name(ir.op1->var_name_)) {
                use.insert(ir.op1->var_name_);
              }
            } else {
//              lhs will not be used.
              use.erase(ir.result->var_name_);
              eliminate_index.insert(i - 1);
            }
          }
        } else {
          std::cerr << __FILE__ << ":" << __LINE__ << std::endl;
        }
        break;
      }
      case IntermediateCodeType::LABEL:
      case IntermediateCodeType::GOTO:
      case IntermediateCodeType::FUNCTION: {
        break;
      }
      case IntermediateCodeType::PARAM:
      case IntermediateCodeType::ARG:
      case IntermediateCodeType::WRITE:
      case IntermediateCodeType::RETURN: {
        if (is_var_name(ir.op1->var_name_)) {
          use.insert(ir.op1->var_name_);
        }
        break;
      }
      case IntermediateCodeType::READ: {
        if (is_var_name(ir.op1->var_name_)) {
          use.erase(ir.op1->var_name_);
        }
        break;
      }
      case IntermediateCodeType::DEC:break;
      case IntermediateCodeType::IF_GOTO: {
        if (is_var_name(ir.op1->var_name_)) {
          use.insert(ir.op1->var_name_);
        }
        if (is_var_name(ir.op2->var_name_)) {
          use.insert(ir.op2->var_name_);
        }
        break;
      }
      case IntermediateCodeType::ARRAY_OFFSET:break;
      case IntermediateCodeType::GET_VALUE_IN_ADDRESS:break;
      case IntermediateCodeType::ASSIGN_VALUE_IN_ADDRESS:break;
      case IntermediateCodeType::ADDRESS_ASSIGN_ADDRESS:break;
    }
  }
  for (size_t i = 0; i < basic_block.ir_vector_.size(); ++i) {
    if (eliminate_index.find(i) != eliminate_index.end()) {
      continue;
    }
    ir_vec.push_back(basic_block.ir_vector_[i]);
  }
  BasicBlock basic_block1;
  basic_block1.ir_vector_ = ir_vec;
  return basic_block1;
}

IROptimizer::BasicBlock IROptimizer::EliminateLocalTempVar(const BasicBlock &basic_block) {
  auto ir_vec = basic_block.ir_vector_;
  typedef IntermediateCode IR;
  typedef struct {
    std::string var_name_1_;
    std::string var_name_2_;
    std::string op_;
    int val_ = 0;
    bool isConst() const {
      return var_name_1_.empty();
    }
    bool IsUnary() const {
      return !isConst() && var_name_2_.empty();
    }
  } KeyEntry;
  typedef struct {
    std::string var_name_;
  } ValEntry;
//  c := a + b
//  c := a
//  a + b or a is the key
//  c is the value
  std::map<std::string, Operand> var_name_op_map;
  auto insert_if_not_exist = [](std::map<std::string, Operand> &map, Operand *op) {
    auto var_name = op->var_name_;
    if (map.find(var_name) != map.end()) {
      map.insert(std::make_pair(var_name, Operand(*op)));
    }
  };
  for (const auto &ir: ir_vec) {
    switch (ir.intermediateCodeEnum) {
      case IntermediateCodeType::CONSTANT: {
        insert_if_not_exist(var_name_op_map, ir.result);
        break;
      }
      case IntermediateCodeType::ASSIGN: {
//        TODO: What is lhs? op1? op2? result?
        break;
      }
      case IntermediateCodeType::BINARY: {
        insert_if_not_exist(var_name_op_map, ir.result);
        insert_if_not_exist(var_name_op_map, ir.op1);
        insert_if_not_exist(var_name_op_map, ir.op2);
        break;
      }
      case IntermediateCodeType::MINUS: {
        insert_if_not_exist(var_name_op_map, ir.result);
        insert_if_not_exist(var_name_op_map, ir.op1);
        break;
      }
      case IntermediateCodeType::CALL: {
        insert_if_not_exist(var_name_op_map, ir.result);
        break;
      }
      case IntermediateCodeType::READ: {
        insert_if_not_exist(var_name_op_map, ir.op1);
        break;
      }
      default: {
        continue;
      }
    }
  }
  std::map<std::string, KeyEntry> var_name_rhs_map;
  for (auto ir: ir_vec) {
    switch (ir.intermediateCodeEnum) {
      case IntermediateCodeType::CONSTANT: {
        KeyEntry key_entry;
        key_entry.val_ = std::stoi(ir.result->var_name_);
        if (var_name_rhs_map.find(ir.result->var_name_) == var_name_rhs_map.end()) {
          var_name_rhs_map.insert(std::make_pair(ir.result->var_name_, key_entry));
        } else {
          var_name_rhs_map[ir.result->var_name_] = key_entry;
        }
        break;
      }
      case IntermediateCodeType::ASSIGN: {
//        TODO: What is lhs? op1? op2? result?
        break;
      }
      case IntermediateCodeType::BINARY: {
        auto &op1 = ir.op1;
        auto &op2 = ir.op2;
        auto &result = ir.result;
        if (var_name_rhs_map.find(result->var_name_) != var_name_rhs_map.end()) {
          var_name_rhs_map.erase(result->var_name_);
        }
        if (var_name_rhs_map.find(op1->var_name_) != var_name_rhs_map.end()) {
          auto key_entry = var_name_rhs_map[op1->var_name_];
          if (key_entry.isConst()) {
//            op1->operandEnum =
          }
        }
        break;
      }
      case IntermediateCodeType::MINUS: {
        break;
      }
      case IntermediateCodeType::CALL: {
        break;
      }
      case IntermediateCodeType::READ: {
        break;
      }
      default: {
        continue;
      }
    }
  }
}
