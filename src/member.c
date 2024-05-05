#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

#define BASE_MEMBER_LENGTH 8



member_t* init_member() {
    member_t* new_member = malloc(sizeof(member_t));
    //fix here (derefering NULL pointer 'new_member')
    if (new_member != NULL) {
        new_member->username = NULL;
        new_member->hashed_password = NULL;
        new_member->gender = 0;
        new_member->email = NULL;
        new_member->contact_no = NULL;
        new_member->membership = 0;
    }
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

int create_member_record(member_vector_t *members, char *name, char *password, char gender, char* email, char* contact_no, enum Membership membership) {
    if (is_member_exists(members, name))
        return EXIT_FAILURE;

    member_t* newMember = init_member();

    newMember->username = strdup(name);
    newMember->hashed_password = malloc(65);
    hash_message(password, newMember->hashed_password);
    newMember->gender = gender;
    newMember->email = strdup(email);
    newMember->contact_no = strdup(contact_no);
    newMember->membership = membership;

    add_member(members, newMember);
    return EXIT_SUCCESS;
}

int create_member_record_with_hashed_password(member_vector_t *members, char *name, char *hashed_password, char gender, char* email, char* contact_no, enum Membership membership) {
    if (is_member_exists(members, name))
        return EXIT_FAILURE;

    member_t* newMember = init_member();

    newMember->username = strdup(name);
    newMember->hashed_password = strdup(hashed_password);
    newMember->gender = gender;
    newMember->email = strdup(email);
    newMember->contact_no = strdup(contact_no);
    newMember->membership = membership;

    add_member(members, newMember);
    return EXIT_SUCCESS;
}

int load_members(member_vector_t* members, const char* filepath) {
	FILE* file;
	file = fopen(filepath, "r");

	if (file == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
	}

    char username[255], password[65], gender, email[255], contact_no[20];
    membership_t membership;
    while (fscanf(file, "%254[^\t]\t%64s\t%c\t%254s\t%19[^\t]\t%d\n",
                  username,
                  password,
                  &gender,
                  email,
                  contact_no,
                  &membership) == 6)
        create_member_record_with_hashed_password(members, username, password, gender, email, contact_no, membership);

    fclose(file);
    return EXIT_SUCCESS;
}

int write_members(member_vector_t* members, const char* filepath) {
    FILE* file;
    file = fopen(filepath, "w");

    if (file == NULL) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < members->num_of_members; i++)
        fprintf(file, "%.254s\t%.64s\t%c\t%.254s\t%.19s\t%d\n",
                members->array[i]->username,
                members->array[i]->hashed_password,
                members->array[i]->gender,
                members->array[i]->email,
                members->array[i]->contact_no,
                members->array[i]->membership);

    fclose(file);
    return EXIT_SUCCESS;
}

member_t* login_as_member(member_vector_t* members, const char* username, const char* password) {
    int i = find_member_index(members, username);

    if (i == -1)
        return NULL;

    member_t* member = members->array[i];
    if (compare_hash(password, member->hashed_password) == 0)
        return member;

    return NULL;
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

int delete_member(member_vector_t* members, const char* username) {
    int i = find_member_index(members, username);

    if (i == -1)
        return EXIT_FAILURE;

    free_member(members->array[i]);
    members->array[i] = NULL;

    // shift into empty slot
    for (int j = i + 1; j < members->num_of_members; j++)
        members->array[j - 1] = members->array[j];
    members->array[--members->num_of_members] = NULL;

    return EXIT_SUCCESS;
}

//feedback
void feedback(const member_t* member, feedback_type_t type, const char* message) {
    FILE *fptr;
    fptr = fopen("feedback.txt", "a");

    if (fptr == NULL) {
        fprintf(stderr, "Error: Unable to open file for writing feedback\n");
        return;
    }

    char type_str[50];
    switch (type) {
        case SUGGESTION:
            strcpy(type_str, "Suggestion");
        break;
        case COMPLAINT:
            strcpy(type_str, "Complaint");
        break;
    }
    fprintf(fptr, "%s\t%s\t%s\t%s\n", member->username, member->email, type_str, message);
    fclose(fptr);

    feedback_t* feedback = malloc(sizeof(feedback_t));
    if (feedback == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for feedback\n");
        return;
    }
    feedback->member_name = strdup(member->username);
    feedback->member_email = strdup(member->email);
    feedback->type = type;
    feedback->message = strdup(message);

    printf("Feedback received from %s <%s>\n", member->username, member->email);
    printf("Type: %s\n", type_str);
    printf("Message: %s\n", message);

    free(feedback->member_name);
    free(feedback->member_email);
    free(feedback->message);
    free(feedback);
}

void free_members_vector(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    free(members->array);
    free(members);
}

void free_member(member_t* member) {
    if (member->username)        free(member->username);
    if (member->hashed_password) free(member->hashed_password);
    if (member->email)           free(member->email);
    if (member->contact_no)      free(member->contact_no);
    free(member);
}