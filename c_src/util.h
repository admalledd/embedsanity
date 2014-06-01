#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED




//unsigned get_file_size (const char * file_name);

char* concat(int count, ...);

char * read_whole_file (const char * file_name);

int init_paths();

void load_config();

char * get_path(const char* path);

void split_path_file(char** p, char** f, char *pf);


#endif // UTIL_H_INCLUDED
