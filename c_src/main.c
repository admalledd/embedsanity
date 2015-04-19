#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "pypy_hooks.h"
#include "java_hooks.h"
#include "mono_hooks.h"
#include "util.h"
#include "globals.h"

int main(int argc, char * argv[])
{
    G_globals = malloc(sizeof(t_G_globals));//init globals memory. yuk

    FILE *fp;
    if (argc > 1){
        //arg __must__ be a .conf file to parse
        fp = fopen(argv[1],"rb");
        if (!fp){
                perror("config load error ");
                fprintf(stderr,"file: '%s'",argv[1]);
                exit(EXIT_FAILURE);
        }
        printf("loading config file '%s'\n",argv[1]);
        G_globals->config_file=argv[1];
    }
    else{
        //no arg, load $cwd/config.conf
        char * cwd = getcwd(NULL,0);
        //cwd+'/'+$CONFIG_FILE_NAME+\0
        G_globals->config_file = (char *)malloc(sizeof(char)*(strlen(cwd)+strlen(CONFIG_FILE_NAME)+1+1));
        strcpy(G_globals->config_file,cwd);
        free(cwd);
        strcat(G_globals->config_file,"/");
        strcat(G_globals->config_file,CONFIG_FILE_NAME);
        fp = fopen(G_globals->config_file,"rb");
        if (!fp){
                perror("config load error ");
                fprintf(stderr,"file: '%s'",G_globals->config_file);
                exit(EXIT_FAILURE);
        }
        printf("loading config file '%s'\n",G_globals->config_file);
    }
    json_error_t error;
    G_globals->config=json_loadf(fp,0,&error);

    if (!G_globals->config){
        fprintf(stderr, "error parsing config file: on line %d: %s\n", error.line, error.text);
        return 1;
    }

    load_config();

    init_pypy();

    init_java();

    init_mono();

    /*MAIN LOOP CALL HERE*/

    json_decref(G_globals->config);
    return 0;
}
