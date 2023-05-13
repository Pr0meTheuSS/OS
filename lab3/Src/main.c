// Copyright 2023 Olimpiev Y. Y.
#include <assert.h>
#include <dirent.h>
#include <libgen.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 10

void swap(char* a, char* b) {
    if (!a || !b) {
        perror("Swap get NULL ptr.\n");
        return;
    }

    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void strrev(char* str) {
    if (!str) {
        perror("Function strrev get NULL ptr.\n");
        return;
    }

    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        swap(&str[i], &str[len - i - 1]);
    }
}

// Clone file with path src_file_path into dest_dir_name with reversed file name and with reversed content.
int clone_reversed_reg_file(char* src_file_path, char* dest_dir_name) {
    assert(src_file_path);

    struct stat st; if (stat(src_file_path, &st) != EXIT_SUCCESS) {
        fprintf(stderr, "Cannot get stat of %s\n", src_file_path);
        return EXIT_FAILURE;    
    }

    if (!S_ISREG(st.st_mode)) {
        return EXIT_SUCCESS;
    }

    FILE* src = fopen(src_file_path, "r"); if (!src) {
        fprintf(stderr, "Cannot open file %s", src_file_path);
        return EXIT_FAILURE;
    }

    char dest_file_path[PATH_MAX];
    char* bname =  basename(src_file_path);
    strrev(bname);

    sprintf(dest_file_path, "%s/%s", dest_dir_name, bname);
    printf("Dest file path is: %s\n", dest_file_path);

    FILE* dest = fopen(dest_file_path, "w+"); if (!dest) {
        fprintf(stderr, "Cannot open file %s", dest_file_path);
        fclose(src);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE + 1];

    fseek(src, 0, SEEK_END);
    ssize_t file_size = ftell(src);
    fprintf(stdout, "sizeof file in bytes is: %zu\n", file_size);

    ssize_t read_bytes = 0;
    ssize_t offset = BUFFER_SIZE < file_size ? BUFFER_SIZE : file_size;
    
    for (ssize_t cur_pos = file_size - offset; cur_pos >= 0; offset += read_bytes) {
        cur_pos = file_size - offset;
        fseek(src, (cur_pos < 0) ? 0 : cur_pos, SEEK_SET);
        read_bytes = fread(buffer, sizeof(buffer[0]), (cur_pos < 0) ? file_size % BUFFER_SIZE : BUFFER_SIZE, src);

        buffer[read_bytes] = '\0';        
        strrev(buffer);
        fwrite(buffer, read_bytes, sizeof(buffer[0]), dest);
    }

    fclose(src);
    fclose(dest);
    
    return EXIT_SUCCESS;
}

// Handler of mirror link. 
// Copy dir with reversed name in the current execution context with copies of reversed regular files.
int mirror(char* file_path) {
    printf("file path: %s\n", file_path);

    char dest_dir_name[PATH_MAX];
    strcpy(dest_dir_name, basename(file_path));
    strrev(dest_dir_name);

    struct stat st; if (stat(file_path, &st) != EXIT_SUCCESS) {
        fprintf(stderr, "Cannot get stat of %s\n", file_path);
        return EXIT_FAILURE;    
    }

    if (S_ISDIR(st.st_mode)) {
        printf("Creating dir name: %s\n", dest_dir_name);

        if (mkdir(dest_dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != EXIT_SUCCESS) {
            perror("Cannot make dir.\n");
            return EXIT_FAILURE;
        }

        printf("file path %s\n", file_path);
        DIR* dir = opendir(file_path); if (!dir) {
            perror("Cannot opendir.\n");
            return EXIT_FAILURE;
        }

        struct dirent *entry; 
        while ((entry = readdir(dir))) {
            printf("%s\n", entry->d_name);
            char file_path_to_reverse[PATH_MAX];
            sprintf(file_path_to_reverse, "%s/%s", file_path, entry->d_name);
            printf("file to clone: %s\n", file_path_to_reverse);            
            clone_reversed_reg_file(file_path_to_reverse, dest_dir_name);
        }

        closedir(dir);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

// Return a string with link name, that was executed for running current programm.
char* get_link_name() {
    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, PATH_MAX - 1);
    if (len == -1) {
        perror("Error in readlink.\n");
        return NULL;
    }
    exe_path[len] = '\0';
    return basename(exe_path);
}

// Manager to invoke fit handler by link name.
int execute(char* link_name, int argc, char* argv[]) {
    printf("App was exec in [%s] mode.\n", link_name);

    if (strcmp(link_name, "mirror") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in mirror calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return mirror(argv[1]);

    } else if (strcmp(link_name, "link2") == EXIT_SUCCESS) {

    } else {
        fprintf(stderr, "Unknown link: %s\n", link_name);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    char* link_name; if (!(link_name = get_link_name())) {
        perror("Get link name function failed.\n");
        return EXIT_FAILURE;
    }
    
    if (execute(link_name, argc, argv) != EXIT_SUCCESS) {
        perror("Execution failed.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
