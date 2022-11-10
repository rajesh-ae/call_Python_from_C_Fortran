// call_py_function.c - Simple C program to  
// demonstrate calling python functions from C
//
// To compile: (in ubuntu 20.04 with gcc)
// gcc -o call_py_function -I/usr/include/python3.8/ call_py_function.c -lpython3.8
//
// To Run: (example)
// ./call_py_function py_function hello
// ./call_py_function py_function multiply_two 8 9
// ./call_py_function py_function multiply_list 3 4 6 8

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"

PyObject *makelist(int array[], int size);
PyObject *makearray(int *array, int size);

int main(int argc, char *argv[])
{
  PyObject *pModule = NULL;
  PyObject *pFunc = NULL;
  PyObject *pValue = NULL;
  PyObject *pArgs = NULL;
  PyObject *pList = NULL;
  PyObject *pArray = NULL;

  if (argc < 3) 
  {
      printf("Usage: exec_name python_source python_function values_to_multiply\n");
      return 1;
  }

  // Initialize the Python Interpreter
  Py_Initialize(); // Initialize Python
  _import_array(); // Initialize NumPy

  // Add current directory to Python path
  PyObject *sys_path = PySys_GetObject("path");
  PyList_Append(sys_path, PyUnicode_FromString("./"));

  // Load the module object
  pModule = PyImport_ImportModule(argv[1]);

  // Get the python function
  pFunc = PyObject_GetAttrString(pModule, argv[2]);

  if (PyErr_Occurred()) PyErr_Print();

  if (PyCallable_Check(pFunc)) 
  {
    if(argc == 3){ 
      // call python to say hello - no args
      PyObject_CallObject(pFunc, NULL);
    }else if(argc == 5){
      // call multiply_two - no args
      int a = atoi(argv[0 + 3]);
      int b = atoi(argv[1 + 3]);
      PyObject *a_object = PyLong_FromLong(a);
      PyObject *b_object = PyLong_FromLong(b);
      pValue = PyObject_CallFunctionObjArgs(pFunc,a_object,b_object,NULL);
    /*  // Using a list to send values to python
    }else if(argc > 5){
      int *arr;
      int nvar = argc - 3;
      arr = (int *) malloc(sizeof(int)*nvar);
      for (int i = 0; i < nvar; i++) arr[i] = atoi(argv[i + 3]);

      // Create a list to send to Python
      pList = makelist(arr, nvar);

      // Pass the list to Python function
      pValue = PyObject_CallFunctionObjArgs(pFunc,pList,NULL);
    */
    }else if(argc > 5){
      // Using a Numpy array to send values to python

      // Create an array from input values
      int *arr;
      int nvar = argc - 3;
      arr = (int *) malloc(sizeof(int)*nvar);
      for (int i = 0; i < nvar; i++){
        arr[i] = atoi(argv[i + 3]);
      }
      // Wrap the array using NumPy
      pArray = makearray(arr, nvar);

      // Pass the array to Python function
      pValue = PyObject_CallFunctionObjArgs(pFunc,pArray,NULL);
    }else{
      printf("Error: Arguments not supported");
      printf("Usage: exec_name python_source python_function values_to_multiply\n");
      return 1;
    }
    // Display result obtained from Python
    if (pValue != NULL) 
    {
        printf("Result of multiplication from Python : %ld\n", PyLong_AsLong(pValue));
        Py_DECREF(pValue);
    }
  }else{
      PyErr_Print();
  }

  // Clean up
  Py_DECREF(pModule);

  // Finish the Python Interpreter
  Py_Finalize();

  return 0;
}

PyObject *makelist(int array[], int size){
  // Create a PyObject containing a Python list from the array values
  // The array data is copied into the new list
  PyObject *mylist = PyList_New(size);
  for(int i = 0; i<= size; i++) PyList_SET_ITEM(mylist, i, PyLong_FromLong(array[i]));
  return mylist;
}

PyObject *makearray(int *array, int size) {
  // Create a PyObject containing a NumPy array from the array values
  // The array data is only referenced and not copied
    npy_intp dims[1];
    dims[0] = size;
    return PyArray_SimpleNewFromData(1, dims, NPY_INT32, (void *)array);
}
