# CS2612 编程任务：C 语言中 struct/union/enum 的定义与声明的词法分析与语法分析

## 功能特性

本项目实现了C语言中 struct/union/enum/typedef 以及全局变量的定义与声明的词法分析与语法分析。

- struct/union 的声明与字段定义
- enum 和 enum 常量的声明
- 基于 typedef 的类型定义
- 变量、函数、指针、数组的声明

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
├── doc/               # 文档
├── main               # 可执行文件
└── src/               # 源代码

```

## 构建

提供了一个简单的 Makefile，可以直接执行 `make` 来构建项目。

注意：构建过程在MacOS和Ubuntu上测试通过，但是没有在Windows上测试过。

### 依赖

- `gcc`
- `bison: 3.8.2`
- `flex: 2.6.4`

### 构建步骤

```bash
make
```

这将会在 `build/` 目录下生成中间文件，在当前目录下生成可执行文件 `main`。

要清除构建文件，可以执行`make clean`。

## 运行与测试

### 运行可执行文件

可执行文件 `main` 接受一个参数，为需要进行语法分析的文件路径。将语法/词法分析结果输出到标准输出。

```bash
./main <path-to-file>
```

`main`检测环境变量`PARSER_VERBOSE`。如果设置了`PARSER_VERBOSE`，则会输出更多的调试信息。

例如：

```bash
export PARSER_VERBOSE=1
./main <path-to-file>
```

或

```bash
PARSER_VERBOSE=1 make test_all
```

### 通过脚本批量测试

随项目提供了`batchtest.sh`脚本，可以批量测试给定目录下的所有文本文件。

要查看用法，可以运行：
```bash
bash batchtest.sh
```

在 `resource/test` 目录下提供了一些测试文件。在`batchtest.mk`中，调用了`batchtest.sh`脚本，可以批量测试`resource/test`目录下的所有测试文件。

运行`resource/test`下的所有测试案例：
```bash
make test_all
```
输出结果将会在输入文件的同一目录下生成`.out`文本文件。

清除所有`.out`文件：
```bash
make clean_test_all
```

要查阅测试案例的相关内容，可以查看`resource/test/readme.md`，以及查看测试输入文件的内容与注释

## 语法分析和词法分析

请查看`doc/`目录下的文档。

## 注意事项

- 理论上不用 devcontainer 也可以跑
- 稍微配置了一下 clang-format，但是devcontainer里的clangd版本太老了，读不懂配置文件

## TODO

- [x] 有三个移入/规约冲突（关于 `annon-right-type` ） 这些冲突是因为这个标注类型可能为空导致的。
- [x] 利用 `lib.h` 给的哈希表存放一些类型前置定义
  - [x] 检查在全局变量/Struct/Union/Enum/Typedef声明中出现的标识符是否已经在全局出现过
  - [x] 检查Struct/Union/Enum/Typedef被使用时是否已经有前置定义
  - [x] 检查Struct/Union的字段内是否有变量名重复
  - [x] 指出检查的完备性（没有遗漏检查）
- [x] 完善build、test、使用文档

