#include <tui/start_menu.h>
#include <tui/menu/member_menu.h>
#include <tui/form/member_details_form.h>
#include <tui/form/register_member_form.h>
#include <tui/form/forgot_password_form.h>
#include <tui/form/feedback_form.h>
#include <tui/table/member_ticket_table.h>
#include <string.h>
#ifdef EMAIL_RECOVERY_FEATURE
#include <recovery_mail.h>
#endif


#define MEMBER_LOGIN_HEADER "Login as Member"
#define MEMBER_REGISTRATION_HEADER "Registration"
#define FEEDBACK_FORM_HEADER "Send a feedback"

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR (60 * SECONDS_IN_MINUTE)
#define SECONDS_IN_DAYS (24 * SECONDS_IN_HOUR)

void reload_members(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    members->num_of_members = 0;
    load_members(members, MEMBERS_FILEPATH);
}

// member_login -> member_menu   --┐
//   / \        -> register_menu --|
//    └----------------------------┘
current_menu_t member_login_menu(WINDOW* menu_window) {
    WINDOW* login_window;
    login_form_t login_form;

    member_vector_t* members;
    member_t* current_member;

    login_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_login_form(&login_form, login_window, TITLE, MEMBER_LOGIN_HEADER);
    display_login_form(&login_form, COLOR_1);

    members = init_members_vector();
    load_members(members, MEMBERS_FILEPATH);

    current_menu_t current_menu = MEMBER_MENU;
    while (current_menu == MEMBER_MENU) {
        switch (form_driver(&login_form.form, wgetch(login_window))) {
            case SUBMIT_ACTION:
                current_member = login_as_member(members, get_username(&login_form), get_password(&login_form));
                if (current_member) {
                    clear_login_menu(&login_form);

                    member_menu(menu_window, members, current_member);

                    display_login_form(&login_form, COLOR_1);
                }
                else {
                    store_last_pos(login_window);

                    print_login_form_message(&login_form, "Incorrect username or password", ERROR);

                    restore_last_pos(login_window);
                }
                break;

            case SWITCH_MENU_ACTION:
                clear_login_menu(&login_form);

                current_menu = STAFF_MENU;
                break;

            case REGISTER_ACTION:
                clear_login_menu(&login_form);

                member_registration_menu(menu_window, members);

                display_login_form(&login_form, COLOR_1);
                break;

#ifdef EMAIL_RECOVERY_FEATURE
            case FORGOT_PASSWORD_ACTION:
                clear_login_menu(&login_form);

                member_forgot_password(menu_window, members);

                display_login_form(&login_form, COLOR_1);
                break;
#endif

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
                reload_members(members);
                break;

            case REFRESH_SCREEN_ACTION:
                display_login_form(&login_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_members(members, MEMBERS_FILEPATH);

    free_login_form(&login_form);
    free_members_vector(members);
    delwin(login_window);

    return current_menu;
}

void member_menu(WINDOW* menu_window, member_vector_t* members, member_t* member) {
    WINDOW* member_menu_window;
    member_menu_t member_menu;

    member_menu_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    char header_message[50];
    snprintf(header_message, 50, "Welcome %s", member->username);

    init_member_menu(&member_menu, member_menu_window, TITLE, header_message);
    display_member_menu(&member_menu, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (menu_driver(&member_menu.menu, wgetch(member_menu_window))) {
            case SUBMIT_ACTION:
                cleanup_menu(&member_menu.menu);

                switch (member_menu.menu.selected_option) {
                    case 0:  // book ticket
                        view_schedule_menu(menu_window, member);
                        break;
                    case 1:  // view own ticket
                        view_ticket_menu(menu_window, member);
                        break;
                    case 2:  // view own details
                        view_member_details_menu(menu_window, members, member);
                        break;
                    case 3:  // send feedback
                        member_feedback_form(menu_window, member);
                        break;
                    case 4:  // logout
                        exit = true;
                        break;
                }

                if (!exit) display_member_menu(&member_menu, COLOR_1);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            case REFRESH_SCREEN_ACTION:
                display_member_menu(&member_menu, COLOR_1);
                break;

            default:
                break;
        }

        // uh cleanup somehow affect member_menu_window
        keypad(member_menu_window, true);
        curs_set(0);
    }

    free_member_menu(&member_menu);

    delwin(member_menu_window);
}

void view_member_details_menu(WINDOW* menu_window, member_vector_t* members, member_t* member) {
    register_form_t register_form;
    WINDOW* register_form_window;

    register_form_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_member_details_form(&register_form, register_form_window, TITLE, MEMBER_REGISTRATION_HEADER, member);
    display_register_form(&register_form, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&register_form.form, wgetch(register_form_window))) {
            case SUBMIT_ACTION:
                if (is_member_exists(members, get_register_username(&register_form)) &&
                    strcmp(member->username, get_register_username(&register_form)) != 0) {
                    print_register_form_message(&register_form, "Username is taken", ERROR);
                    break;
                }

                if (!validate_email(&register_form)) {
                    print_register_form_message(&register_form, "Email is invalid", ERROR);
                    break;
                }

                if (!validate_same_password(&register_form) &&
                    strlen(get_register_password(&register_form))) {
                    print_register_form_message(&register_form, "Password does not match", ERROR);
                    break;
                }

                if (!validate_gender(&register_form)) {
                    print_register_form_message(&register_form, "Invalid gender", ERROR);
                    break;
                }

                if (!validate_contact_no(&register_form)) {
                    print_register_form_message(&register_form, "Invalid contact no", ERROR);
                    break;
                }

                // saving
                if (strcmp(member->username, get_register_username(&register_form)) != 0) {
                    free(member->username);
                    member->username = strdup(get_register_username(&register_form));
                }
                if (strcmp(member->email, get_register_username(&register_form)) != 0) {
                    free(member->email);
                    member->email = strdup(get_register_email(&register_form));
                }
                if (strlen(get_register_password(&register_form)) > 0) {
                    hash_message(get_register_password(&register_form), member->hashed_password);
                }
                member->gender = get_register_gender(&register_form);
                if (strcmp(member->contact_no, get_register_contact_no(&register_form)) != 0) {
                    free(member->contact_no);
                    member->contact_no = strdup(get_register_contact_no(&register_form));
                }

                exit = true;
                print_register_form_message(&register_form, "Details saved. Press any key", GOOD);
                wgetch(register_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(register_form.form.window, "Return to menu?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_register_form(&register_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_members(members, MEMBERS_FILEPATH);

    free_register_form(&register_form);

    delwin(register_form_window);
}

void member_registration_menu(WINDOW* menu_window, member_vector_t* members) {
    register_form_t register_form;
    WINDOW* register_window;

    register_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_register_form(&register_form, register_window, TITLE, MEMBER_REGISTRATION_HEADER);
    display_register_form(&register_form, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&register_form.form, wgetch(register_window))) {
            case SUBMIT_ACTION:
                if (is_member_exists(members, get_register_username(&register_form))) {
                    print_register_form_message(&register_form, "Username is taken", ERROR);
                    break;
                }

                if (!validate_email(&register_form)) {
                    print_register_form_message(&register_form, "Email is invalid", ERROR);
                    break;
                }

                if (!validate_same_password(&register_form)) {
                    print_register_form_message(&register_form, "Password does not match", ERROR);
                    break;
                }

                if (!validate_gender(&register_form)) {
                    print_register_form_message(&register_form, "Invalid gender", ERROR);
                    break;
                }

                if (!validate_contact_no(&register_form)) {
                    print_register_form_message(&register_form, "Invalid contact no", ERROR);
                    break;
                }

                create_member_record(
                        members,
                        get_register_username(&register_form),
                        get_register_password(&register_form),
                        get_register_gender(&register_form),
                        get_register_email(&register_form),
                        get_register_contact_no(&register_form),
                        NONE
                );

                print_register_form_message(&register_form, "Successfully registered. Press any key to continue.", GOOD);
                exit = true;

                wgetch(register_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(register_form.form.window, "Exit registration ?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_register_form(&register_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_members(members, MEMBERS_FILEPATH);

    free_register_form(&register_form);

    delwin(register_window);
}

#ifdef EMAIL_RECOVERY_FEATURE
void member_forgot_password(WINDOW* menu_window, member_vector_t* members) {
    forgot_password_form_t forgot_password_form;
    WINDOW* forgot_password_window;

    forgot_password_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_forgot_password_form(&forgot_password_form, forgot_password_window, TITLE, MEMBER_REGISTRATION_HEADER);
    display_forgot_password_form(&forgot_password_form, COLOR_1);

    int code;
    char* email;
    member_t* member;
    bool exit = false;
    while (!exit) {
        switch (forgot_password_form_driver(&forgot_password_form, wgetch(forgot_password_window))) {
            case RELOAD_ACTION:
                code = generate_random_code(1, 999999);
                email = get_recovery_email(&forgot_password_form);

                print_forgot_password_form_message(&forgot_password_form, "Sending recovery code", SELECTED);
                wrefresh(forgot_password_window);

                for (int i = 0; i < members->num_of_members; i++)
                    if (strcmp(members->array[i]->email, email) == 0) {
                        member = members->array[i];
                        send_recovery(members->array[i]->email, code);
                        break;
                    }
                print_forgot_password_form_message(&forgot_password_form, "Recovery code sent", GOOD);
                move_cursor_to_input_field(&forgot_password_form.form);
                break;

            case SUBMIT_ACTION:
                if (get_recovery_code(&forgot_password_form) == code) {
                    init_reset_password_fields(&forgot_password_form);
                    display_forgot_password_form(&forgot_password_form, COLOR_1);

                    while (!exit) {
                        switch (form_driver(&forgot_password_form.form, wgetch(forgot_password_window))) {
                            case SUBMIT_ACTION:
                                if (!validate_new_password_same(&forgot_password_form)) {
                                    print_forgot_password_form_message(&forgot_password_form, "Password does not match", ERROR);
                                    break;
                                }
                                hash_message(get_recovery_password(&forgot_password_form), member->hashed_password);
                                exit = true;

                                print_forgot_password_form_message(&forgot_password_form, "Password reset successful. Please login again.", GOOD);
                                wgetch(forgot_password_window);
                                break;

                            case EXIT_FORM_ACTION:
                                exit = true;
                                break;

                            case FORGOT_PASSWORD_ACTION:
                                if (confirmation_menu(forgot_password_form.form.window, "Go back login ?") == EXIT_SUCCESS) {
                                    exit = true;
                                    break;
                                }

                            default:
                                break;
                        }
                    }

                    break;
                }
                print_forgot_password_form_message(&forgot_password_form, "Incorrect code", ERROR);

                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case FORGOT_PASSWORD_ACTION:
                if (confirmation_menu(forgot_password_form.form.window, "Go back login ?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_forgot_password_form(&forgot_password_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    free_forgot_password_form(&forgot_password_form);

    delwin(forgot_password_window);
}
#endif

void member_feedback_form(WINDOW* menu_window, member_t* member) {
    WINDOW* feedback_window;
    feedback_form_t feedback_form;

    feedback_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_feedback_form(&feedback_form, feedback_window, TITLE, FEEDBACK_FORM_HEADER);
    display_feedback_form(&feedback_form, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&feedback_form.form, wgetch(feedback_window))) {
            case SUBMIT_ACTION:
                if (strlen(get_feedback_type(&feedback_form)) == 0) {
                    print_feedback_form_message(&feedback_form, "Please specify the type of feedback", ERROR);
                    break;
                }

                if (strlen(get_feedback_content(&feedback_form)) == 0) {
                    print_feedback_form_message(&feedback_form, "Please describe your feedback", ERROR);
                    break;
                }
                save_feedback(&feedback_form, FEEDBACKS_FILEPATH, member->username);
                exit = true;
                print_feedback_form_message(&feedback_form, "Feedback received. Press any key", GOOD);
                wgetch(feedback_window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            default:
                break;
        }
    }

    free_feedback_form(&feedback_form);
    delwin(feedback_window);
}


void view_ticket_menu(WINDOW* menu_window, member_t* member) {
    WINDOW* member_ticket_window;
    member_ticket_table_t member_ticket_table;

    member_ticket_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_member_ticket_table(member_ticket_window, &member_ticket_table, member);

    train_ticket_t* selected_ticket;
    const time_t NOW = time(NULL);
    bool exit = false;
    do {
        display_tickets(&member_ticket_table);
        switch (wgetch(member_ticket_table.table.window)) {
            case 'q':
            case CTRL('C'):
                exit = true;
                break;

            case KEY_UP:  // TODO: fix scrolling
                if (member_ticket_table.table.current_line > 0 &&
                    member_ticket_table.table.selected_line < member_ticket_table.tickets->num_of_tickets - member_ticket_table.table.number_of_display_lines + 1
                        )
                    member_ticket_table.table.current_line--;
                if (member_ticket_table.table.selected_line > 0)
                    member_ticket_table.table.selected_line--;
                break;

            case KEY_DOWN:
                if (member_ticket_table.table.current_line < member_ticket_table.tickets->num_of_tickets - member_ticket_table.table.number_of_display_lines)
                    member_ticket_table.table.current_line++;
                if (member_ticket_table.table.selected_line < member_ticket_table.tickets->num_of_tickets - 1)
                    member_ticket_table.table.selected_line++;
                break;

            case 'D':
            case 'd':
                selected_ticket = get_member_selected_ticket(&member_ticket_table);
                if ((NOW - selected_ticket->order_timestamp > 7 * SECONDS_IN_DAYS) ||
                    (selected_ticket->timestamp - NOW < 3 * SECONDS_IN_DAYS)) {
                    const char* error_text = "Refund ineligible. Purchased less than a week and 3 days before departure.";
                    wattron(member_ticket_window, COLOR_PAIR(ERROR));
                    wmove(member_ticket_window, getmaxy(member_ticket_window) - 2, get_centered_x_start(member_ticket_window,(int)strlen(error_text)));
                    wprintw(member_ticket_window, "%s", error_text);
                    wattroff(member_ticket_window, COLOR_PAIR(ERROR));
                    wrefresh(member_ticket_window);
                    wgetch(member_ticket_window);
                    break;
                }

                if (confirmation_menu(member_ticket_window, "Confirm Refund?") == EXIT_SUCCESS)
                    delete_ticket(TICKETS_FILEPATH, get_member_selected_ticket(&member_ticket_table)->ticket_id);
                curs_set(0);
                break;

//            case 'E':
//            case 'e':
//
//                break;
        }
        wclear(member_ticket_table.table.window);
    } while (!exit);

    free_member_ticket_table(&member_ticket_table);
}