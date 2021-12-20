# CS323 Project 3 Report

11811620毛尊尧 11811535武尚萱 11813121张家澍

## Intermediate Code Design

```c++
class IntermediateCode {
public:
    IntermediateCodeType intermediateCodeEnum;
    Operand *result;
    Operand *op1;
    Operand *op2;
    std::string relation;
};
```

We implemented Intermediate code as linear representation. The  `intermediateCodeEnum` will be used to distinguish
different kind of IR, which will be used in `print` and `optimiztion`. `result`, `op1` `op2` are the three variable used
in IR;The operand currently is very simple: just a enum class to tell the kind of oprand (label, function, variable or
constant), and a string of variable name.

```c++
class Operand {
public:
    OperandType operandEnum;
    std::string var_name_;
};
```

## IR Generation Implementation

Just as the project document illustrated, SDT is a very useful tool to generate IR. We use `two pass` strategy, which
means we generate our IR after the whole syntax parse tree is constructed. For certain syntax units(syntax tree nodes),
we implement the translation function to generate their IR correspondingly.

The code below are the syntax tree nodes we should handle:

```C++
std::vector<IntermediateCode *> &translate_Args(SyntaxTreeNode *argsNode, SymbolTable &symbolTable, std::vector<std::string> &argList);
std::vector<IntermediateCode *> &translate_Exp(SyntaxTreeNode *exp, SymbolTable &symbolTable, std::string &place);
std::vector<IntermediateCode *> &translate_stmt(SyntaxTreeNode *stmt, SymbolTable &symbolTable);
std::vector<IntermediateCode *> &translate_Compst(SyntaxTreeNode *compst, SymbolTable &symbolTable);
std::vector<IntermediateCode *> &translate_cond_Exp(SyntaxTreeNode *exp, SymbolTable &symbolTable, std::string &lb_t, std::string lb_f);
std::vector<IntermediateCode *> &translate_fundec(SyntaxTreeNode *fundec, SymbolTable &symbolTable);
std::vector<IntermediateCode *> &translate_VarList(SyntaxTreeNode *varList);
std::vector<IntermediateCode *> &translate_dec(SyntaxTreeNode *pNode, SymbolTable &table);
```

After calling the translation function in a pre-order, one node's IR and its chilren's IR will be pushed into a vector
called `selfAndChildrenCodes`.Finally, after top-down recursively calling, all the IR will be pushed into the `Program`
non-terminal node. Then we print it.

```c++
class SyntaxTreeNode {   
std::vector<IntermediateCode *> selfAndChildrenCodes;
}
```

## IR Optimization

Our IR optimization is based on constant propagation and dead code elimination within single blocks. First, we introduce
how we formulated such an idea. Then, we introduce how we implemented our IR optimization. Finally, we demonstrate the
effectiveness of such IR optimization technique.

There are two causes of the redundency of generated IR. The first cause is the unoptimized way to define and use
variables. For example, to define an integer variable ```a``` with an intial value of ```0```, the basic IR generation
outputs the following code.

```
_t0 = 0
a = _t0

```

Then, to assign the value of this variable to another integer variable ```b```, the following IR is generated.

```
_t1 = a
b = _t1
```

We can see that these operations involves a lot of temporary variables introduced by the generation schemes. These
temporary variables are used for only one time locally, and are never further used. Apparently, we can reduce the
occurence of these variables by accessing the non-temporary variables directly. Another cause is the useless variables
and codes defined by users. For instance, the user may define a variable that he or she will never use later.

In summary, the redundency comes from the duplication caused by the IR generation scheme or the user. Such duplication
can be simplified, and the number of instructions will be decreased. Therefore, from a data-centric view, the key issue
in IR optimization is to determine which variables in the IR are useless and when they become useless. If an variable
defined and assigned somewhere is no longer used later, it is useless. However, it is complex to define the sequence of
IR. This is because IR contains branches and loops which break the linear representation of IR into pieces of vertices
which are actually basic blocks. These basic blocks forms a graph which makes it a complicated issue to determine when a
variable is useless. Although there are complete solutions which we can use, since this topic will only be covered in
the later lectures, and due to our time budget, we decide to implement IR optimization within single basic blocks. This
optimization strategy is effective and much simpler than data flow analysis.

Data flow analysis is complex because we have to decide when a variable is useless from the graph of basic
blocks.However, we oberve that in our case, temporary variables are never accessed across multiple basic blocks. It a
temporary variable is never accessed from another basic block, then during IR optimization, we can simply regard it
useless in all other basic blocks. In this case, IR optimiztion within one single block is possible when we only
consider the temporary variables.

The implementation of IR optimization involves constant propagation and dead code elimination. First, we demonstrate how
we implemented constant propagation. In constant propagation, we traverse the IR of each basic block and consider
arithmetic operations and assigning operation. Arithmetic operators are either unary or binary. Without loss of
generality, we only demonstrate how constant propagation is done on IR with binary operators. For the IR with binary
operator ```a = b op c```, if ```b``` and ```c``` are constants with known values, then ```a``` is a constant with a
known value. In this case, we simply replace the original IR with ```a = val```, where ```val``` is the value
of ```b op c```. Then, for assigning operations, we record or update the value of this variable. When this variable is
later used as an rvalue, we replace the variable with its corresponding value. After constant propagation, we implement
dead code elimination, where we also traverse the IR of each basic block, but in reverse order. In dead code
elimination, we only consider the IR without side effects, which rules out jumps, lables, function definitions, function
calls, and returns. When we traverse these IR, we simply jump over them. First, We define ```use```, a set to record the
variables in use. Before we traverse the first IR, we set the initial value of ```use``` to empty. After we traverse
each IR, if the lvalue is a temporary variable and is not in ```use```, we remove this IR because the variable it
defines or updates is never used later. Otherwise, we remove the lvalue from ```use``` and add rvalues into ```use```.
This order must be strictly enforced. Here, we give a concrete example to demonstrate IR optimization.

This is the unoptimized IR.

```
_t0 = 1
a = _t0
_t1 = 2
b = _t1
_t2 = _t0 + _t1
c = _t2
```

First, we run constant propagation from the first IR to the last. After constant propagation, the IR is as follows.

```
_t0 = 1
a = 1
_t1 = 2
b = 2
_t2 = 3
c = 3
```

Then, we traverse the IR from the last to the first to do dead code elimination. The result is as follows.

```
a = 1
b = 2
c = 3
```

```_t0```, ```_t1```, and ```_t2``` are eliminated because after they are assigned, they are never used. We do not
eliminate the IR which manipulates non-temporary variables because they might be used in other basic blocks.

The IR we generate is very effective in that it greatly reduces constant calculation. For example, for the first
testcase, the length of IR we generate is only 14, and all the constant calculations are reduced into cosntant results.
This is the original program.

``` c
int main()
{
    int a, b, c;
    int final = 0;
    a = 5;
    b = a * a * (a + 2);
    write(b);
    c = b / a + 1;
    write(c);
    final = a + b - c * 3 + (b / a - 4);
    write(final);
    return 0;
}
```

This is the generated IR.

```
FUNCTION main :
final := #0
a := #5
b := #175
t6 := #175
WRITE t6
c := #36
t11 := #36
WRITE t11
final := #103
t21 := #103
WRITE t21
t22 := #0
RETURN t22
```

In summary, our IR optimiztion is simple because it does not involve cross-basic block analysis, but still effective
because it can eliminate constant calculation.

## Bonus Part

Our project can support array type in spl code, which can pass `test_3_b01.spl` in extra directory. To implement this,
we need to handle more products in SDT action:

```json
EXP: EXP_LB_EXP_RB
VARDEC: VARDEC_LB_INT_RB
```

For `EXP: EXP1_LB_EXP2_RB`, such as `array[1]`, we need to generate at least three codes:

1. Declare a temp to calculate the exp2. In this case, `temp1 := #1`
2. Declare a temp to calculate the offset in bytecode, in this example, `temp2 := temp1 * #4`
3. Declare a temp to calculate the absolute address, in this example, `temp3 := &array + temp2`
4. use the `place` variable from caller to get the value, in this example, `place := *temp3`

Furthermore, if the `array[1]` shows up in left side of `exp1 = exp2` we need to change the translation scheme
for `EXP: EXP_ASSIGNOP_EXP`.We need to get the temporary variable which storing the array member address ( in this case,
temp3), and generate IR `*temp3 = exp2`.

```c
int main(){
    int array[2];
    array[0]=3;
    array[1]=array[0];
    write(array[1]);
    return array[1];
}
```

Result:

```assembly
FUNCTION main :
DEC array 8
t1 := #3
t5 := #0
t3 := t5 * #4
t4 := &array + t3
t2 := *t4
*t4 := t1
t11 := #0
t9 := t11 * #4
t10 := &array + t9
t7 := *t10
t14 := #1
t12 := t14 * #4
t13 := &array + t12
t8 := *t13
*t13 := t7
t19 := #1
t17 := t19 * #4
t18 := &array + t17
t16 := *t18
WRITE t16
t23 := #1
t21 := t23 * #4
t22 := &array + t21
t20 := *t22
RETURN t20
```

