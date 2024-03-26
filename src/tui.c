#include <tui.h>
#include <string.h>

const char* MINORI = "      ..........................................................................................      \n"
                     "      ..........................................................................................      \n"
                     "      ..........................................................................................      \n"
                     "      ...............................................:::........................................      \n"
                     "      ................................::=+++===============:....................................      \n"
                     "      .............................:-=========================-:................................      \n"
                     "      ............................-======++++===*=+==============:..............................      \n"
                     "      ..........................:+========***+=++=====*+==========-.............................      \n"
                     "      .........................-==============*#++=======+==========-...........................      \n"
                     "      .......................:-=================#*====================..........................      \n"
                     "      ......................:=============+##*===#**######*==========+=:........................      \n"
                     "      .....................:==========+++#######++++#########+=++=======-.......................      \n"
                     "      ....................:==========*#+###########+*############+==++==+-......................      \n"
                     "      ....................=======++++###+###########+###########%#===+*=++-.....................      \n"
                     "      ...................=======+==+#%##=#######################%%#==+#+=*+=:...................      \n"
                     "      ..................-=======+=+#%*+%=#############%##########%%#=+##++===...................      \n"
                     "      ..................=======++#*#*=++*+############%%#%#######%%%#+###=*==-..................      \n"
                     "      .................-=======+**##==++#=####%##########%%#######%#%*###+++==-.................      \n"
                     "      ................:=+=====++=*%+=++++#+#%##%#######%#%%%%%####%#%####*=====-................      \n"
                     "      ................-+====+===+##-=+++++%+%%##%%#####%+*%##%%###%###%###=====-:...............      \n"
                     "      ...............:=++==++==+##-.-=+++++++%###%%#####++*#**#%#####%####====-=-...............      \n"
                     "      ...............=*+===+=+#%=:..:=++++++++%%##%###%#++++#++##%#%#%####===+:-=-..............      \n"
                     "      ..............:#++==+*++=:....:=++++++++*#%#####%%++#%@@#++##%#%+#*#====-.==:.............      \n"
                     "      ..............+=++==##:=-:..-+@%%%%%#+++*#########@%#%%%%#%@%%##++*#=====.-=-.............      \n"
                     "      .............-*+++=+#-:==:.:##--#%%%#++++++*#++++**#%%%%%%#%##%#++*#=====..==-............      \n"
                     "      ............:#=++++*%#--=-:+%--*%%%@%%+++++++#+++++#@%@@@%%###@+=++#+====:.:=-:...........      \n"
                     "      ............+*==*++*%##+--::#.:*%%@@@%+++++++++++++#%%@@@%%+#%#===+#*====:..+--...........      \n"
                     "      ...........-+=+=+++*%##%+:=:::.+%%%%@#++++++++++++++%@%%%@#+%*+===+##+===-..-=-...........      \n"
                     "      ..........-=+=++++*###%*#++-...:+%@%#+++++++*%#++++++%@@%++**#+++=+####++=-..=:...........      \n"
                     "      ..........==+=++=*+##+----=#:....-+++++++++++##++++++++++*%%#***+==*%%##*++*==:...........      \n"
                     "      .........-==+==+=+#+----+*--+:...-++++++++++++++++++*++%*==#%#####++%+=*##++%:............      \n"
                     "      .........-==+==+##=---+-.....:-:.-+++*#+++++++++++*###=-=###########+=##++#*=#-...........      \n"
                     "      .........-==+=+%=---==.........::-=++++++*++++****#+==+###############*###+*#+++:.........      \n"
                     "      .........:===%=----+:............:=-==++++++++++%+-=*#####################%%*#=-*=........      \n"
                     "      ..........:++-----=:...........:+#++=-----=+##%+=+#########################%*##=..........      \n"
                     "      .........-+=-----=-..........:-+-.........-=##+*##################%##########%+*=.........      \n"
                     "      .......:++------==..........:==........:--#%###################################=:.........      \n"
                     "      .....:=#--------=.........:-+:......::-=#%####################%####%###########*-.........      \n"
                     "      ....-*=--------=::::......--......:-==#%#####################%%####%#############+:.......      \n"
                     "      ...=+---------++..--.....-:......:-=#%######################%%#####%###########*#%*.......      \n"
                     "      ..-=-----------##+=::...--:.:-..:-=%#######################%%##################*#*%=......      \n"
                     "      .-++-------------=+*=-:-+-..:--:-+########################%%%##%%%%############*#+#%=.....      \n"
                     "      :=+=-------------------=++*+-:--=*#######################%%%#@%###%%%#########*##+*%%-....      \n"
                     "      :=-=--------=----------=-------*%#######################%%%%##################+##++#%#-...      \n";

void init_color_pairs() {
    /* Initialize preset colors pairs
     * 1 - Green background
     * 2 - Cyan background
     * 3 - Yellow background
     */
    use_default_colors();
//    assume_default_colors(-1, -1);
//    init_pair(1, COLOR_GREEN, COLOR_BLACK);
//    init_pair(2, COLOR_CYAN, COLOR_BLACK);
//    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}

void start_menu() {
    initscr();
    start_color(); init_color_pairs();
    raw();  // get input by chars
    noecho();  // don't display input
    keypad(stdscr, TRUE);

    current_menu_t menu = MEMBER_MENU;
    while (menu) {
        switch (menu) {
            case MEMBER_MENU:
                menu = member_menu();
                break;
            case STAFF_MENU:
                menu = staff_menu();
                break;
            default:
                break;
        }
    }

    endwin();
}

current_menu_t member_menu() {
    enum CurrentMenu return_menu = MEMBER_MENU;

    FIELD* fields[3] = {
            new_field(1, 20, 2, LOGIN_FIELD_OFFSET_X + 2, 0, 0),
            new_field(1, 20, 3, LOGIN_FIELD_OFFSET_X + 2, 0, 0)
    };

    FORM* login_form = new_form(fields);

    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);
    set_field_back(fields[1], A_UNDERLINE);
    field_opts_off(fields[1], O_AUTOSKIP);
    field_opts_off(fields[1], O_PUBLIC);

    post_form(login_form);
    mvprintw(1, get_centered_x_start(stdscr, 11), "Member Login");
    box(stdscr, 0, 0);
    mvprintw(2, 2, LOGIN_USERNAME_LABEL);
    mvprintw(3, 2, LOGIN_PASSWORD_LABEL);
    move(2, LOGIN_FIELD_OFFSET_X + 2);
    refresh();

    int selection_index = 0, ch;
    while (return_menu == MEMBER_MENU) {
        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                if (selection_index < 1) {
                    form_driver(login_form, REQ_NEXT_FIELD);
                    form_driver(login_form, REQ_END_LINE);
                    selection_index++;
                }
                break;
            case KEY_UP:
                if (selection_index > 0) {
                    form_driver(login_form, REQ_PREV_FIELD);
                    form_driver(login_form, REQ_END_LINE);
                    selection_index--;
                }
                break;
            case KEY_ENTER:
            case '\n':
            case '\r':
                if (selection_index < 1) {
                    form_driver(login_form, REQ_NEXT_FIELD);
                    form_driver(login_form, REQ_END_LINE);
                    selection_index++;
                }
                else {
                    store_last_pos(stdscr);
                    // clear line
                    mvprintw(5, 2, "                                ");

                    // store input
                    form_driver(login_form, REQ_VALIDATION);

                    if (strcmp(field_buffer(fields[1], 0), "abc\n") == 0) {
                        mvprintw(5, 2, "Login successful");
                    }
                    else {
                        mvprintw(5, 2, "Wrong password");
                    }
                    refresh();

                    restore_last_pos(stdscr);
                }
                break;
            case KEY_ESC:
            case CTRL('C'):
                store_last_pos(stdscr);
                if (confirmation_menu("Exit Menu?") == EXIT_SUCCESS)
                    return_menu = EXIT;
                restore_last_pos(stdscr);
                break;
            case KEY_BACKSPACE:
                form_driver(login_form, REQ_DEL_PREV);
                break;
            case KEY_F(1):
                return_menu = STAFF_MENU;
                break;
            default:
                form_driver(login_form, ch);
                break;
        }
    }

    unpost_form(login_form);
    free_form(login_form);
    for (int i = 0; ;i++) {
        if (!fields[i])
            break;
        free_field(fields[i]);
    }

    return return_menu;
}

current_menu_t staff_menu() {

}

int confirmation_menu(const char* message) {
    WINDOW* confirmation_window;

    confirmation_window = newwin(
            CONFIRMATION_WINDOW_HEIGHT,
            CONFIRMATION_WINDOW_WIDTH,
            get_centered_y_start(stdscr, CONFIRMATION_WINDOW_HEIGHT),
            get_centered_x_start(stdscr, CONFIRMATION_WINDOW_WIDTH)
            );
    keypad(confirmation_window, TRUE);  // enable arrow keys
    curs_set(0);  // hide cursor

    // border
    box(confirmation_window, 0, 0);

    // message
    mvwaddstr(confirmation_window,
              1,
              get_centered_x_start(confirmation_window, (int)strlen(message)),
              message);

    // display options
    int option_start_y = CONFIRMATION_WINDOW_HEIGHT - 2;
    int option_start_x = get_centered_x_start(confirmation_window, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH);
    mvwaddstr(confirmation_window,
              option_start_y,
              option_start_x,
              "Yes  No");

    int exit = 0;
    int selection = CONFIRMATION_DEFAULT_OPTION;
    do {
        // clear highlight
        mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, 0, 0, NULL);

        if (selection)  // highlight yes
            mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH, A_STANDOUT, 0, NULL);
        else  // highlight no
            mvwchgat(confirmation_window, option_start_y, option_start_x + CONFIRMATION_YES_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, CONFIRMATION_NO_LENGTH, A_STANDOUT, 0, NULL);
        wrefresh(confirmation_window);

        switch (wgetch(confirmation_window)) {
            case KEY_LEFT:
                selection = 1;
                break;
            case KEY_RIGHT:
                selection = 0;
                break;
            case KEY_ENTER:
            case KEY_ESC:
            case '\n':
            case '\r':
                exit = 1;
                break;
            default:
                break;
        }
    } while (!exit);

    wclear(confirmation_window);
    wrefresh(confirmation_window);
    delwin(confirmation_window);
    curs_set(1);  // return cursor
    return !selection;
}



