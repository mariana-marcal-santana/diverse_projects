
#include "show.h"
#include "support.h"
#include "sort.h"

void show_direct_flights(char *origin, char *destiny , Flight_list *flights_list) {
    Flight *flight = flights_list->head;
    
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight->arrival_IATA, destiny)) {
            print_flight_information(flight);
            printf("\n");
        }
        flight = flight->next;
    }
}

void show_flights_1connection(char *origin, char *destiny , Flight_list *flights_list) {

    Flight *flight = flights_list->head;
    while (flight != NULL) {

        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {

            if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight2->arrival_IATA, destiny) &&
            !strcmp(flight->arrival_IATA, flight2->depart_IATA)) {

                print_flight_information(flight);
                printf("connects to ");
                print_flight_information(flight2);
                printf("\n");
            }
            flight2 = flight2->next;
        }
        flight = flight->next;
    }
}

void show_flights_2connections(char *origin, char *destiny , Flight_list *flights_list) {

    Flight *flight = flights_list->head;
    while (flight != NULL) {

        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {

            Flight *flight3 = flights_list->head;
            while(flight3 != NULL) {

                if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight2->depart_IATA, flight->arrival_IATA) &&
                !strcmp(flight3->arrival_IATA, destiny) && !strcmp(flight2->arrival_IATA, flight3->depart_IATA)) {
                    
                    print_flight_information(flight);
                    printf("connects to ");
                    print_flight_information(flight2);
                    printf("connects to ");
                    print_flight_information(flight3);
                    printf("\n");
                }
                flight3 = flight3->next;
            }
            flight2 = flight2->next;
        }
        flight = flight->next;
    }
    
}

void show_direct_flights_sorted(char *origin, char *destiny, char *sort_type , Flight_list *flights_list) {

    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    
    Flight *flight = flights_list->head;

    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight->arrival_IATA, destiny)) {
            
            Flight *new_flight = malloc(sizeof(Flight));
            new_flight->airline = malloc(10 * sizeof(char));
            new_flight->flight_code = malloc(10 * sizeof(char));
            new_flight->depart_IATA = malloc(5 * sizeof(char));
            new_flight->arrival_IATA = malloc(5 * sizeof(char));
            new_flight->airline = flight->airline;
            new_flight->flight_code = flight->flight_code;
            new_flight->depart_IATA = flight->depart_IATA;
            new_flight->arrival_IATA = flight->arrival_IATA;
            new_flight->depart_time_hour = flight->depart_time_hour;
            new_flight->depart_time_minute = flight->depart_time_minute;
            new_flight->arrival_time_hour = flight->arrival_time_hour;
            new_flight->arrival_time_minute = flight->arrival_time_minute;
            new_flight->next = NULL;
            new_flight->prev = NULL;

            if (sorted_flights->head == NULL) {
                sorted_flights->head = new_flight;
                sorted_flights->tail = new_flight;
            } else {
                sorted_flights->head->prev = new_flight;
                new_flight->next = sorted_flights->head;
                sorted_flights->head = new_flight;
            }
        }
        flight = flight->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {
        print_flight_information(sorted_flight);
        printf("\n");
        sorted_flight = sorted_flight->next;
    }
}

void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type , Flight_list *flights_list) {
    
    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    
    Flight *flight = flights_list->head;
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin)) {
            
            Flight *new_flight = malloc(sizeof(Flight));
            new_flight->airline = malloc(10 * sizeof(char));
            new_flight->flight_code = malloc(10 * sizeof(char));
            new_flight->depart_IATA = malloc(5 * sizeof(char));
            new_flight->arrival_IATA = malloc(5 * sizeof(char));
            new_flight->airline = flight->airline;
            new_flight->flight_code = flight->flight_code;
            new_flight->depart_IATA = flight->depart_IATA;
            new_flight->arrival_IATA = flight->arrival_IATA;
            new_flight->depart_time_hour = flight->depart_time_hour;
            new_flight->depart_time_minute = flight->depart_time_minute;
            new_flight->arrival_time_hour = flight->arrival_time_hour;
            new_flight->arrival_time_minute = flight->arrival_time_minute;
            new_flight->next = NULL;
            new_flight->prev = NULL;

            if (sorted_flights->head == NULL) {
                sorted_flights->head = new_flight;
                sorted_flights->tail = new_flight;
            } else {
                sorted_flights->head->prev = new_flight;
                new_flight->next = sorted_flights->head;
                sorted_flights->head = new_flight;
            }
        }
        flight = flight->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {

        Flight *sorted_flight2 = flights_list->head;
        while(sorted_flight2 != NULL) {

            if (!strcmp(sorted_flight2->arrival_IATA, destiny) && 
            !strcmp(sorted_flight->arrival_IATA, sorted_flight2->depart_IATA) &&
            (sorted_flight->arrival_time_hour < sorted_flight2->depart_time_hour ||
            (sorted_flight->arrival_time_hour == sorted_flight2->depart_time_hour &&
            sorted_flight->arrival_time_minute < sorted_flight2->depart_time_minute))) {

                print_flight_information(sorted_flight);
                printf("connects to ");
                print_flight_information(sorted_flight2);
                printf("\n");
            }
            sorted_flight2 = sorted_flight2->next;
        }
        sorted_flight = sorted_flight->next;
    }
}

void show_flights_2connections_sorted(char *origin, char *destiny, char *sort_type , Flight_list *flights_list) {

    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;

    Flight *flight = flights_list->head;
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin)) {
            
            Flight *new_flight = malloc(sizeof(Flight));
            new_flight->airline = malloc(10 * sizeof(char));
            new_flight->flight_code = malloc(10 * sizeof(char));
            new_flight->depart_IATA = malloc(5 * sizeof(char));
            new_flight->arrival_IATA = malloc(5 * sizeof(char));
            new_flight->airline = flight->airline;
            new_flight->flight_code = flight->flight_code;
            new_flight->depart_IATA = flight->depart_IATA;
            new_flight->arrival_IATA = flight->arrival_IATA;
            new_flight->depart_time_hour = flight->depart_time_hour;
            new_flight->depart_time_minute = flight->depart_time_minute;
            new_flight->arrival_time_hour = flight->arrival_time_hour;
            new_flight->arrival_time_minute = flight->arrival_time_minute;
            new_flight->next = NULL;
            new_flight->prev = NULL;

            if (sorted_flights->head == NULL) {
                sorted_flights->head = new_flight;
                sorted_flights->tail = new_flight;
            } else {
                sorted_flights->head->prev = new_flight;
                new_flight->next = sorted_flights->head;
                sorted_flights->head = new_flight;
            }
        }
        flight = flight->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {

        Flight *sorted_flight2 = flights_list->head;
        while(sorted_flight2 != NULL) {

            Flight *sorted_flight3 = flights_list->head;
            while (sorted_flight3 != NULL) {

                if (!strcmp(sorted_flight3->arrival_IATA, destiny) && 
                !strcmp(sorted_flight->arrival_IATA, sorted_flight2->depart_IATA) &&
                !strcmp(sorted_flight2->arrival_IATA, sorted_flight3->depart_IATA) &&
                (sorted_flight->arrival_time_hour < sorted_flight2->depart_time_hour ||
                (sorted_flight->arrival_time_hour == sorted_flight2->depart_time_hour &&
                sorted_flight->arrival_time_minute < sorted_flight2->depart_time_minute)) &&
                (sorted_flight2->arrival_time_hour < sorted_flight3->depart_time_hour ||
                (sorted_flight2->arrival_time_hour == sorted_flight3->depart_time_hour &&
                sorted_flight2->arrival_time_minute < sorted_flight3->depart_time_minute))) {
                    
                    print_flight_information(sorted_flight);
                    printf("connects to ");
                    print_flight_information(sorted_flight2);
                    printf("connects to ");
                    print_flight_information(sorted_flight3);
                    printf("\n");
                }
                sorted_flight3 = sorted_flight3-> next;
            }
            sorted_flight2 = sorted_flight2->next;
        }
        sorted_flight = sorted_flight->next;
    }
}
/*
void show_flights_shortest_distance_1connection(char *origin, char *destiny, char *sort_type, Flight_list *flights_list ) {}

void show_flights_shortest_distance_2connections(char *origin, char *destiny, char *sort_type,  Flight_list *flights_list) {}
*/