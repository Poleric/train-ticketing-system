#include <utils.h>
#include <stdio.h>
#include <string.h>


int main() {
    char hashed_password[] = "6ca13d52ca70c883e0f0bb101e425a89e8624de51db2d2392593af6a84118090";  // abc123
    char buffer[100];

    while (1) {
        printf("Please enter the password: ");
        
        fgets(buffer, 100, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (compare_digest(buffer, hashed_password) == 0) {
            printf("Welcome!\n");
            break;
        }
        else
            fprintf(stderr, "Wrong password. Please try again!\n");
    }

    return 0;
}

