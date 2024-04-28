#ifndef TRAINTICKETINGSYSTEM_MEMBER_H
#define TRAINTICKETINGSYSTEM_MEMBER_H


enum Membership {
    SILVER = 0x1,
    GOLD = 0x10,
    PLATINUM = 0x100
};

typedef struct Member {
    char *id;
    char *username;
    char *hashed_password;
    char gender;
    char *email;
    char *contact_no;
    enum Membership membership;
} member_t;

typedef struct MemberVector {
    int max_size;
    member_t** array;
    int num_of_members;
} member_vector_t;

member_t* init_member();
member_vector_t* init_members_vector();

int create_member_record(member_vector_t *members, char *name, char *password, char gender, char* email, char* contact_no, enum Membership membership);
int load_members(member_vector_t* members);
int write_members(member_vector_t * members, const char * str);

member_t* login_as_member(member_vector_t * members, const char* username, const char* password);

int find_member_index(member_vector_t *members, const char* username);
#define is_member_exists(members, memberName) find_member_index(members, memberName) != -1
int add_member(member_vector_t* members, member_t* member);

void deleteAcc(member_vector_t* members, const char* username, const char* password);

void free_member(member_t* member);
void free_members_vector(member_vector_t* members);

#endif //TRAINTICKETINGSYSTEM_MEMBER_H
