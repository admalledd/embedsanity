#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "globals.h"
#include "util.h"

char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}

/*  This routine reads the entire file into memory.
    Aborts/exits program if file is not found. (make sure file exists!)
    Generally preferred to use the python based file handling?

 */

char * read_whole_file (const char * rel_file_name)
{
    unsigned long s;
    char * contents;
    FILE * f;
    size_t bytes_read;
    int status;

    char * file_name = get_path(rel_file_name);



    f = fopen (file_name, "r");
    if (! f) {
        fprintf (stderr, "Could not open '%s': %s.\n", file_name,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    fseek(f,0,SEEK_END);
    s = ftell(f);
    rewind(f);
    contents = malloc (s + 1);
    if (! contents) {
        fprintf (stderr, "Not enough memory.\n");
        exit (EXIT_FAILURE);
    }

    bytes_read = fread (contents, sizeof (char), s, f);
    if (bytes_read != s) {
        fprintf (stderr, "Short read of '%s': expected %lu bytes "
                 "but got %zu: %s.\n", file_name, s, bytes_read,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    status = fclose (f);
    if (status != 0) {
        fprintf (stderr, "Error closing '%s': %s.\n", file_name,
                 strerror (errno));
        exit (EXIT_FAILURE);
    }
    contents[s]='\0';
    free(file_name);
    return contents;
}

/*
*init_paths(): init paths to usable settings, load from globals.config.* first, fallback to hardcoded values.
*/
int init_paths()
{
    G_paths.cwd= getcwd(NULL,0);//default to curdir. TODO: add magic finder to find data dir?
    json_t *j_pth=json_object_get(G_globals->config,"paths");
    if (!j_pth){
        fprintf(stderr,"config error: unable to load 'paths' json key\n");
        return -1;
    }
    if (!json_is_string(json_object_get(j_pth,"data_dir"))){
        fprintf(stderr,"config error: 'data_dir' is not a string/defined!\n");
        return -1;
    }
    if (!json_is_string(json_object_get(j_pth,"pypy_home"))){
        fprintf(stderr,"config error: 'pypy_home' is not a string/defined!\n");
        return -1;
    }
    if (!json_is_string(json_object_get(j_pth,"java_class_home"))){
        fprintf(stderr,"config error: 'java_class_home' is not a string/defined!\n");
        return -1;
    }/*
    if (!json_is_string(json_object_get(j_pth,"java_class_sys"))){
        fprintf(stderr,"config error: 'java_class_sys' is not a string/defined!\n");
        return -1;
    }*/
    G_paths.data            = json_string_value(json_object_get(j_pth,"data_dir"));
    G_paths.pypy_home       = json_string_value(json_object_get(j_pth,"pypy_home"));
    G_paths.java_class_home = json_string_value(json_object_get(j_pth,"java_class_home"));
    //G_paths.java_class_sys  = json_string_value(json_object_get(j_pth,"java_class_sys"));

    printf("data dir at: '%s'\n", G_paths.data);
    printf("pypy-home at: '%s'\n",G_paths.pypy_home);
    printf("java_class_home at: '%s'\n", G_paths.java_class_home);
    //printf("java_class_sys at: '%s'\n",G_paths.java_class_sys);
    return 0;
}

void load_config(){
    /*
     * Called once at startup, loads the json config file into the proper structs and such.
     * If any fail, abort program, otherwise we would be in unknowable  state.
     *
    */
    if (init_paths() !=0){
        exit(EXIT_FAILURE);
    }

}

char * get_path(const char *path)
{
    /*
     * used during bootstrap only, prefer later to use CFFI callback'd one where I *know* I can get things right.
    */
    char * ret_path = concat(2,G_paths.data,path);
    printf("merged path: '%s'\n",ret_path);
    return ret_path;
}
