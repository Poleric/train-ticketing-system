#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEMBERS 100

typedef struct {
	char memberName[25];
	char memPassword[20];
}Member;

void member(){
	int numMember = 0;
	bool check;
	char memberName[25], memPassword[20];

	FILE* memberfp;
	memberfp = fopen("member.txt", "a");
	
	if (memberfp == NULL) {
		printf("Error to open this file!\n");
		exit(-1);
	}
	Member member(MAX_MEMBERS);
	int numMember = 0;




}

void login() {
	char memberName[25], memberPassword[20];

	FILE* loginfp;
	loginfp = fopen("memberLogin.txt", "a");

	printf("Enter your name : ");
	rewind(stdin);
	scanf("%s", &memberName);

	//to do check same name & pw
	if (memberName == ) {
		printf("Username already exists.Please choose a different one.\n");
		return;
	}

	printf("Enter password: ");
	rewind(stdin);
	scanf("%s", memberPassword);

	printf("Sign Up successful!\n");

	return 0;
}


void membernameExists() {
	char memberName[25], memberpassword[20];
	FILE* existfp;
	existfp = fopen("member.txt", "r");

	if (memberName == membernameExists) {
		strcmp(memberName)
	}
}