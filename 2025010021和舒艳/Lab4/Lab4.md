四、选择题与填空
本部分除标明为填空题外均为单项选择题。作答时将所选选项前的 [ ] 改为 [x]，每题只能勾选一项，不要删除其他选项。

4.1 private 的作用
在 main 中写 IntVector v; v.size = 3;，结果是：

[ ]A. 编译通过，size 变为 3
[x]B. 编译错误，因为 size 是 private 成员，类外不能访问
[ ]C. 编译通过，运行时报错
[ ]D. 编译通过，但 size 保持不变
4.2 构造函数
关于框架中的 IntVector()，以下说法正确的是：

 [ ]A. 它必须写成 void IntVector()
 [ ]B. 需要在 main 中写 v.IntVector(); 才会执行
 [x]C. 它与类同名、没有返回类型，在 IntVector v; 创建对象时自动执行
 [ ]D. 它的作用与 Lab3 的 printList 相同
4.3 成员函数的调用
C 语言中的 insertAt(&list, 2, 25) 对应 C++ 中的：

 [ ]A. insert(&v, 2, 25)
 [x]B. v.insert(2, 25)
 [ ]C. v->insert(2, 25)
 [ ]D. IntVector.insert(2, 25)
4.4 引用参数的调用
对于 bool remove(int index, int& removed)，正确的调用是：

 [ ]A. v.remove(1, &removed)
 [x]B. v.remove(1, removed)
 [ ]C. v.remove(1, *removed)
 [ ]D. v.remove(1)
4.5 成员函数末尾的 const
int find(int value) const 末尾的 const 表示：

 [ ]A. 参数 value 不能被修改
 [ ]B. 返回值是常量
 [x]C. 这个函数不修改当前对象的 data 和 size
 [ ]D. 这个函数只能被调用一次
4.6 为什么需要 getSize()
C 版本可以直接读 list.size，C++ 版本却要写 getSize()，原因是：

 [ ]A. C++ 不允许类里有 int 成员
 [x]B. size 是 private 的，类外读不到，getSize() 提供一个只读的入口
 [ ]C. getSize() 比直接读更快
 [ ]D. 没有原因，只是习惯
4.7 两个对象
执行下面的代码后，b.getSize() 的值是：

IntVector a;
IntVector b;
a.insert(0, 10);
 [x]A. 1
 [ ]B. 0
 [ ]C. 不确定，取决于内存里原来的值
 [ ]D. 编译错误
4.8 算法与复杂度
C 版本的 insertAt 和 C++ 版本的 insert 相比，以下说法正确的是：

 [ ]A. C++ 版本插入不需要移动元素
 [x]B. 两者的元素移动过程和时间复杂度完全相同，只是代码组织方式不同
 [ ]C. C++ 版本是 O(1)，C 版本是 O(n)
 [ ]D. C++ 版本必须从前向后移动
4.9 get 的返回值设计
get 用返回值表示成功或失败、用引用参数传回元素值，而不是"直接返回元素值，失败时返回 -1"，原因是：

 [x]A. -1 本身可能就是表中的一个元素，无法区分"读到了 -1"和"读取失败"
 [ ]B. C++ 不允许函数返回负数
 [ ]C. 引用参数比返回值运行更快
 [ ]D. 这样可以少写一个 if
4.10 find 的返回值
执行 v.find(30) 返回 0，含义是：

 [ ]A. 没有找到 30
 [x]B. 30 在下标 0 处
 [ ]C. 读取失败
 [ ]D. 表为空
4.11 填空：程序运行验证
intvector.cpp 实际运行输出是否与第三部分给出的期望输出完全一致？

 [x]A. 一致
 [ ]B. 不一致


无

4.12 填空：C 与 C++ 写法的对应关系
把右列补全，至少填写三行（可以全部填写）：

C（Lab3 / 课堂）	C++（本次作业）
SeqList list; list.size = 0;	IntVector v
insertAt(&list, 2, 25)	v.insert(2.25)
removeAt(&list, 1, &removed)	v.remove(1,removed);
函数体内的 list->data[i]	data[i]
返回 int 的 1 / 0	返回 bool 的 true / false
4.13 填空：一句话总结
Lab3 的 main 里可以直接写 list.size = 4;。用一句话说明：为什么把 size 设为 private 之后，insert、remove 里的边界检查才真正"管用"？

把 size 设为 private 后，类外无法直接修改它，所有增删改操作都必须通过类内成员函数，这样就无法绕过函数里的边界合法性检查，保证了数据的安全。

