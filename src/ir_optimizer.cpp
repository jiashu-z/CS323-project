#include "ir_optimizer.h"

IROptimizer::IROptimizer(const std::vector<IntermediateCode>& ir_vector) : original_ir_vector_(ir_vector) {}

void IROptimizer::GenerateBasicBlocks() {
  BasicBlock block;
  auto iter = original_ir_vector_.begin();
  block.ir_vector_.push_back(*iter);
  iter++;
  while (iter != original_ir_vector_.end()) {
    if (iter->intermediateCodeEnum == IntermediateCodeType::LABEL) {
      
    }
  }
  basic_block_list_.push_back(block);
}