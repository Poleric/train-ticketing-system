#include <staff.h>
#include <stdlib.h>
#include <stdio.h>

int staff() {
	FILE* staffFP;
	staff_t currentStaff;


	if ((staffFP = fopen("Staff_List.txt", "r")) == NULL) {
		return EXIT_FAILURE;
	}
	else {
		// fscanf(staffFP, "%d | %[^|] | %s | %d | %[^|] | | %lf",
		// currentStaff.id, currentStaff.name, currentStaff.email, currentStaff.contactNo, currentStaff.position, currentStaff.permissions, scurrentStaff.alary);


	}
	return 0;
}

int account() {

}

int schedule() {

}

int manageAcc() {

}