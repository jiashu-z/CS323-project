This is the repository for the projects of CS323 Compilers in SUSTech.
# CS323 Project 1

[![CMake](https://github.com/wateryloo/CS323-project1/actions/workflows/cmake.yml/badge.svg)](https://github.com/wateryloo/CS323-project1/actions/workflows/cmake.yml)

## References

- https://cmake.org/cmake/help/latest/module/FindFLEX.html
- https://github.com/black13/flex_bison_cmake/blob/master/CMakeLists.txt
- https://www.toptal.com/developers/gitignore
- https://google.github.io/styleguide/cppguide.html

# CS323 Project 2

终结符：

extDef：全局变量、结构体、函数定义

FuncDec：函数声明

Def：局部的变量和结构体声明

Exp：计算表达式

## 错误类型：

| 描述                                    | 可能存在语义错误的非终结符 | 完成情况     | 备注                                                         |
| --------------------------------------- | -------------------------- | ------------ | ------------------------------------------------------------ |
| 1、变量使用时未定义                     | exp                        | finished     | 很多地方只通过了老师给的test case，还需要自己多设计一些样例出来 |
| 2、函数调用时未定义                     | exp                        | finished     |                                                              |
| 3、变量重复定义                         | extdef def                 | finished     |                                                              |
| 4、函数重复定义                         | fundec                     | finished     |                                                              |
| 5、=号两侧表达式类型不匹配              | exp                        | finished     | 结构体和数组类型可能需要在别的产生式更新exptype值            |
| 6、=号左侧出现只有右值的表达式          | exp                        | finished     |                                                              |
| 7、操作数类型不匹配                     | exp                        | finished     |                                                              |
| 8、return语句与函数声明的类型不匹配     | extdef                     | finished     |                                                              |
| 9、函数调用时实参和型参数目或类型不匹配 | exp                        | finished     |                                                              |
| 10、对非数组类型使用[]                  | exp                        | testing(wsx) |                                                              |
| 11、对非函数类型使用（）                | exp                        | finished     |                                                              |
| 12、对数组使用[]时里面是非整数          | exp                        | testing(wsx) |                                                              |
| 13、对非结构体类型使用.                 | exp                        | testing(zjs) |                                                              |
| 14、访问结构体中未定义的field           | exp                        | testing(zjs) |                                                              |
| 15、结构体名字重复                      | exp def                    | testing(zjs) |                                                              |

This is the repository for the projects of CS323 Compilers in SUSTech.


得到语法树根结点 --- 遍历语法树，在遇到特定产生式生成的特定结点时，调用相应的语法检查规则(查表、检查、插表)，如果有错就报错。

e.g.:

```c++
int test_1_o02()
{
    a=2;
}
```

遍历到图中所标注的Exp结点时，已知信息是产生式为Exp:ID，根结点是Exp，那么调用检查ID在symbol table是否存在的函数，如果找不到，打印报错信息，遍历继续。

```json
Program (1)
  ExtDefList (1)
    ExtDef (1)
      Specifier (1)
        TYPE: int
      FunDec (1)
        ID: test_1_o02
        LP
        RP
      CompSt (2)
        LC
        StmtList (3)
          Stmt (3)
            Exp (3)  
              Exp (3)     （这里）
                ID: a     
              ASSIGN
              Exp (3)
                INT: 2
            SEMI
        RC
```

## SymbolTable

```
预计为一个hashmap，在C++里是unordered_map<String,Symbol>
String为相关Symbol的名字，Symbol预计有四种类型：
Primitive（int float char）
Array（可以是二维以上）
Struct
Function
```

```C++
class Symbol {
    using DATA = variant<Primitive, ArrayType, StructType>;
public:
    string name;
    CATEGORY category = CATEGORY::PRIMITIVE;
    DATA data;
};
class Array {
public:
    Symbol base;//数组中每个元素的类型
    int size;          //数组长度
};
class StructType{
public:
    string name;
   std:: vector<Symbol> fieldList;

};
```

