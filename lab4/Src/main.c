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

#define BUFFER_SIZE 4096

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

int create_dir(const char* directoryName) {
    int status = mkdir(directoryName, S_IRWXU | S_IRWXG | S_IRWXO); if (status != 0) {
        perror("Opendir error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int lookup_dir(const char* directoryPath) {
    DIR* directory = opendir(directoryPath);
    if (directory == NULL) {
        perror("Opendir error.\n");
        return EXIT_FAILURE;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    return closedir(directory);
}

int rm_dir(const char* directoryName) {
    int status = rmdir(directoryName); if (status != 0) {
        perror("RmDir error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
    if (strlen(bname) > PATH_MAX) {
        fprintf(stderr, "Path len so long: %ld", strlen(bname));
        return EXIT_FAILURE;
    }

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

int create_file(const char* filename) {
    FILE* file = fopen(filename, "w"); if (file == NULL) {
        perror("Create file error.\n");
        return EXIT_FAILURE;
    }
    fclose(file);
    
    return EXIT_SUCCESS; 
}

int rm_file(const char* filename) {
    int status = unlink(filename); if (status != EXIT_SUCCESS) {
        printf("Remove file error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int cat_file(const char* filename) {
    FILE* f = fopen(filename, "r"); if (f == NULL) {
        perror("File open error.\n");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(buffer[0]), BUFFER_SIZE, f)) > 0) {
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read); if (bytes_written == -1) {
            perror("Write error.\n");
            fclose(f);
            return EXIT_FAILURE;
        }
    }

    if (bytes_read == -1) {
        perror("Read file error.\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);
    return EXIT_SUCCESS;
}
int create_symlink(const char* source, const char* linkName) {
    int status = symlink(source, linkName);
    if (status != EXIT_SUCCESS) {
        perror("Symlink create error.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int cat_symlink(const char* linkName) {
    char targetPath[PATH_MAX];
    ssize_t bytesRead = readlink(linkName, targetPath, sizeof(targetPath) - 1);
    if (bytesRead != EXIT_SUCCESS) {
        perror("Symlink read error.\n");
        return EXIT_FAILURE;
    }

    targetPath[bytesRead] = '\0';
    printf("symlink: %s\n", targetPath);
    return EXIT_SUCCESS;
}

int cat_link_content(const char* filename) {
    return cat_file(filename);
}

int rm_symlink(const char* linkName) {
    return rm_file(linkName);
}

int create_hardlink(const char* targetPath, const char* linkPath) {
    if (link(targetPath, linkPath) != 0) {
        perror("Create hard link error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int rm_hardlink(const char* linkPath) {
    if (unlink(linkPath) != 0) {
        perror("Rm hardlink error\n");
        return EXIT_FAILURE;
    } 

    return EXIT_SUCCESS;
}

int print_permissions_hardlink(const char* filePath) {
    struct stat fileStat;
    if (stat(filePath, &fileStat) == 0) {
        printf("Permissions: %o\n", fileStat.st_mode & 0777);
        printf("Hardlinks amount: %ld\n", fileStat.st_nlink);
        return EXIT_SUCCESS;
    } else {
        perror("Stat error\n");
        return EXIT_FAILURE;
    }
}

int change_permissions(const char* filePath, __mode_t permissions) {
    if (chmod(filePath, permissions) != 0) {
        perror("Change permission error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Handler of mirror link. 
// Copy dir with reversed name in the current execution context with copies of reversed regular files.
int mirror(char* file_path) {
    printf("file path: %s\n", file_path);

    if (strlen(file_path) >= PATH_MAX) {
        fprintf(stderr, "Path len so long: %ld", strlen(file_path));
        return EXIT_FAILURE;
    }

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
            if (strlen(entry->d_name) >= PATH_MAX) {
                fprintf(stderr, "Path len so long: %ld", strlen(entry->d_name));
                return EXIT_FAILURE;
            }

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
    ssize_t len = readlink("/proc/self/exe", exe_path, PATH_MAX - 1); if (len == -1) {
        perror("Error in readlink.\n");
        return NULL;
    }

    exe_path[len] = '\0';
    return basename(exe_path);
}

// Manager - invoke fit handler by link name.
int execute(char* link_name, int argc, char* argv[]) {
    printf("App was exec in [%s] mode.\n", link_name);

    if (strcmp(link_name, "mirror") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in mirror calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return mirror(argv[1]);
    } else if (strcmp(link_name, "create_file") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in create_file calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return create_file(argv[1]);
    } else if (strcmp(link_name, "rm_file") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in rm_file calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return rm_file(argv[1]);
    } else if (strcmp(link_name, "create_dir") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in create_dir calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return create_dir(argv[1]);
    } else if (strcmp(link_name, "cat_file") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in cat_file calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return cat_file(argv[1]);
    } else if (strcmp(link_name, "lookup_dir") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in lookup_dir calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return lookup_dir(argv[1]);
    } else if (strcmp(link_name, "rm_dir") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in rm_dir calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return rm_dir(argv[1]);
    } else if (strcmp(link_name, "create_symlink") == EXIT_SUCCESS) {
        if (argc < 3) {
            perror("Error in create_symlink calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return create_symlink(argv[1], argv[2]);
    } else if (strcmp(link_name, "rm_symlink") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in rm_symlink calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return rm_symlink(argv[1]);
    } else if (strcmp(link_name, "cat_link_content") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in cat_link_content calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return cat_link_content(argv[1]);
    } else if (strcmp(link_name, "cat_symlink") == EXIT_SUCCESS) {
        if (argc < 2) {
            perror("Error in rm_symlink calls - wrong argc.\n");
            return EXIT_FAILURE;
        }

        return cat_symlink(argv[1]);
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
