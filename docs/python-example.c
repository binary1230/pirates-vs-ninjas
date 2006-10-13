/*
 * Dom's HACKY-ass embedded python test.
 *
 * You can use the following to compile it:
 * gcc python-example.c -I/usr/include/python2.3 -lm -lpython2.3 -lpthread  
 * -lutil -ldl  -L/usr/lib/python2.3/config -export-dynamic
 */

#include <Python.h>

#define SCRIPT_FILENAME "test"

// no arguments can be passed with this right now
int run_python_function(PyObject* pModule, char* function_name) {

	PyObject* pFunc = PyObject_GetAttrString(pModule, function_name);	

	if (!pFunc) {
		fprintf(stderr, "Can't find function '%s'\n", function_name);
		PyErr_Print();
		return 0;
	}

	if (!PyCallable_Check(pFunc)) {
		fprintf(stderr, "'%s' not callable!\n", function_name);
		PyErr_Print();
		Py_DECREF(pFunc);
		return 0;
	}

	PyObject* pRetValue = PyObject_CallObject(pFunc, NULL);
	Py_DECREF(pFunc);

	// Do something with pValue (return value)
	if (!pRetValue) {
		PyErr_Print();
		return 0;
	}

	//double ret_val;
	//ret_val = PyFloat_AsDouble(pRetValue);
	//printf("%s() result = '%f'\n", function_name, ret_val);

	Py_DECREF(pRetValue);
	
	return 1;
}

int main(int argc, char *argv[])
{
	Py_SetProgramName(argv[0]);
	Py_Initialize();

	char* script_file = "test";

	PyObject* pName = PyString_FromString(script_file);
	if (!pName) {
		PyErr_Print();
		return -1;
	}

	PyObject* pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if (!pModule) {
		PyErr_Print();
		return -1;
	}

	run_python_function(pModule, "woo");

	Py_DECREF(pModule);

	Py_Finalize();

  return 0;
}
