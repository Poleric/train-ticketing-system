#include <tui/utils/menu_utils.h>

#include <staff.h>

int main() {
    //Suzette Wiggett    861 924 1650    swiggett@gmail.com    Manager    5000     kaibing_pro
    //Rickert Khosa    769 649 5276    rkhosa@gmail.net             staff            3000    kb&onechan
    //Graig Robillart    214 424 6446    grobillart@hotmail.com    staff        3000    gaga1233@
    //Micky Schops    485 914 4215    mschops@gmail.com    staff        3000    micky^^&
    //Joly Reignould    535 368 7581    jreignould@yahoo.edu    admin        7000    jojojojo~
    //Sally Poly            761 591 1943            sallypoly@yahoo.com.br manager    5000    iwoiwowio
    //Freddy Swaston    170 162 7493    fswainston@gmail.co.uk    staff        3000    iforgotpw11
    //Kristos Craw    630 170 7310    kcrawforth@yahoo.it    staff        3000    owo!!
    //Merrel Sambrok    943 139 9127    msamb@hotmail.com    staff            3000    1wowow11
    //Darcy Tomaselli    807 111 4561            dtomasel@yahoo.com    staff            3000    dardar<>l

    staff_vector_t* staffs;

    staffs = init_staff_vector();

    //reading
    //    load_staff(staffs, STAFFS_FILEPATH);

    create_staff_record(staffs, "Suzette Wiggett", "kaibing_pro", "swiggett@gmail.com", "861 924 1650", "manager", MANAGE_STAFF | MANAGE_MEMBER, 5000);

    //saving
    write_staff(staffs, STAFFS_FILEPATH);

    free_staff_vector(staffs);
}
