
#HOTFIX for broken sys module
import os,sys
sys.stdin  = sys.__stdin__  = os.fdopen(0,'r')
sys.stdout = sys.__stdout__ = os.fdopen(1,'w')
sys.stderr = sys.__stderr__ = os.fdopen(2,'w')
#END HOTFIX for broken sys module

import traceback
import io
from cffi import FFI
ffi = FFI()


print("bootstrapping started (inside pypy)")

@ffi.callback('int(int)')
def func(a):
    print('In python, Got from C:%d' % a)
    return a * 2


ffi.cdef('''    
typedef struct {
    /* We want to use this as little as possible due to having to also change it in the bootstrap.py file.
     * The main t_G_config struct is where "most" paths beyond the basics to call bootstrap.py should go?
    */ 
    const char * cwd;
    const char * pypy_home;
    const char * data;
} t_paths;

typedef struct {
    t_paths * paths;
    void    * globals;
}t_pypy_bootstrap;

// define opaque types here, things that we never go past (and therefore don't care if are wrong. only size!)

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INTEGER,
    JSON_REAL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
} json_type;

typedef struct json_t {
    json_type type;
    size_t refcount;
} json_t;

''')

py_boot = ffi.cast("t_pypy_bootstrap *",c_argument)

try:
    import os.path
    gfile = os.path.join(ffi.string(py_boot.paths.data).decode('utf-8'),'..','c_src','globals.h')
    print("trying for globals.h at '%s'"%gfile)
    if not os.path.isfile(gfile):
        raise OSError("could not find globals.h! eeeeep!")
    gfile = open(gfile,'r').read()


    cdefs = io.StringIO()
    lines = gfile.splitlines()
    for i,line in enumerate(lines):
        line = line.strip()
        if line.startswith('#'):
            continue
        cdefs.write(line+'\n')
    ffi.cdef(cdefs.getvalue(), override=True)#over ride old def's if they exist. trust globals.h

    g_globals = ffi.cast("t_G_globals *",py_boot.globals)
    print("g_globals successfully bridged and casted!")
    print(ffi.string(g_globals.config_file).decode('utf-8'))
    sys.path.insert(0,ffi.string(py_boot.paths.data).decode('utf-8'))
    #ugly cheat to get globals anywhere in our files without writing our own injector. yay!
    
    sys.modules['data']={
                       "ffi":ffi,
                       "gl":g_globals,
                       "paths":py_boot.paths
                        }

    import epy
    epy.init()

except Exception as e:
    traceback.print_tb(sys.exc_info()[2])
    print("Exception: %s"%e)
    raise