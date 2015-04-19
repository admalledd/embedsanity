import data

import pprint
ffi=data['ffi']

@ffi.callback('int(int)')
def func(a):
    print('Python: Got from C %d' % a)
    return a * 2

@ffi.callback('void()')
def c_callback():
    print('Python: called from c_callback')

def init():
    #add our generic callback...
    data['gl'].callbacks.c_2_pypy_callback = ffi.cast('void *',c_callback)

    print("***calling C from pypy***")
    c_func = ffi.cast('int(*)(int(*)(int))',data['gl'].callbacks.pypy_2_c_callback)
    c_func(func)