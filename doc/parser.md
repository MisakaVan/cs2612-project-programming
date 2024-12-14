# Parser的实现细节

## Lexer

`lang.l` 内存放了Parser的词法分析器的实现。

相比于课上的例子，额外增加对于行注释的支持。行注释忽略了`//`之后的所有字符，直到行尾。

在`test-line-comment`中有对行注释的测试案例。


## Parser

`lang.y` 内存放了Parser的语法分析器的实现。

### CFG

以下介绍具体实现的上下文无关文法。

#### GLOBAL ITEM

```
    0 $accept: NT_WHOLE $end

    1 NT_WHOLE: NT_GLOB_ITEM_LIST

    2 NT_GLOB_ITEM_LIST: NT_GLOBAL_ITEM
    3                  | NT_GLOBAL_ITEM NT_GLOB_ITEM_LIST

    4 NT_GLOBAL_ITEM: NT_STRUCT_DEFINITION
    5               | NT_STRUCT_DECLARATION
    6               | NT_UNION_DEFINITION
    7               | NT_UNION_DECLARATION
    8               | NT_ENUM_DEFINITION
    9               | NT_ENUM_DECLARATION
   10               | NT_TYPE_DEFINITION
   11               | NT_VAR_DECLARATION
```

AST的最上层由带分号结尾的`GLOBAL ITEM`的列表构成。`GLOBAL ITEM`共有8种类型：
- struct定义（例：`struct A { int a; }`）
- struct声明（例：`struct A;`）
- union定义（例：`union A { int a; }`）
- union声明（例：`union A;`）
- enum定义（例：`enum A { A1, A2 }`）
- enum声明（例：`enum A;`）
- 类型定义（例：`typedef int A;`）
- 变量声明（例：`int a;`）

```
   12 NT_STRUCT_DEFINITION: TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL

   13 NT_STRUCT_DECLARATION: TM_STRUCT TM_IDENT TM_SEMICOL

   14 NT_UNION_DEFINITION: TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL

   15 NT_UNION_DECLARATION: TM_UNION TM_IDENT TM_SEMICOL

   16 NT_ENUM_DEFINITION: TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE TM_SEMICOL

   17 NT_ENUM_DECLARATION: TM_ENUM TM_IDENT TM_SEMICOL

   18 NT_TYPE_DEFINITION: TM_TYPEDEF NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL

   19 NT_VAR_DECLARATION: NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL
```

前七种`GLOBAL ITEM`（除了变量声明）的测试案例可以在`resource/test/`中的以下文件夹找到：
- `test-struct`
- `test-union`
- `test-enum`
- `test-typedef`

变量声明在下文介绍。typedef语句也使用了变量声明中使用的两个非终结符`LEFT TYPE`和`NAMED RIGHT TYPE EXPR`，在下文一并介绍。


为了定义struct/union/enum的字段，引入了`FIELD LIST`和`ENUM ELE LIST`前者是由分号分隔的变量声明列表（可以为空），后者是由逗号分隔的标识符列表（不可以为空）。
```
   20 NT_FIELD_LIST: ε
   21              | NT_LEFT_TYPE NT_NAMED_RIGHT_TYPE_EXPR TM_SEMICOL NT_FIELD_LIST

   22 NT_ENUM_ELE_LIST: TM_IDENT
   23                 | TM_IDENT TM_COMMA NT_ENUM_ELE_LIST
```

#### 变量声明

C语言的变量声明由左右两部分组成。在[cppreference](https://en.cppreference.com/w/c/language/declarations)中，左部分称为`specifiers-and-qualifiers`，右部分称为`declarators-and-initializers`。

在此，我们不实现C语言内的所有功能（如`const`、`volatile`等 type qualifiers,`constexpr`、`static`等`specifiers`，以及变量初始化使用的`initializers`）。

左部分`LEFT TYPE`是C语言`type specifiers`的子集，包含：
- `int`和`char`两种基本类型
- `struct/union/enum`的声明（如`struct Foo`，其中`Foo`是一个已经被声明的`struct`类型标识符）
- `struct/union/enum`的定义（如`struct Foo {int bar}`，其中`Foo`是一个新的`struct`类型标识符）
- `struct/union/enum`的匿名定义（如`struct {int bar}`）
- 一个`typedef`的类型（如`Foo`，其中`Foo`是一个已经被`typedef`的标识符）

```
   24 NT_LEFT_TYPE: TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   25             | TM_STRUCT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   26             | TM_STRUCT TM_IDENT
   27             | TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   28             | TM_UNION TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   29             | TM_UNION TM_IDENT
   30             | TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE
   31             | TM_ENUM TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE
   32             | TM_ENUM TM_IDENT
   33             | TM_INT
   34             | TM_CHAR
   35             | TM_IDENT
```

关于如何检测标识符是否已经被前文声明/使用，请见后文。

右部分`NAMED RIGHT TYPE EXPR`是C语言`declarators`的子集，包含：
- 一个未被声明的标识符（如`a`）
- 指向`NAMED RIGHT TYPE EXPR`的指针（如`*a`）
- `NAMED RIGHT TYPE EXPR`的数组（如`a[10]`），其中数组的大小必须是一个给定的正整数（不支持`a[]`）
- 是一个函数（如`a(int)`）。`S D(params)`是一个接受`params`参数，返回`S`类型的函数（https://en.cppreference.com/w/c/language/declarations）。其中函数的参数列表是一个逗号分隔的`ANNON RIGHT TYPE EXPR`的列表（可以为空）。
- 用括号包裹的`NAMED RIGHT TYPE EXPR`（如`(a)`）。C语言中，标识符优先和数组中括号、函数声明的小括号结合，再和指针星号结合。用括号包裹的`NAMED RIGHT TYPE EXPR`可以改变优先级。

```
   36 NT_NAMED_RIGHT_TYPE_EXPR: TM_IDENT
   37                         | TM_STAR NT_NAMED_RIGHT_TYPE_EXPR
   38                         | NT_NAMED_RIGHT_TYPE_EXPR TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
   39                         | NT_NAMED_RIGHT_TYPE_EXPR TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
   40                         | TM_LEFT_PAREN NT_NAMED_RIGHT_TYPE_EXPR TM_RIGHT_PAREN
```

用来表示函数参数的`ANNON RIGHT TYPE EXPR`与`NAMED RIGHT TYPE EXPR`类似，但是不出现标识符。C语言要求[the identifier in the declarator is optional](https://en.cppreference.com/w/c/language/function_declaration)，此处我们强制要求函数声明的参数都不包含标识符。

需要注意的是，如果仿照`NT_NAMED_RIGHT_TYPE_EXPR`的产生式来书写`NT_ANNO_RIGHT_TYPE_EXPR`，仅仅将`TM_IDENT`替换为`ε`是不够的，这会导致移入规约冲突。具体来说，是无法区分最左侧的左括号是用来表示函数参数列表的开始，还是用来改变优先级的。重写后的产生式如下：

```
   41 NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY: TM_STAR
   42                                   | TM_STAR NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY
   43                                   | TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
   44                                   | NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_LEFT_BRACKET TM_NAT TM_RIGHT_BRACKET
   45                                   | TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
   46                                   | NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_LEFT_PAREN NT_ARGUMENT_LIST TM_RIGHT_PAREN
   47                                   | TM_LEFT_PAREN NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY TM_RIGHT_PAREN

   48 NT_ANNON_RIGHT_TYPE_EXPR: NT_ANNON_RIGHT_TYPE_EXPR_NON_EMPTY
   49                         | ε
```

在递归定义中，不显式地使用`ε`以避免移入规约冲突。

函数参数列表是由逗号分隔的`ANNON RIGHT TYPE EXPR`的列表构成。函数参数列表可以为空。

```
   50 NT_ARGUMENT_LIST: ε
   51                 | NT_ARGUMENT_LIST_NON_EMPTY

   52 NT_ARGUMENT_LIST_NON_EMPTY: NT_LEFT_TYPE NT_ANNON_RIGHT_TYPE_EXPR
   53                           | NT_LEFT_TYPE NT_ANNON_RIGHT_TYPE_EXPR TM_COMMA NT_ARGUMENT_LIST_NON_EMPTY
```

CFG的语法规则到此结束，需要更多信息可以查看`bison`的输出文件（`make`后，项目根目录下的`parser_report.txt`）

### 标识符管理

所有直接使用标识符的CFG产生式如下：

```
   12 NT_STRUCT_DEFINITION: TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL

   13 NT_STRUCT_DECLARATION: TM_STRUCT TM_IDENT TM_SEMICOL

   14 NT_UNION_DEFINITION: TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE TM_SEMICOL

   15 NT_UNION_DECLARATION: TM_UNION TM_IDENT TM_SEMICOL

   16 NT_ENUM_DEFINITION: TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE TM_SEMICOL

   17 NT_ENUM_DECLARATION: TM_ENUM TM_IDENT TM_SEMICOL

   22 NT_ENUM_ELE_LIST: TM_IDENT
   23                 | TM_IDENT TM_COMMA NT_ENUM_ELE_LIST

   24 NT_LEFT_TYPE: TM_STRUCT TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   26             | TM_STRUCT TM_IDENT
   27             | TM_UNION TM_IDENT TM_LEFT_BRACE NT_FIELD_LIST TM_RIGHT_BRACE
   29             | TM_UNION TM_IDENT
   30             | TM_ENUM TM_IDENT TM_LEFT_BRACE NT_ENUM_ELE_LIST TM_RIGHT_BRACE
   32             | TM_ENUM TM_IDENT
   35             | TM_IDENT

   36 NT_NAMED_RIGHT_TYPE_EXPR: TM_IDENT
```

标识符的声明可能和已经存在于作用域中的表示符冲突；标识符的使用要求作用域中已经存在标识符的前置声明。在语法分析中，要求我们检查当前作用域中的标识符使用情况。本项目处理的作用域情况较为简单，共分为两种：
- 全局作用域。
- struct/union的定义作用域。

注意函数作用域：由于本项目不支持函数/语句定义，函数参数列表也不出现形参名称，因此不需要处理函数作用域。
本项目也不支持局部变量定义（因为没有语句定义），因此不需要处理局部作用域。

与全局作用域相关的CFG产生式可以分为两类：

1. （全局）声明标识符：向全局作用域添加新的标识符。
- struct/union/enum名称的定义和声明：新的类型标识符。（规则12，13，14，15，16，17）
- enum元素（枚举常量）的定义：新的枚举常量。（规则22，23）
- typedef的定义中的`NT_NAMED_RIGHT_TYPE_EXPR`：新的类型标识符。
- 全局变量的声明中的`NT_NAMED_RIGHT_TYPE_EXPR`：变量名。
- 在所有的`NT_LEFT_TYPE`中，引入带有定义的struct/union/enum类型标识符。（规则24，27，30）
- 在所有的`NT_LEFT_TYPE`中，使用的struct/union/enum类型标识符。（规则26，29，32）
    注：一般来说，会写出这样的代码 `struct Foo; struct Foo foo;`或者`struct Foo{}; struct Foo foo;`，我们一般认为前一句声明/引入了`Foo`，后一句使用了`Foo`。实际上，后一句也可以声明/引入`Foo`。`struct Foo foo; struct Foo{};`也是合法的C语言代码。只不过同一个标识符的重复声明不会引发错误而已。


参考C语言的行为，同一个标识符可以被用作不同的类型。
一个标识符可以是struct/union/enum类型标识符（此三者内部互斥），同时也可以是typedef/全局变量/枚举常量的标识符（此三者内部互斥）。我们支持这种行为：向全局作用域添加新的标识符时，将检查是否已经存在同名的标识符，如果存在，将进一步检查前置定义的类型是否与当前希望定义的类型兼容。

2. （全局）使用标识符：仅检查全局作用域中是否存在该标识符，且标识符对应的类型是否正确。
- 在所有的`NT_LEFT_TYPE`中，使用已定义的typedef类型标识符。（规则35）

与struct/union的定义作用域相关的CFG产生式：

- 成员变量标识符：向struct/union的field_list添加新的标识符（通过`NT_NAMED_RIGHT_TYPE_EXPR`）。

参考C语言的行为，成员变量名与全局作用域内的标识符独立。只需要检查当前作用域内是否存在同名的标识符。

以上的检查函数都被定义在`lib.h/lib.c`中，并且在parser尝试将新的语法成分加入AST时被调用。

### 标识符管理：无法解决的问题

标识符的重复声明/定义检查。
- 在C语言中，typedef的重复声明不会引发错误，只要它们所指的是同一个类型。这个问题没有在本项目中解决，因为设计AST的比较操作。
- 在C语言中，struct/union的重复声明不会引发错误；重复定义会引发错误。这个问题没有在本项目中解决，因为没有追踪struct/union是否已经被定义。默认重复声明/定义同一个struct/union不会引发警告。
- 在C语言中，全局变量的重复声明不会引发错误，但是涉及链接阶段可见性（如`extern int a; int a;`是合法的）。这个问题没有在本项目中解决，因为本项目不涉及`extern`，`static`等关键字。本项目默认重复声明同一个变量名是错误的，引发警告。


### 错误处理

如果遇到了语法错误，parser会输出错误信息并停止解析，通过`yyerror`函数实现。因为使用了`%define parse.error detailed`，`yyerror`会输出更详细的错误信息。

如果遇到了标识符重复定义的错误，parser会以`warning`的形式输出错误信息，但是不会停止解析。它仍然会尝试生成AST。
定义`PARSER_VERBOSE`环境变量以输出更多的标识符管理信息。

### AST打印

如果可以生成AST，parser会输出AST的树形结构，通过重写的`astprint`模块实现。

astprint修正了原有的打印函数关于函数变量的打印错误（语义上）。

对于`RIGHT TYPE EXPR`，astprint 提供两种打印方式：
1. 作为AST打印。
2. 作为人类语言打印。

例如，对于`int (* foo)[10]`
- 作为AST打印：外层是[10]，内层是*，最内层是foo。
- 作为人类语言打印：foo是一个指针，指向一个大小为10的数组，数组的元素是LHS类型。

例：

对于以下的输入：
```c
enum foo {
  a,
  b,
  c
} (**(*(*(*func)[113])(char, char))[7]);
```

输出的AST如下：
```
Global item:
  Type: var definition
    Left type:
      new enum foo
      Elements:
        Element: a
        Element: b
        Element: c
    Right type:
      (As AST)
      Ptr:
        Ptr:
          Arr[7]:
            Ptr:
              Function:
              | Ptr:
              |   Arr[113]:
              |     Ptr:
              |       Original type: func
              Arguments:
              + Arg 0:
              |   Left type:
              |     char
              |   Right type:
              |     Original type: <empty>
              + Arg 1:
              |   Left type:
              |     char
              |   Right type:
              |     Original type: <empty>
      (As Human Language)
      Original type: func
      is
        pointer to
          array[113] of
            pointer to
              function of arguments
              + Arg 0:
              |   Left type:
              |     char
              |   Right type:
              |     Original type: <empty>
              |     is
              |       the LHS type
              + Arg 1:
              |   Left type:
              |     char
              |   Right type:
              |     Original type: <empty>
              |     is
              |       the LHS type
              returning
                pointer to
                  array[7] of
                    pointer to
                      pointer to
                        the LHS type
```