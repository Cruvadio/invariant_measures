//#define PY_SSIZE_T_CLEAN
//#include <Python.h>
//#include <Windows.h>
//#include <cmath>
//#include <vector>
//#include "symbolic_image.hpp"
//
//
//
//
//PyObject* start(PyObject*,  PyObject* args) {
//
//
//    PyObject* d;
//    PyObject* il;
//    PyObject* ib; 
//    PyObject* a1; 
//    PyObject* b1;
//
//    double delta;
//    double a;
//    double b;
//    long it_loc;
//    long it_bal;
//
//
//    if (!PyArg_ParseTuple(args, "dddll", &delta, &a, &b, &it_loc, &it_bal))
//        return NULL;
//    SymbolicImage s(-2, 2, -2, 2, 100, delta, it_loc, it_bal, a, b);
//
//
//
//
//    vector<double> v = s.find_invariant_measures();
//
//    int size = v.size();
//    PyObject* tuple; 
//
//    tuple = PyTuple_New(size);
//
//    for (int i = 0; i < size; i++)
//    {
//        PyTuple_SetItem(tuple, i, PyFloat_FromDouble(v[i]));
//    }
//
//    return tuple;
//}
//
//
//static PyMethodDef calculations_methods[] = {
//    // The first property is the name exposed to Python, fast_tanh, the second is the C++
//    // function name that contains the implementation.
//    { "fast_start", (PyCFunction)start, METH_VARARGS, nullptr },
//
//    // Terminate the array with an object containing nulls.
//    { nullptr, nullptr, 0, nullptr }
//};
//
//
//static PyModuleDef calculations_module = {
//    PyModuleDef_HEAD_INIT,
//    "calculations",                        // Module name to use with Python import statements
//    "Provides some functions, but faster",  // Module description
//    0,
//    calculations_methods                  // Structure that defines the methods of the module
//};
//
//PyMODINIT_FUNC PyInit_calculations() {
//    return PyModule_Create(&calculations_module);
//}