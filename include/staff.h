#ifndef TRAINTICKETINGSYSTEM_STAFF_H
#define TRAINTICKETINGSYSTEM_STAFF_H

enum Permissions {
    MANAGE_MEMBER = 0b1,
    MANAGE_STAFF = 0b10,
};

typedef struct Staff {
    char* username;
    char* hashed_password;
    char* email;
    char* contact_no;
    char* position;
    enum Permissions permissions;
    double salary;
} staff_t;

typedef struct StaffVector {
    int max_size;
    staff_t** array;
    int num_of_staff;
} staff_vector_t;

staff_t* init_staff();
staff_vector_t* init_staff_vector();

int create_staff_record(staff_vector_t* staff_v, char* name, char* password, char email, char* contact_no, enum Permissions permissions, double salary);
int load_staff(staff_vector_t* staff_v);
int write_staff(staff_vector_t* staff_v);

staff_t* login_as(staff_vector_t* staff_v, const char* username, const char* password);

int find_member_index(staff_vector_t* staff_v, const char* username);
#define is_member_exists(staff, memberName) find_member_index(staff, memberName) != -1
int add_member(staff_vector_t* staff_v, staff_t* staff);

void free_member(staff_t* staff);
void free_members_vector(staff_vector_t* staff_v);

#endif //TRAINTICKETINGSYSTEM_STAFF_H
