#include <utils.h>
#include <stdio.h>



int main() {
    char hash[SHA256_HASH_LENGTH];
    char* message = "Hello, World!";
    hash_message(message, hash);

    printf("Original message: %s\n", message);
    printf("Hashed message: %s", hash);

    return 0;
}

