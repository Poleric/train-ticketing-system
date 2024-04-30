#include <staff.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <ctype.h>

#define BASE_STAFF_LENGTH 4

staff_t* init_staff() {
    staff_t* new_staff = malloc(sizeof(staff_t));
    //fix here (derefering NULL pointer 'new_staff')
    if (new_staff != NULL) {
        new_staff->username = NULL;
        new_staff->hashed_password = NULL;
        new_staff->email = NULL;
        new_staff->contact_no = NULL;
        new_staff->position = NULL;
        new_staff->permissions = 0;
        new_staff->salary = 0;
    }
    return new_staff;
}

int login_staff() {
	FILE* staffList;
	staff_t currentStaff;

	if ((staffList = fopen("Staff_List.txt", "r")) == NULL) {
		fprintf(stderr, "Error to open this file!\n");
		return EXIT_FAILURE;
	}

	fscanf(staffList, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%d|%lf\n",
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

staff_vector_t* init_staff_vector() {
    staff_vector_t* staff_v = malloc(sizeof(staff_vector_t));
    if (staff_v == NULL)
        return NULL;

    staff_v->max_size = BASE_STAFF_LENGTH;
    staff_v->array = calloc(sizeof(staff_t*), staff_v->max_size);
    staff_v->num_of_staff = 0;

    return staff_v;
}

int create_staff_record(staff_vector_t* staff_v, char* name, char* password, char* email, char* contact_no, char* position, enum Permissions permissions, double salary) {
    if (is_staff_exists(staff_v, name))
        return EXIT_FAILURE;

    staff_t* newStaff = init_staff();

    newStaff->username = strdup(name);
    newStaff->hashed_password = malloc(65);
    hash_message(password, newStaff->hashed_password);
    newStaff->email = strdup(email);
    newStaff->contact_no = strdup(contact_no);
    newStaff->position = strdup(position);
    newStaff->permissions = permissions;
    newStaff->salary = salary;

    add_staff(staff_v, newStaff);
    return EXIT_SUCCESS;
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

int load_staff(staff_vector_t* staff_v) {
    FILE* file;
    file = fopen("Staff_List.txt", "r");

    if (file == NULL) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    char username_buffer[255], password_buffer[255];
    //fix here (string username_buffer and pw_buffer might no be zero-terminated)
    while (fscanf(file, "%254s %254s", username_buffer, password_buffer) == 2) {
        username_buffer[254] = '\0';
        password_buffer[254] = '\0';

        staff_t* new_staff = init_staff();

        new_staff->username = strdup(username_buffer);
        new_staff->hashed_password = strdup(password_buffer);

        add_staff(staff_v, new_staff);
    }

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

int add_staff(staff_vector_t* staff_v, staff_t* staff) {
    if (staff_v->num_of_staff + 1 > staff_v->max_size) {
        staff_v->max_size *= 2;
        staff_t** tmp = realloc(staff_v->array, sizeof(staff_t*) * staff_v->max_size);

        if (tmp == NULL)
            return EXIT_FAILURE;

        staff_v->array = tmp;
    }

    staff_v->array[staff_v->num_of_staff++] = staff;
    return EXIT_SUCCESS;
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
            free_staff(staff_v->array[i]);

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