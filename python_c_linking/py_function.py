'''
    py_function class contains simple functions
    to demonstrate calling from a C program.
'''
def hello():
    print("Hello from Python!")
    return
    
def multiply_two(a,b):
    return a*b
    
def multiply_list(mylist):
    print("The argument supplied is: ",type(mylist))
    c = 1
    for i in range(len(mylist)):
      c = c*mylist[i]
    return c
