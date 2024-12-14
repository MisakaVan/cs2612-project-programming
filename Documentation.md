# C语言中 struct/union/enum 的定义与声明的词法分析与语法分析

## 功能特性

本项目在 WhileDB 语言的基础上，提供了以下的词法分析与语法分析支持：

- struct/union/enum 的定义与声明
- 基于 typedef 的类型定义
- 变量的定义
- 以上定义中右半部分（RIGHT_TYPE）的指针类型、数组类型、函数类型
- 变量名、类型名的定义冲突检查

---

## 环境依赖

本项目使用 Makefile 执行编译与测试。需要以下依赖：

- 操作系统：Ubuntu
- 编译器与工具：GCC, Make
- 词法、语法分析器：Flex, Bison

> 测试于 Ubuntu 24.04，flex 2.6.4，bison 3.8.2

---

## 文件结构

```
cs2612-project-programming/
├── build/             # 构建输出目录
├── resource/
│   └── test/          # 测试文件夹
│       ├── test-enum                        # enum 测试文件
│       ├── test-identifier-predeclared      # 前向声明测试文件
│       ├── test-<feature-1>                 # 更多测试样例
│       ...
└── src/               # 源代码
```

---

## 编译与测试步骤

1. 进入目录并执行编译

   ```bash
   cd cs2612-project-programming
   make all
   ```

   此时会在该目录下由 bison 生成 `parser_report.txt`，包含了文法（Grammer）以及带结构的移入规约操作分析。可以检查上下文无关语法的定义以及有无移入规约冲突。

2. 执行测试

   ```bash
   make test_all
   ```

   该操作会对 `resource/test` 目录下每个子目录内的 c 语言文件（以`.jtl.h`为后缀）执行语法分析测试，并在每个 c 语言文件相同目录下生成 `.out` 文件，表示该 c 语言文件的语法分析结果。

---

## 测试结果

### typedef, 数组与指针

以 `resource/test/test-typedef/testcase1.jtl.h` 中的一段为例

```c
// typedef with array
typedef int arr_of_int_t[10];
typedef int *arr_of_ptr_to_int_t[10]; // array of pointers to int
typedef int (*ptr_to_arr_of_int_t)[10]; // pointer to array of int
```

其语法分析结果对应 `resource/test/test-typedef/testcase1.jtl.h.out` 中的

```
Global item:
  Type: typedef
    Left type:
      int
    Right type:
      (As AST)
      Arr[10]:
        Original type: arr_of_int_t
      (As Human Language)
      Original type: arr_of_int_t
      is
        array[10] of
          the LHS type
Global item:
  Type: typedef
    Left type:
      int
    Right type:
      (As AST)
      Ptr:
        Arr[10]:
          Original type: arr_of_ptr_to_int_t
      (As Human Language)
      Original type: arr_of_ptr_to_int_t
      is
        array[10] of
          pointer to
            the LHS type
Global item:
  Type: typedef
    Left type:
      int
    Right type:
      (As AST)
      Arr[10]:
        Ptr:
          Original type: ptr_to_arr_of_int_t
      (As Human Language)
      Original type: ptr_to_arr_of_int_t
      is
        pointer to
          array[10] of
            the LHS type
```

分别被正确解析成了 `array[10] of int`，`array[10] of pointers to int` 以及 `pointer to array[10] of int`。

### 前向声明与类型未定义错误

再以 `resource/test/test-identifier-predeclared/testcase1.jtl.h` 为例

```c
struct Foo;

struct Foo a; // OK: Foo is registered as struct at line 1

enum Foo b; // Warning: Identifier Foo has been registered as struct, not enum

union Foo c; // Warning: Identifier Foo has been registered as struct, not union


typedef Foo foo_t;
foo_t d; // OK: foo_t has been registered as typedef
bar_t e; // Warning: Identifier bar_t has never been registered
```

其中先前向声明 `struct Foo`，而后声明了一个 `struct Foo` 类型的变量 `a`，没有问题。

然而，尝试将 `Foo` 用作 `enum` 或 `union` 的名称时，则会有对应的错误（参见 `resource/test/test-identifier-predeclared/testcase1.jtl.h.out`）：

```
Warning: (Line 5) Identifier Foo is not registered as enum
Warning: (Line 7) Identifier Foo is not registered as union
Warning: (Line 10) Identifier Foo is not registered as typedef
```

其后 `bar_t e;` 语句因为 `bar_t` 从未声明过，所以出现错误，也可以在 `.out` 文件中找到：

```
Warning: (Line 12) Identifier bar_t has never been registered
```



> 更多的测试结果也可以在 `resource/test` 的每个子目录下查看。
