#include "D:\Python27\include\Python.h"

/* file: mymath.c */

// 真正实现
int add(int a, int b)
{
    return a + b;
}

// 包装函数。Python调用add方法时传进来的参数在args里
PyObject* wrap_add(PyObject* self, PyObject* args)
{
    int a, b, result;
    // 解析参数
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return NULL;
    result = add(a, b);
    // 返回PyObject* 类型的参数
    return Py_BuildValue("i", result);
}

// mymath模块所包含的函数列表
static PyMethodDef mymathMethods[] =
{
    // 每行一个方法，含义依次为
    // Python方法名，C方法名，参数值，方法文档
    {"add", wrap_add, METH_VARARGS, "doc: add(a, b) \nreturn a + b"},
    {NULL, NULL, 0, NULL}
    // 上面的最后一行相当于结束符
};

// 初始化模块的方法，自动调用
// 命名要求为init后加上模块名
void initmymath()
{
    PyObject* m;
    // 调用Py_InitModule方法初始化模块mymath，其中模块所具有
    // 的函数列表由第二个参数提供
    m = Py_InitModule("mymath", mymathMethods);
}
