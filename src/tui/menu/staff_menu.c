#include <tui/menu/staff_menu.h>
#include <tui/utils/menu_utils.h>
#include <stdlib.h>
#include <utils.h>
#include <string.h>

#define NUMBER_OF_OPTIONS 5
#define OPTION_1 "View Staff"
#define OPTION_2 "View Schedule"
#define OPTION_3 "Edit Profile"
#define OPTION_4 "View All Tickets"
#define OPTION_5 "Logout"

#define STAFF_MENU_TOP_MARGIN_PERCENTAGE 0.2
#define STAFF_MENU_SIDE_MARGIN_PERCENTAGE 0.2

void init_staff_menu(staff_menu_t* staff_menu, WINDOW* menu_window, const char* header, char* menu_header) {
    init_menu(
            &staff_menu->menu,
            menu_window,
            -1,
            A_STANDOUT,
            SELECTED
    );

    staff_menu->header = header;
    staff_menu->header_y = 0;

    staff_menu->menu_header = menu_header;
    staff_menu->menu_header_y = (int)(getmaxy(menu_window) * STAFF_MENU_TOP_MARGIN_PERCENTAGE);

    staff_menu->menu.number_of_options = NUMBER_OF_OPTIONS;
    staff_menu->menu.menu_options = calloc(staff_menu->menu.number_of_options, sizeof(menu_option_t));
    staff_menu->menu.menu_options[0] = (menu_option_t) {
            OPTION_1,
            staff_menu->menu_header_y + 2,
            CENTERED_X
    };
    staff_menu->menu.menu_options[1] = (menu_option_t) {
            OPTION_2,
            staff_menu->menu_header_y + 4,
            CENTERED_X
    };
    staff_menu->menu.menu_options[2] = (menu_option_t) {
            OPTION_3,
            staff_menu->menu_header_y + 6,
            CENTERED_X
    };
    staff_menu->menu.menu_options[3] = (menu_option_t) {
            OPTION_4,
            staff_menu->menu_header_y + 8,
            CENTERED_X
    };
    staff_menu->menu.menu_options[4] = (menu_option_t) {
            OPTION_5,
            staff_menu->menu_header_y + 10,
            CENTERED_X
    };

    staff_menu->number_of_footers = 3;
    staff_menu->footers = calloc(staff_menu->number_of_footers, sizeof(const char*));
    staff_menu->footers[0] = "[↑↓] Scroll";
    staff_menu->footers[1] = "";
    staff_menu->footers[2] = "[Enter] Select";

    staff_menu->footer_widths = calloc(staff_menu->number_of_footers, sizeof(int));
    staff_menu->footer_widths[0] = 1;
    staff_menu->footer_widths[1] = 2;
    staff_menu->footer_widths[2] = 1;

    staff_menu->footer_y = getmaxy(menu_window) - 1;

    scale_staff_menu_to_window(staff_menu);
}

void scale_staff_menu_to_window(staff_menu_t* staff_menu) {
    int max_x = getmaxx(staff_menu->menu.window);

    // scale up / down all column widths
    const int footer_max = sum_d(staff_menu->footer_widths, staff_menu->number_of_footers);
    for (int i = 0; i < staff_menu->number_of_footers; i++)
        staff_menu->footer_widths[i] = (int)((float)staff_menu->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_staff_menu_header(staff_menu_t* staff_menu, short color_pair) {
    wmove(staff_menu->menu.window, staff_menu->header_y, get_centered_x_start(staff_menu->menu.window, (int)strlen(staff_menu->header)));
    wprintw(staff_menu->menu.window, "%s", staff_menu->header);

    move_to_x(staff_menu->menu.window, 0);
    wchgat(staff_menu->menu.window, staff_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void print_staff_menu_menu_header(staff_menu_t* staff_menu, short color_pair) {
    wmove(staff_menu->menu.window, staff_menu->menu_header_y, get_centered_x_start(staff_menu->menu.window, (int)strlen(staff_menu->menu_header)));
    wprintw(staff_menu->menu.window, "%s", staff_menu->menu_header);

    const int start_x = (int)(staff_menu->menu.width * STAFF_MENU_SIDE_MARGIN_PERCENTAGE);
    const int width = staff_menu->menu.width - 2 * start_x;
    move_to_x(staff_menu->menu.window, start_x);
    wchgat(staff_menu->menu.window, width, A_STANDOUT, color_pair, NULL);
}

void print_staff_menu_footer(staff_menu_t* staff_menu, short color_pair) {
    wmove(staff_menu->menu.window, staff_menu->footer_y, 0);

    int x = 0;
    for (int i = 0; i < staff_menu->number_of_footers; i++) {
        move_to_x(staff_menu->menu.window, x);
        move_offset_x(staff_menu->menu.window, get_offset_for_centered((int)strlen(staff_menu->footers[i]), staff_menu->footer_widths[i]));
        wprintw(staff_menu->menu.window, "%s", staff_menu->footers[i]);
        x += staff_menu->footer_widths[i];
    }

    move_to_x(staff_menu->menu.window, 0);
    wchgat(staff_menu->menu.window, staff_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void display_staff_menu(staff_menu_t* staff_menu, short color_pair) {
    print_staff_menu_header(staff_menu, color_pair);

    print_staff_menu_menu_header(staff_menu, color_pair);

    print_options(&staff_menu->menu);

    print_staff_menu_footer(staff_menu, color_pair);

    select_option(&staff_menu->menu, staff_menu->menu.selected_option);

    wrefresh(staff_menu->menu.window);
}

void free_staff_menu(staff_menu_t* staff_menu) {
    cleanup_menu(&staff_menu->menu);

    free(staff_menu->menu.menu_options);
    free(staff_menu->footers);
    free(staff_menu->footer_widths);
}