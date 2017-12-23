#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
char sub_function_code[] = "int sub(int a, int b){return (a - b);}";
int main()
{
    int fd = open("sub.c", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    write(fd, sub_function_code, strlen(sub_function_code));
    close(fd);
    int retval = system("gcc -fPIC -shared sub.c -o libsub.so");
    printf("systme retval: %d\n", retval);

    void* handle = dlopen("./libsub.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror(); /* Clear any existing error */

    int (*sub)(int, int) = (int (*)(int, int)) dlsym(handle, "sub");
    char *error = dlerror();
    if (error != NULL) {
       fprintf(stderr, "%s\n", error);
       exit(EXIT_FAILURE);
    }
    
    printf("sub: %d\n", sub(9,2));
    dlclose(handle);
    return 0;
}
