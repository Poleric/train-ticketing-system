#include <tui/menu/member_menu.h>
#include <tui/utils/menu_utils.h>
#include <stdlib.h>
#include <utils.h>
#include <string.h>

#define NUMBER_OF_OPTIONS 5
#define OPTION_1 "Book Ticket"
#define OPTION_2 "My Ticket"
#define OPTION_3 "My Details"
#define OPTION_4 "Send Feedback"
#define OPTION_5 "Logout"

#define MEMBER_MENU_TOP_MARGIN_PERCENTAGE 0.2
#define MEMBER_MENU_SIDE_MARGIN_PERCENTAGE 0.2

void init_member_menu(member_menu_t* member_menu, WINDOW* menu_window, const char* header, char* menu_header) {
    init_menu(
            &member_menu->menu,
            menu_window,
            -1,
            A_STANDOUT,
            SELECTED
            );

    member_menu->header = header;
    member_menu->header_y = 0;

    member_menu->menu_header = menu_header;
    member_menu->menu_header_y = (int)(getmaxy(menu_window) * MEMBER_MENU_TOP_MARGIN_PERCENTAGE);

    member_menu->menu.number_of_options = NUMBER_OF_OPTIONS;
    member_menu->menu.menu_options = calloc(member_menu->menu.number_of_options, sizeof(menu_option_t));
    member_menu->menu.menu_options[0] = (menu_option_t) {
        OPTION_1,
        member_menu->menu_header_y + 2,
        CENTERED_X
    };
    member_menu->menu.menu_options[1] = (menu_option_t) {
        OPTION_2,
        member_menu->menu_header_y + 4,
        CENTERED_X
    };
    member_menu->menu.menu_options[2] = (menu_option_t) {
        OPTION_3,
        member_menu->menu_header_y + 6,
        CENTERED_X
    };
    member_menu->menu.menu_options[3] = (menu_option_t) {
        OPTION_4,
        member_menu->menu_header_y + 8,
        CENTERED_X
    };
    member_menu->menu.menu_options[4] = (menu_option_t) {
        OPTION_5,
        member_menu->menu_header_y + 10,
        CENTERED_X
    };

    member_menu->number_of_footers = 3;
    member_menu->footers = calloc(member_menu->number_of_footers, sizeof(const char*));
    member_menu->footers[0] = "[↑↓] Scroll";
    member_menu->footers[1] = "";
    member_menu->footers[2] = "[Enter] Select";

    member_menu->footer_widths = calloc(member_menu->number_of_footers, sizeof(int));
    member_menu->footer_widths[0] = 1;
    member_menu->footer_widths[1] = 2;
    member_menu->footer_widths[2] = 1;

    member_menu->footer_y = getmaxy(menu_window) - 1;

    scale_member_menu_to_window(member_menu);
}

void scale_member_menu_to_window(member_menu_t* member_menu) {
    int max_x = getmaxx(member_menu->menu.window);

    // scale up / down all column widths
    const int footer_max = sum_d(member_menu->footer_widths, member_menu->number_of_footers);
    for (int i = 0; i < member_menu->number_of_footers; i++)
        member_menu->footer_widths[i] = (int)((float)member_menu->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_member_menu_header(member_menu_t* member_menu, short color_pair) {
    wmove(member_menu->menu.window, member_menu->header_y, get_centered_x_start(member_menu->menu.window, (int)strlen(member_menu->header)));
    wprintw(member_menu->menu.window, "%s", member_menu->header);

    move_to_x(member_menu->menu.window, 0);
    wchgat(member_menu->menu.window, member_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void print_member_menu_menu_header(member_menu_t* member_menu, short color_pair) {
    wmove(member_menu->menu.window, member_menu->menu_header_y, get_centered_x_start(member_menu->menu.window, (int)strlen(member_menu->menu_header)));
    wprintw(member_menu->menu.window, "%s", member_menu->menu_header);

    const int start_x = (int)(member_menu->menu.width * MEMBER_MENU_SIDE_MARGIN_PERCENTAGE);
    const int width = member_menu->menu.width - 2 * start_x;
    move_to_x(member_menu->menu.window, start_x);
    wchgat(member_menu->menu.window, width, A_STANDOUT, color_pair, NULL);
}

void print_member_menu_footer(member_menu_t* member_menu, short color_pair) {
    wmove(member_menu->menu.window, member_menu->footer_y, 0);

    int x = 0;
    for (int i = 0; i < member_menu->number_of_footers; i++) {
        move_to_x(member_menu->menu.window, x);
        move_offset_x(member_menu->menu.window, get_offset_for_centered((int)strlen(member_menu->footers[i]), member_menu->footer_widths[i]));
        wprintw(member_menu->menu.window, "%s", member_menu->footers[i]);
        x += member_menu->footer_widths[i];
    }

    move_to_x(member_menu->menu.window, 0);
    wchgat(member_menu->menu.window, member_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void display_member_menu(member_menu_t* member_menu, short color_pair) {
    print_member_menu_header(member_menu, color_pair);

    print_member_menu_menu_header(member_menu, color_pair);

    print_options(&member_menu->menu);

    print_member_menu_footer(member_menu, color_pair);

    select_option(&member_menu->menu, member_menu->menu.selected_option);

    wrefresh(member_menu->menu.window);
}

void free_member_menu(member_menu_t* member_menu) {
    cleanup_menu(&member_menu->menu);

    free(member_menu->menu.menu_options);
    free(member_menu->footers);
    free(member_menu->footer_widths);
}