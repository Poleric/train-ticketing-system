#include <member.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#define MAX_MEMBERS 100

void member();
void signup(const char* memberName, const char* memberPwd);
void login();
void memberNameExists(const char* memberName[25]);
void loadFile();
int loginVal(const char* memberName, const char* memberPwd);

typedef struct {
	char memberName[25];
	char memberPwd[20];
}Member;

Member members[MAX_MEMBERS], newMember;
int numMembers = 0;

void signup(const char *memberName,const char *memberPwd) {
	system("clear");
	Member newMember; //give this struct a variable (name)
	char memberName[25], memberPwd[20];

	FILE* signupfp;
	signupfp = fopen("memberSignup.txt", "a");
	if (signupfp == NULL) {
		printf("Error to open this file! Please it again\n");
		exit(-1);
	}
	else {
		fprintf(signupfp, "%s %s\n", newMember.memberName, newMember.memberPwd);
		fclose(signupfp);
	}
	
	//name
	printf("Enter full name : ");
	rewind(stdin);
	scanf("%s", newMember.memberName);

	//check username exists
	if (memberNameExists(newMember.memberName)) {
		printf("Username already exists. Please choose a different one.\n");
		return;
	}
	//pwd
	printf("Enter password : ");
	rewind(stdin);
	scanf("%s", &newMember.memberPwd);

	members[numMembers++] = newMember;
	printf("Account created succesfully!\n");

	fwrite(&newMember, sizeof(newMember), 1, signupfp);
	printf("Sign Up successful!\n");
	fclose(loginfp);

	getch();
	system("clear");
	login();
	
}

void loadFile() {
	system("clear");
	FILE* load;
	load = fopen("memberSignup.txt", "r");
	if ("memberSignup" != NULL) {
		while (fscanf(load, "%s %s", members.[numMembers].memberName, members.memberPwd) == 2) {
			numMembers++;
			if (numMembers >= MAX_MEMBERS) {
				break;
			}
		}
		fclose(load);
	}
	else {
		printf("Error to open this file!\n");
	}
	system("clear");
}

void login() {
	system("clear");
	char memberName[25], memberPwd[20];
	FILE* log;
	log = fopen("memberLogin.txt", "r");
	Member members;
	printf("Enter full name : ");
	rewind(stdin);
	scanf("%s", &memberName);
	printf("Enter password : ");
	rewind(stdin);
	scanf("%s", &memberPwd);

	while (fread(&members, sizeof(members), 1, log)) {
		if (strcmp(memberName, members.memberName) == 0 && strcmp(memberPwd, members.memberPwd) == 0) {
			printf("Succesful login!\n");
		}
		else {
			printf("Please enter correct username or password!\n");
		}
	}
	fclose(log);
}

int loginVal(const char* memberName, const char* memberPwd) {
	for (int i = 0; i < numMembers; i++) {
		if (strcmp(members[i].memberName, memberName) == 0 && strcmp(members[i].memberPwd, memberPwd) == 0) {
			return 1;
		}
	}
	return 0;
}
/*
void member() {
	system("clear");

	int numMember = 0;
	bool check;
	char memberName[25], memberPwd[20];

	FILE* memberfp;
	memberfp = fopen("member.txt", "a");
	
	if (memberfp == NULL) {
		printf("Error to open this file!\n");
		exit(-1);
	}
	Member member(MAX_MEMBERS);
	int numMember = 0;




}*/

void memberNameExists(const char *memberName[25]) {
	char memberName[25], memberPwd[20];
	for (int i = 0; i < numMembers; i++) {
		if (strcmp(members[i].memberName, memberName) == 0) {
			return 1;
		}
	}
	return 0;
}