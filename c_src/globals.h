#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Where all the global state is stored (boo! I know...)
 * Mostly this should only be used for start-up constants and call-back
 * pointer holding
 *
 * Note on syntax: no fancy macros, no fancy #ifdefs, as simple as possible
 *      The reason for this is that this file is also used in the bootstrap
 *      process and needs to be parsed as easily as possible for CFFI.cdef()'ing
 *
*/

#include <jansson.h>

#define CONFIG_FILE_NAME "config.conf"

typedef struct {
    /* We want to use this as little as possible due to having to also change it in the bootstrap.py file.
     * The main t_G_config struct is where "most" paths beyond the basics to call bootstrap.py should go?
    */
    const char * cwd;
    const char * pypy_home;
    const char * java_class_home;
    
    // .NET File where "static Main()" is defined 
    const char * mono_interop_exe; 
    
    //Config options for what the first callback (that we pass globals to) lies
    const char * mono_namespace;
    const char * mono_class;
    const char * mono_callback;

    const char * data;
} t_paths;

typedef struct {
    /*
     * Should these only be allowed to be pointers such that any opaque types that we don't need in pypy
     * can safely be skipped with a void* pointer?
     *
     *
    */
    json_t * config;
    char * config_file;
    struct {
        //These should really be the end-result func pointers iunstead of void *...
        void * pypy_2_c_callback;
        void * c_2_pypy_callback;
        void * java_2_c_callback;
        void * c_2_java_callback;
    }callbacks;

} t_G_globals;

extern t_paths G_paths;
extern t_G_globals * G_globals;

#endif // GLOBALS_H_INCLUDED
