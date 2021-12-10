#ifndef IR_OPTIMIZER_H
#define IR_OPTIMIZER_H

#include <vector>
#include <list>
#include "IntermediateCode.h"

class IROptimizer {
 private:
 
  typedef struct {
    std::vector<IntermediateCode> ir_vector_;
  } BasicBlock;

  const std::vector<IntermediateCode> original_ir_vector_;
  std::list<BasicBlock> basic_block_list_;

  void GenerateBasicBlocks();
  void DoLocalOptimization();
 public:
  explicit IROptimizer(const std::vector<IntermediateCode>& ir_vector);
  std::list<IntermediateCode> GenerateOptimizedIR();
};

#endif