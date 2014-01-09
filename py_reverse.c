#include <Python.h>

int main(char* argc) {
    PyObject *strret, *mymod, *strfunc, *strargs;
    char *cstrret;

    Py_Initialize();

    PySys_SetPath(".");
    mymod = PyImport_ImportModule("reverse");

    strfunc = PyObject_GetAttrString(mymod, "rstring");

    strargs = Py_BuildValue("(s)", "Hello World");

    strret = PyEval_CallObject(strfunc, strargs);

    PyArg_Parse(strret, "s", &cstrret);
    printf("Reversed string: %s\n", cstrret);

    Py_Finalize();
    return 0;
}
