#include <member.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMBERS_SIZE 4

int main() {
    char char_buff;
    char username[255], password[255];

    member_vector_t* members = malloc(sizeof(member_vector_t));
    members->max_size = MEMBERS_SIZE;
    members->array = calloc(sizeof(member_t*), MEMBERS_SIZE);
    members->num_of_members = 0;

    load_members(members);

    printf("\n");

    printf("Want to sign up (Y/N) ?");
    scanf("%c", &char_buff);
    if (char_buff == 'Y')
        signup(members, NULL, NULL);

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    printf("\n");

    member_t* member = login_as(members, username, password);

    if (member)
        printf("Login successful\n");

    free_members_vector(members);
    return 0;
}