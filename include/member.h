#ifndef TRAINTICKETINGSYSTEM_MEMBER_H
#define TRAINTICKETINGSYSTEM_MEMBER_H

typedef enum Membership membership_t;
typedef struct Member member_t;
typedef struct MemberVector member_vector_t;

enum Membership {
    SILVER = 1,
    GOLD = 2,
    PLATINUM = 3
};

struct Member {
    char *username;
    char *hashed_password;
    char gender;
    char *email;
    char *contact_no;
    membership_t membership;
};

struct MemberVector {
    int max_size;
    member_t** array;
    int num_of_members;
};

member_t* init_member();
member_vector_t* init_members_vector();

int create_member_record(member_vector_t *members, char *name, char *password, char gender, char* email, char* contact_no, enum Membership membership);

int load_members(member_vector_t* members, const char* filepath);
int write_members(member_vector_t * members, const char* filepath);

member_t* login_as_member(member_vector_t * members, const char* username, const char* password);

int find_member_index(member_vector_t *members, const char* username);
#define is_member_exists(members, memberName) find_member_index(members, memberName) != -1

int add_member(member_vector_t* members, member_t* member);
int delete_member(member_vector_t* members, const char* username);

void free_member(member_t* member);
void free_members_vector(member_vector_t* members);

#endif //TRAINTICKETINGSYSTEM_MEMBER_H
