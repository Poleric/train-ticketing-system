#include <tui/utils/menu_utils.h>

void clear_login_menu(login_form_t* login_form) {
    reset_form(&login_form->form);
    wclear(login_form->form.window);
}

void print_minori(WINDOW* window) {
    int x, _;
    (void)_;

    getyx(window, _, x);

    wprintw(window, "%s", MINORI3);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI4);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI5);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI6);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI7);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI8);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI9);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI10);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI11);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI12);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI13);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI14);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI15);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI16);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI17);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI18);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI19);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI20);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI21);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI22);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI23);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI24);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI25);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI26);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI27);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI28);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI29);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI30);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI31);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI32);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI33);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI34);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI35);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI36);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI37);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI38);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI39);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI40);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI41);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI42);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI43);
    move_to_x(window, x);
    wprintw(window, "%s", MINORI44);
    move_to_x(window, x);
}