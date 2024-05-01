#include <tui/utils/menu_utils.h>

void clear_login_menu(login_form_t* login_form) {
    reset_form(&login_form->form);
    wclear(login_form->form.window);
}