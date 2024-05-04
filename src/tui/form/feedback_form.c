#include <tui/form/feedback_form.h>
#include <string.h>
#include <utils.h>

#define FEEDBACK_LABEL_1 "Type    :"
#define FEEDBACK_LABEL_2 "Content :"
#define FEEDBACK_LABEL_LENGTH strlen(FEEDBACK_LABEL_1)
#define FEEDBACK_FIELD_GAP 1

#define FEEDBACK_FORM_SIDE_MARGIN_PERCENTAGE 0.1
#define FEEDBACK_FORM_TOP_MARGIN_PERCENTAGE 0.2

void init_feedback_form(feedback_form_t* feedback_form, WINDOW* form_window, const char* header, const char* form_header) {
    init_form(
            &feedback_form->form,
            form_window,
            2,
            MAX(
                    getmaxx(form_window) - FEEDBACK_LABEL_LENGTH - FEEDBACK_FIELD_GAP - (int)(getmaxx(form_window) * FEEDBACK_FORM_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    feedback_form->form.field_label_width = FEEDBACK_LABEL_LENGTH;

    feedback_form->header = header;
    feedback_form->header_y = 0;

    feedback_form->form_header = form_header;
    feedback_form->form_header_y = (int)(getmaxy(form_window) * FEEDBACK_FORM_TOP_MARGIN_PERCENTAGE);

    feedback_form->form.fields[0].label = FEEDBACK_LABEL_1;
    feedback_form->form.fields[0].y = feedback_form->form_header_y + 2;
    feedback_form->form.fields[0].offset_x = FEEDBACK_LABEL_LENGTH + FEEDBACK_FIELD_GAP;
    feedback_form->form.fields[0].x = get_centered_x_start(feedback_form->form.window, feedback_form->form.fields[0].offset_x + (int)feedback_form->form.buffer_length);
    feedback_form->form.fields[1].label = FEEDBACK_LABEL_2;
    feedback_form->form.fields[1].y = feedback_form->form_header_y + 3;
    feedback_form->form.fields[1].offset_x = FEEDBACK_LABEL_LENGTH + FEEDBACK_FIELD_GAP;
    feedback_form->form.fields[1].x = get_centered_x_start(feedback_form->form.window, feedback_form->form.fields[1].offset_x + (int)feedback_form->form.buffer_length);

    feedback_form->number_of_footers = 2;
    feedback_form->footers = calloc(feedback_form->number_of_footers, sizeof(const char*));
    feedback_form->footers[0] = "[Ctrl + C] Quit";
    feedback_form->footers[1] = "";

    feedback_form->footer_widths = calloc(feedback_form->number_of_footers, sizeof(int));
    feedback_form->footer_widths[0] = 1;
    feedback_form->footer_widths[1] = 4;

    feedback_form->form_message_y = feedback_form->form_header_y + 5;

    feedback_form->footer_y = getmaxy(form_window) - 1;

    scale_feedback_form_to_window(feedback_form);
}

void scale_feedback_form_to_window(feedback_form_t* feedback_form) {
    int max_x = getmaxx(feedback_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(feedback_form->footer_widths, feedback_form->number_of_footers);
    for (int i = 0; i < feedback_form->number_of_footers; i++)
        feedback_form->footer_widths[i] = (int)((float)feedback_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_feedback_form_fields(feedback_form_t* feedback_form) {
    int old_row = feedback_form->form.selection_row;
    feedback_form->form.selection_row = 0;
    for (int i = 0; i < feedback_form->form.number_of_fields; i++) {
        print_current_field_label(&feedback_form->form);
        print_current_field_buffer(&feedback_form->form);
        feedback_form->form.selection_row++;
    }
    feedback_form->form.selection_row = old_row;
}

void print_feedback_form_header(feedback_form_t* feedback_form, short color_pair) {
    wmove(feedback_form->form.window, feedback_form->header_y, get_centered_x_start(feedback_form->form.window, (int)strlen(feedback_form->header)));
    wprintw(feedback_form->form.window, "%s", feedback_form->header);

    move_to_x(feedback_form->form.window, 0);
    wchgat(feedback_form->form.window, feedback_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_feedback_form_form_header(feedback_form_t* feedback_form, short color_pair) {
    wmove(feedback_form->form.window, feedback_form->form_header_y, get_centered_x_start(feedback_form->form.window, (int)strlen(feedback_form->form_header)));
    wprintw(feedback_form->form.window, "%s", feedback_form->form_header);

    const int width = feedback_form->form.fields[0].offset_x + (int)feedback_form->form.buffer_length;
    move_to_x(feedback_form->form.window, feedback_form->form.fields[0].x);
    wchgat(feedback_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_feedback_form_footer(feedback_form_t* feedback_form, short color_pair) {
    wmove(feedback_form->form.window, feedback_form->footer_y, 0);

    int x = 0;
    for (int i = 0; i < feedback_form->number_of_footers; i++) {
        move_to_x(feedback_form->form.window, x);
        move_offset_x(feedback_form->form.window, get_offset_for_centered((int)strlen(feedback_form->footers[i]), feedback_form->footer_widths[i]));
        wprintw(feedback_form->form.window, "%s", feedback_form->footers[i]);
        x += feedback_form->footer_widths[i];
    }

    move_to_x(feedback_form->form.window, 0);
    wchgat(feedback_form->form.window, feedback_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_feedback_form_message(feedback_form_t* login_form, const char* message, short color_pair) {
    wmove(login_form->form.window, login_form->form_message_y, 0);
    wclrtoeol(login_form->form.window);

    move_to_x(login_form->form.window, get_centered_x_start(login_form->form.window, (int)strlen(message)));
    wattron(login_form->form.window, COLOR_PAIR(color_pair));
    wprintw(login_form->form.window, "%s", message);
    wattroff(login_form->form.window, COLOR_PAIR(color_pair));
}

void display_feedback_form(feedback_form_t* feedback_form, short color_pair) {
    print_feedback_form_header(feedback_form, color_pair);

    print_feedback_form_form_header(feedback_form, color_pair);

    print_feedback_form_fields(feedback_form);

    print_feedback_form_footer(feedback_form, color_pair);

    move_cursor_to_input_field(&feedback_form->form);

    wrefresh(feedback_form->form.window);
}

void free_feedback_form(feedback_form_t* feedback_form) {
    cleanup_form(&feedback_form->form);

    free(feedback_form->footers);
    free(feedback_form->footer_widths);
}

char* get_feedback_type(feedback_form_t* feedback_form) {
    return feedback_form->form.fields[0].buffer;
}

char* get_feedback_content(feedback_form_t* feedback_form) {
    return feedback_form->form.fields[1].buffer;
}

int save_feedback(feedback_form_t* feedback_form, const char* filepath, char* username) {
    FILE* fptr;

    fptr = fopen(filepath, "a+");

    if (fptr == NULL)
        return EXIT_FAILURE;

    fprintf(fptr, "%s\t%s\t%s\n", username, get_feedback_type(feedback_form), get_feedback_content(feedback_form));

    fclose(fptr);
    return EXIT_SUCCESS;
}