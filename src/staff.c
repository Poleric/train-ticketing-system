#include <staff.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BASE_STAFF_LENGTH 4

int load_staff() {
	FILE* staffFP;
	staff_t currentStaff;

	if ((staffFP = fopen("Staff_List.txt", "r")) == NULL) {
		fprintf(stderr, "Error to open this file!\n");
		return EXIT_FAILURE;
	}

	fscanf(staffFP, "%[^|] | %[^|] | %s | %d | %[^|] | %d | %lf",
	currentStaff.username, currentStaff.hashed_password, currentStaff.email, currentStaff.contactNo, currentStaff.position, currentStaff.permissions, scurrentStaff.salary);

	char username_buffer[255], password_buffer[255];
	char retry;

	if ((strcmp(username_buffer, currentStaff.username) || strcmp(password_buffer, currentStaff.hashed_password)) != 0) {
		printf("Wrong username and password combination. Want to try again? (Y to retry) > ");
		scanf("%c", retry);
		return EXIT_FAILURE;
	}

	return 0;
}

int account() {

}

int schedule() {

}

int manageAcc() {

}