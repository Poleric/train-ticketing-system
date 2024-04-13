#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define strdup _strdup
#endif


void signup(member_vector_t *members, const char *memberName,const char *memberPwd) {
    member_t* newMember = malloc(sizeof (member_t)); //give this struct a variable (name)
    char input_buffer[255];

	FILE* signupfp;
	signupfp = fopen("memberSignup.txt", "a+");
	if (signupfp == NULL) {
		printf("Error to open this file! Please it again\n");
		return;
	}

    //name
    printf("Enter full name : ");
    rewind(stdin);
    scanf("%s", input_buffer);
    newMember->username = strdup(input_buffer);

    //check username exists
    if (is_member_exists(members, newMember->username)) {
        printf("Username already exists. Please choose a different one.\n");
        return;
    }

    //pwd
    newMember->hashed_password = malloc(65);  // 65 characters
    printf("Enter password : ");
    rewind(stdin);
    scanf("%s", input_buffer);
    hash_message(input_buffer, newMember->hashed_password);

    add_member(members, newMember);

    fprintf(signupfp, "%s %s\n", newMember->username, newMember->hashed_password);

    fclose(signupfp);

	printf("Account created succesfully!\n");

	printf("Sign Up successful!\n");
}

void load_members(member_vector_t* members) {
	FILE* file;
	file = fopen("memberSignup.txt", "r");

	if (file == NULL) {
        printf("Error to open this file!\n");
        return;
	}

    member_t *new_member = malloc(sizeof(member_t));
    char username_buffer[255], password_buffer[255];

    while (fscanf(file, "%s %s", username_buffer, password_buffer) == 2) {
        new_member->username = strdup(username_buffer);
        new_member->hashed_password = strdup(password_buffer);
        add_member(members, new_member);
    }

    fclose(file);
}

member_t* login_as(member_vector_t * members, const char* username, const char* password) {
    int i = find_member_index(members, username);

    if (i == -1) {
        printf("User does not exists\n");
        return NULL;
    }

    member_t* member = members->array[i];
    if (compare_hash(password, member->hashed_password) != 0) {
        printf("Wrong passwordo \n");
        return NULL;
    }

    return member;
}

/*
void member() {
	system("clear");

	int numMember = 0;
	bool check;
	char username[25], memberPwd[20];

	FILE* memberfp;
	memberfp = fopen("member.txt", "a");
	
	if (memberfp == NULL) {
		printf("Error to open this file!\n");
		exit(-1);
	}
	Member member(MAX_MEMBERS);
	int numMember = 0;




}*/

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
    free(member->id);
    free(member->username);
    free(member->hashed_password);
    free(member->email);
    free(member->contact_no);
    free(member);
}