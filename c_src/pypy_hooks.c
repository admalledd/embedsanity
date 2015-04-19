
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "PyPy.h"

#include "util.h"
#include "globals.h"
#include "pypy_hooks.h"

#define PYPY_BOOTSTRAP_FILE "data/bootstrap.py"

/*
 * Only ever used here, this holds the pile of data we hand off to bootstrap.py
 * inside a tiny struct that hides what lies beyond via (void *) and such.
 * This is so that we can just pass a mostly opaque struct to pypy and decompose it later.
 *
*/

struct t_pypy_bootstrap {
    t_paths     * paths;
    void * globals;
};


int callback(int (*func)(int))
{
    printf("C->pypy calling\n");
    int ret = func(3);
    printf("Called to Python, pyresult: %d\n", ret);
    //show calling global func pointer
    ((void (*)() )G_globals->callbacks.c_2_pypy_callback)();
    return 16;
}


int init_pypy()
{
    int res;
    char * source;

    G_globals->callbacks.pypy_2_c_callback = &callback;

    source = read_whole_file(PYPY_BOOTSTRAP_FILE);
    printf("doing pypy bootstrap...(in c)\n");
    rpython_startup_code();
    res = pypy_setup_home((char *)G_paths.pypy_home, 1);
    if (res) {
        printf("Error setting pypy home!\n");
        free(source);
        return res;
    }
    //ready to bootstrap this insanity!
    struct t_pypy_bootstrap pypy_bootstrap;
    pypy_bootstrap.paths   =&G_paths;
    pypy_bootstrap.globals =(void *)G_globals;

    res = pypy_execute_source_ptr(source, &pypy_bootstrap);
    if (res) {
        printf("Error calling bootstrap.py!\n");
    }
    free (source);
    return res;

}
