#ifndef IR_OPTIMIZER_H
#define IR_OPTIMIZER_H

#include <vector>
#include <list>
#include <set>
#include "IntermediateCode.h"

class IROptimizer {
 private:

  class Variable {
    Operand * op_;
   public:
    explicit Variable(Operand *op) : op_(op) {}
  };

  class BasicBlock {
   public:
    std::vector<IntermediateCode> ir_vector_;
  };

  const std::vector<IntermediateCode> original_ir_vector_;
  std::vector<BasicBlock> basic_blocks_;

  static BasicBlock EliminateLocalTempVar(BasicBlock basic_block);
  static BasicBlock EliminateCommonSubexpressions(BasicBlock basic_block);
  static std::set<std::string> ComputeDef(const BasicBlock& basic_block);
  static std::set<std::string> ComputeUse(const BasicBlock& basic_block);
  void GenerateBasicBlocks();
  void BuildBasicBlockDAG();
  void DoLocalOptimization();
  bool CheckIfTempVarCrossBasicBlock();
 public:
  explicit IROptimizer(const std::vector<IntermediateCode>& ir_vector);
  std::vector<IntermediateCode> GenerateOptimizedIR();
};

#endif