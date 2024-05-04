#ifndef TRAINTICKETINGSYSTEM_FEEDBACK_FORM_H
#define TRAINTICKETINGSYSTEM_FEEDBACK_FORM_H

#include <tui/form/form.h>

typedef struct FeedbackForm feedback_form_t;

struct FeedbackForm {
    form_t form;

    const char* header;
    int header_y;

    const char* form_header;
    int form_header_y;

    int form_message_y;

    const char **footers;
    int* footer_widths;
    int number_of_footers;
    int footer_y;
};

void init_feedback_form(feedback_form_t* feedback_form, WINDOW* form_window, const char* header, const char* form_header);

void scale_feedback_form_to_window(feedback_form_t* feedback_form);

void print_feedback_form_fields(feedback_form_t* feedback_form);
void print_feedback_form_header(feedback_form_t* feedback_form, short color_pair);
void print_feedback_form_form_header(feedback_form_t* feedback_form, short color_pair);
void print_feedback_form_footer(feedback_form_t* feedback_form, short color_pair);
void print_feedback_form_message(feedback_form_t* login_form, const char* message, short color_pair);

void display_feedback_form(feedback_form_t* feedback_form, short color_pair);

void free_feedback_form(feedback_form_t* feedback_form);

char* get_feedback_type(feedback_form_t* feedback_form);
char* get_feedback_content(feedback_form_t* feedback_form);

int save_feedback(feedback_form_t* feedback_form, const char* filepath, char* username);

#endif //TRAINTICKETINGSYSTEM_FEEDBACK_FORM_H
