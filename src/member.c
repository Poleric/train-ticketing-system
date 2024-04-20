#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

#define BASE_MEMBER_LENGTH 8

member_t* init_member() {
    member_t* new_member = malloc(sizeof(member_t));
    
    new_member->id = NULL;
    new_member->username = NULL;
    new_member->hashed_password = NULL;
    new_member->gender = 0;
    new_member->email = NULL;
    new_member->contact_no = NULL;
    new_member->membership = 0;
    
    return new_member;
}

member_vector_t* init_members_vector() {
    member_vector_t* members = malloc(sizeof(member_vector_t));
    if (members == NULL)
        return NULL;

    members->max_size = BASE_MEMBER_LENGTH;
    members->array = calloc(sizeof(member_t*), members->max_size);
    members->num_of_members = 0;

    return members;
}

int create_member_record(member_vector_t *members, const char *name, const char *password) {
    if (is_member_exists(members, name))
        return EXIT_FAILURE;

    member_t* newMember = init_member();

    newMember->username = strdup(name);
    newMember->hashed_password = malloc(65);
    hash_message(password, newMember->hashed_password);

    add_member(members, newMember);
    return EXIT_SUCCESS;
}

int load_members(member_vector_t* members) {
	FILE* file;
	file = fopen("memberSignup.txt", "r");

	if (file == NULL) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
	}

    char username_buffer[255], password_buffer[255];

    while (fscanf(file, "%s %s", username_buffer, password_buffer) == 2) {
        member_t *new_member = init_member();

        new_member->username = strdup(username_buffer);
        new_member->hashed_password = strdup(password_buffer);

        add_member(members, new_member);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int write_members(member_vector_t* members) {
    FILE* file;
    file = fopen("memberSignup.txt", "w");

    if (file == NULL) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < members->num_of_members; i++)
        fprintf(file, "%s %s\n", members->array[i]->username, members->array[i]->hashed_password);

    fclose(file);
    return EXIT_SUCCESS;
}

member_t* login_as(member_vector_t* members, const char* username, const char* password) {
    int i = find_member_index(members, username);

    if (i == -1)
        return NULL;

    member_t* member = members->array[i];
    if (compare_hash(password, member->hashed_password) != 0)
        return NULL;

    return member;
}

int find_member_index(member_vector_t* members, const char* username) {
	for (int i = 0; i < members->num_of_members; i++)
		if (strcmp(members->array[i]->username, username) == 0)
			return i;
	return -1;
}

int add_member(member_vector_t* members, member_t* member) {
    if (members->num_of_members + 1 > members->max_size) {
        members->max_size *= 2;
        member_t **tmp = realloc(members->array, sizeof (member_t*) * members->max_size);

        if (tmp == NULL)
            return EXIT_FAILURE;

        members->array = tmp;
    }

    members->array[members->num_of_members++] = member;
    return EXIT_SUCCESS;
}

void free_members_vector(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    free(members->array);
    free(members);
}

void free_member(member_t* member) {
    if (member->id)              free(member->id);
    if (member->username)        free(member->username);
    if (member->hashed_password) free(member->hashed_password);
    if (member->email)           free(member->email);
    if (member->contact_no)      free(member->contact_no);
    free(member);
}