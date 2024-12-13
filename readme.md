# CS2612 编程任务：C 语言中 struct/union/enum 的定义与声明的词法分析与语法分析

## 注意事项

- 理论上不用 devcontainer 也可以跑
- 稍微配置了一下 clang-format，但是devcontainer里的clangd版本太老了，读不懂配置文件

## TODO

- [x] 有三个移入/规约冲突（关于 `annon-right-type` ） 这些冲突是因为这个标注类型可能为空导致的。
- [ ] 利用 `lib.h` 给的哈希表存放一些类型前置定义
  - [x] 检查在全局变量/Struct/Union/Enum/Typedef声明中出现的标识符是否已经在全局出现过
  - [x] 检查Struct/Union/Enum/Typedef被使用时是否已经有前置定义
  - [ ] 检查Struct/Union的字段内是否有变量名重复
- [ ] 完善build、test、使用文档

