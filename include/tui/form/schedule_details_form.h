#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_DETAILS_FORM_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_DETAILS_FORM_H

#include <tui/form/schedule_form.h>
#include <schedule.h>

void init_schedule_details_form(schedule_form_t* schedule_form, WINDOW* form_window, const char* header, const char* form_header, schedule_t* schedule, tm_wday_t weekday);

#endif //TRAINTICKETINGSYSTEM_SCHEDULE_DETAILS_FORM_H
