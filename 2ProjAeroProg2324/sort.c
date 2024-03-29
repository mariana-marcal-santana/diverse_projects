
#include "sort.h"

void sort_flights_ascending(Flight_list *flights) {
   
    int flag = 1;
    Flight *temp = NULL;

    while (flag) {
        flag = 0;
        Flight *current_flight = flights->head;
        Flight *prev_flight = NULL;

        while (current_flight != NULL && current_flight->next != NULL) {
            if (current_flight->depart_time_hour > current_flight->next->depart_time_hour ||
                (current_flight->depart_time_hour == current_flight->next->depart_time_hour &&
                 current_flight->depart_time_minute > current_flight->next->depart_time_minute)) {
               
                if (prev_flight != NULL) {
                    prev_flight->next = current_flight->next;
                } else {
                    flights->head = current_flight->next;
                }

                temp = current_flight->next;
                current_flight->next = temp->next;
                temp->next = current_flight;

                flag = 1;
            } 
            prev_flight = current_flight;
            current_flight = current_flight->next;
        }
    }
}

void sort_flights_descending(Flight_list *flights) {

    int flag = 1;
    Flight *temp = NULL;

    while (flag) {
        flag = 0;
        Flight *current_flight = flights->head;
        Flight *prev_flight = NULL;

        while (current_flight != NULL && current_flight->next != NULL) {
            if (current_flight->depart_time_hour < current_flight->next->depart_time_hour ||
                (current_flight->depart_time_hour == current_flight->next->depart_time_hour &&
                 current_flight->depart_time_minute < current_flight->next->depart_time_minute)) {
            
                if (prev_flight != NULL) {
                    prev_flight->next = current_flight->next;
                } else {
                    flights->head = current_flight->next;
                }

                temp = current_flight->next;
                current_flight->next = temp->next;
                temp->next = current_flight;

                flag = 1;
            } 
            prev_flight = current_flight;
            current_flight = current_flight->next;
        }
    }
}
