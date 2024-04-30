#include <staff.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

#define BASE_STAFF_LENGTH 4

int login_staff() {
	FILE* staffList;
	staff_t currentStaff;

	if ((staffList = fopen("Staff_List.txt", "r")) == NULL) {
		fprintf(stderr, "Error to open this file!\n");
		return EXIT_FAILURE;
	}

	fscanf(staffList, "%[^|] | %[^|] | %[^|] | %d | %[^|] | %d | %lf",
	currentStaff.username, currentStaff.hashed_password, currentStaff.email, currentStaff.contact_no, currentStaff.position, currentStaff.permissions, currentStaff.salary);

	char username_buffer[255], password_buffer[255];
	char retry;

	do {
		if ((strcmp(username_buffer, currentStaff.username) || strcmp(password_buffer, currentStaff.hashed_password)) != 0) {
			printf("Wrong username and password combination. Want to try again? (Y to retry) > ");
			scanf("%c", retry);
			rewind(stdin);
			return EXIT_FAILURE;
		}
	} while (retry == 'Y');
	return 0;
}

int write_staff(staff_vector_t* staff_v, const char* filepath) {
    FILE* file;
    file = fopen(filepath, "w");

    if (file == NULL) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < staff_v->num_of_staff; i++)
        fprintf(file, "%s %s\n", staff_v->array[i]->username, staff_v->array[i]->hashed_password);

    fclose(file);
    return EXIT_SUCCESS;
}

staff_t* login_as_staff(staff_vector_t* staff_v, const char* username, const char* password) {
	int i = find_staff_index(staff_v, username);

	if (i == -1)
		return NULL;

	staff_t* staff = staff_v->array[i];
	if (compare_hash(password, staff->hashed_password) == 0)
		return staff;

	return NULL;
}

int find_staff_index(staff_vector_t * staff_v, const char* username) {
    for (int i = 0; i < staff_v->num_of_staff; i++)
        if (strcmp(staff_v->array[i]->username, username) == 0)
            return i;
    return -1;
}

void deleteStaff(staff_vector_t* staff_v, const char* username, const char* password) {
    int i;
    char confirm;

    for (i = 0; i < staff_v->num_of_staff; i++) {
        if (strcmp(staff_v->array[i]->username, username) == 0) {
            break;
        }
    }
    if (i < staff_v->num_of_staff) {
        printf("Are you sure you want to delete your account? (Y/N): ");
        scanf(" %c", &confirm);

        if (toupper(confirm) == 'Y') {
            free_member(staff_v->array[i]);

            for (int j = i; j < staff_v->num_of_staff - 1; j++) {
                staff_v->array[j] = staff_v->array[j + 1];
            }
            (staff_v->num_of_staff)--;
            printf("Account deleted successfully !\n");
        }
        else {
            printf("Account not found, Please try again.\n");
        }
    }
}

void editStaff(staff_vector_t* staff_v, const char* username, const char* password, char* email, char* contact_no) {
    int choice;
    int index = find_staff_index(staff_v, username);

    if (index == -1) {
        printf("Member not found\n");
        return;
    }

    do {
        printf("Edit Staff Details\n");
        printf("1. Edit username\n");
        printf("2. Edit password\n");
        printf("3. Edit contact number\n");
        printf("4. Edit email\n");
        printf("5. Exit edit staff.\n");
        scanf("%d", &choice);
        rewind(stdin);
        switch (choice) {
        case 1:
            printf("Enter new username : ");
            scanf("%s", staff_v->array[index]->username);
            rewind(stdin);
            break;
        case 2:
            printf("Enter new password : ");
            scanf("%s", staff_v->array[index]->hashed_password);
            rewind(stdin);
            break;
        case 3:
            printf("Enter new contact number : ");
            scanf("%s", staff_v->array[index]->contact_no);
            rewind(stdin);
            break;
        case 4:
            printf("Enter new email : ");
            scanf("%s", staff_v->array[index]->email);
            rewind(stdin);
            break;
        case 5:
            printf("Exiting edit staff details.\n");
            break;
        default:
            printf("Invalid input, please try again.\n");
        }
    } while (choice != 5);

    write_staff(staff_v, "Staff_List.txt");
    printf("Member details updated succesfully.\n");
}

void free_staff_vector(staff_vector_t* staff_v) {
    for (int i = 0; i < staff_v->num_of_staff; i++)
        free_staff(staff_v->array[i]);
    free(staff_v->array);
    free(staff_v);
}

void free_staff(staff_t* staff) {
    if (staff->username)        free(staff->username);
    if (staff->hashed_password) free(staff->hashed_password);
    if (staff->email)           free(staff->email);
    if (staff->contact_no)      free(staff->contact_no);
    free(staff);
}