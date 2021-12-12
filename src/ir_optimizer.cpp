#include "ir_optimizer.h"
#include <map>
#include <algorithm>

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
      block.ir_vector_.push_back(*iter);
    }
    block.ir_vector_.push_back(*iter);
    if (iter != original_ir_vector_.end()
        && (iter->intermediateCodeEnum == IntermediateCodeType::GOTO
            || iter->intermediateCodeEnum == IntermediateCodeType::IF_GOTO)) {
      basic_blocks_.push_back(block);
      block = BasicBlock();
    }
  }
  basic_blocks_.push_back(block);
}

void IROptimizer::DoLocalOptimization() {}

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
      if (ir.result != nullptr && ir.result->var_name_[0] == 't') {
        var_name_set.insert(ir.result->var_name_);
      }
      if (ir.op1 != nullptr && ir.op1->var_name_[0] == 't') {
        var_name_set.insert(ir.op1->var_name_);
      }
      if (ir.op2 != nullptr && ir.op2->var_name_[0] == 't') {
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
                            intersection.begin());
      if (!intersection.empty()) {
        return false;
      }
    }
  }
  return true;
}
