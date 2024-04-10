#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char memberName[25];
	char memberpassword[20];
}Member;



void member(){
	FILE* memberfp;
	memberfp = fopen("member.txt", "a");
	
	if (memberfp == NULL) {
		printf("Error to open this file!!!");
		exit(-1);
	}
	else() {

	}


}

void login() {
	char memberName[25], memberPassword[20];

	FILE* loginfp;
	loginfp = fopen("memberLogin.txt", "a");

	printf("Enter your name : ");
	rewind(stdin);
	scanf("%s", &memberName);

	//to do check same name & pw
	if (memberName==) {
		printf("Username already exists.Please choose a different one.\n");
		return;
	}

	printf("Enter password: ");
	rewind(stdin);
	scanf("%s", password);
	printf("Sign Up successful!\n");

}

void membernameExists() {
	char memberName[25], memberpassword[20];
	FILE* existfp;
	existfp = fopen("member.txt", "r");

	if (memberName == membernameExists) {
		strcmp(memberName)
	}
}