#include <staff.h>
#include <stdlib.h>
#include <stdio.h>

int staff() {
	FILE* staffFP;
	staff_t staff[50];


	if ((staffFP = fopen("Staff_List.txt", "r") == NULL)) {
		return EXIT_FAILURE;
	}

	// fscanf(staffFP, "%d | %[^|] | %s | %d | %[^|] | | %lf", );

	

	return 0;
}

int account() {

}

int schedule() {

}

int manageAcc() {

}