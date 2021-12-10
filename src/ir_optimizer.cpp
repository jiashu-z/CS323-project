#include "ir_optimizer.h"

IROptimizer::IROptimizer(const std::vector<IntermediateCode> &ir_vector) : original_ir_vector_(
    ir_vector) {
  GenerateBasicBlocks();
}

void IROptimizer::GenerateBasicBlocks() {
  BasicBlock block;
  auto iter = original_ir_vector_.begin();
  block.ir_vector_.push_back(*iter);
  iter++;
  while (iter != original_ir_vector_.end()) {
    if (iter->intermediateCodeEnum == IntermediateCodeType::LABEL) {
      if (!block.ir_vector_.empty()) {
        basic_block_list_.push_back(block);
      }
      block = BasicBlock();
      block.ir_vector_.push_back(*iter);
    }
    block.ir_vector_.push_back(*iter);
    if (iter != original_ir_vector_.end()
        && (iter->intermediateCodeEnum == IntermediateCodeType::GOTO
            || iter->intermediateCodeEnum == IntermediateCodeType::IF_GOTO)) {
      basic_block_list_.push_back(block);
      block = BasicBlock();
    }
  }
  basic_block_list_.push_back(block);
}

void IROptimizer::DoLocalOptimization() {
  for (const auto& iter : basic_block_list_) {

  }
}

std::list<IntermediateCode> IROptimizer::GenerateOptimizedIR() {
  DoLocalOptimization();
  std::list<IntermediateCode> optimizedIRList;
  for (const auto& iter : basic_block_list_) {
    for (const auto& ir_iter : iter.ir_vector_) {
      optimizedIRList.push_back(ir_iter);
    }
  }
  return optimizedIRList;
}