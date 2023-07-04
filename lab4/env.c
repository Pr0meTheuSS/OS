#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv, char** env) {
    if (argc < 2) {
        perror("Expected cli-args - count of env vars for printing.\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < atoi(argv[1]); i++) {
        printf("%s stored in addr: %p\n", env[i], &env[i]);
    }
    
    // You can watch /proc/<pid>/maps before setenv()
    printf("pid: %d\n", getpid());
    sleep(20);

    // setenv() and get address:
    for (int i = 0; i < 100000; i++) {
        char value_name[16];
        sprintf(value_name, "%d", i);
        setenv(value_name, "VALUE_OF_MY_VAR", 0);
        char* new_env_var = getenv(value_name);
        printf("my new env var has address: %p\n", new_env_var);
     }

   sleep(100);
    return EXIT_SUCCESS;
}

