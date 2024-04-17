#include <member.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <ctype.h>

#define MEMBERS_SIZE 4

int prompt_register(member_vector_t* members) {
    char username_buff[255], password_buff[255];

    printf("Registration\n");
    printf("------------\n");

    input("Username: ", username_buff, 255);
    input("Password: ", password_buff, 255);

    create_member_record(members, username_buff, password_buff);
    write_members(members);
    return EXIT_SUCCESS;
}

member_t* prompt_login(member_vector_t* members) {
    char username_buff[255], password_buff[255];

    printf("Login\n");
    printf("-----\n");

    input("Username: ", username_buff, 255);
    input("Password: ", password_buff, 255);

    return login_as(members, username_buff, password_buff);
}

int main() {
    member_vector_t* members = malloc(sizeof(member_vector_t));
    members->max_size = MEMBERS_SIZE;
    members->array = calloc(sizeof(member_t*), MEMBERS_SIZE);
    members->num_of_members = 0;

    load_members(members);
    
    char ch[2];
    input("Want to register? (Y/N) : ", ch, 2);

    if (toupper(*ch) == 'Y')
        prompt_register(members);

    printf("\n");

    member_t* member_session = prompt_login(members);

    printf("\n");

    if (member_session)
        printf("Login successful\n");
    else
        printf("Login failed. Please check username and password.\n");
        
    free_members_vector(members);
    return 0;
}