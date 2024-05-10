#include <tui/form/schedule_details_form.h>
#include <string.h>

void init_schedule_details_form(schedule_form_t* schedule_form, WINDOW* form_window, const char* header, const char* form_header, schedule_t* schedule, tm_wday_t weekday) {
    init_schedule_form(schedule_form, form_window, header, form_header);

    strncpy(schedule_form->form.fields[0].buffer, schedule->train_id, schedule_form->form.buffer_length);
    strncpy(schedule_form->form.fields[1].buffer, schedule->from_station_id, schedule_form->form.buffer_length);
    strncpy(schedule_form->form.fields[2].buffer, schedule->to_station_id, schedule_form->form.buffer_length);
    snprintf(schedule_form->form.fields[3].buffer, schedule_form->form.buffer_length, "%d", weekday);
    snprintf(schedule_form->form.fields[4].buffer, schedule_form->form.buffer_length, "%02d:%02d", schedule->departure_time.tm_hour, schedule->departure_time.tm_min);
    snprintf(schedule_form->form.fields[5].buffer, schedule_form->form.buffer_length, "%02d:%02d", schedule->eta.tm_hour, schedule->eta.tm_min);
    snprintf(schedule_form->form.fields[6].buffer, schedule_form->form.buffer_length, "%d", schedule->n_seats);
    snprintf(schedule_form->form.fields[7].buffer, schedule_form->form.buffer_length, "%.2lf", schedule->price);
}