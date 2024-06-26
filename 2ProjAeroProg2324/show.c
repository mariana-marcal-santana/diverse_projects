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
            Flight *new_flight = create_flight(flight);
            add_flight_to_list(sorted_flights, new_flight);
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
    free_flights_list(sorted_flights);
}

void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type , Flight_list *flights_list) {
    
    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    
    Flight *flight = flights_list->head;
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin)) {
            Flight *new_flight = create_flight(flight);
            add_flight_to_list(sorted_flights, new_flight);
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
    free_flights_list(sorted_flights);
}

void show_flights_2connections_sorted(char *origin, char *destiny, char *sort_type , Flight_list *flights_list) {

    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;

    Flight *flight = flights_list->head;
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin)) {
            Flight *new_flight = create_flight(flight);
            add_flight_to_list(sorted_flights, new_flight);
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
    free_flights_list(sorted_flights);
}

void show_flights_shortest_distance_1connection(char *origin, char *destiny, char *sort_type, Flight_list *flights_list ) {
    
    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    double distance_min = 100000000;

    Flight *flight1 = flights_list->head;
    while (flight1 != NULL) {
        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {
            if (!strcmp(flight1->depart_IATA, origin) && !strcmp(flight2->arrival_IATA, destiny) &&
            !strcmp(flight1->arrival_IATA, flight2->depart_IATA)) {
                if (distance_min > flight1->distance + flight2->distance) {
                    distance_min = flight1->distance + flight2->distance;
                    clean_flights_list(sorted_flights);
                    sorted_flights = realloc(sorted_flights, sizeof(Flight_list));
                    Flight *new_flight = create_flight(flight1);
                    add_flight_to_list(sorted_flights, new_flight);
                }
                else if (distance_min == flight1->distance + flight2->distance) {
                    Flight *new_flight = create_flight(flight1);
                    add_flight_to_list(sorted_flights, new_flight);   
                }
            }
            flight2 = flight2->next;
        }
        flight1 = flight1->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {
        Flight *sorted_flight2 = flights_list->head;
        while(sorted_flight2 != NULL) {
            if (distance_min == sorted_flight->distance + sorted_flight2->distance && 
            !strcmp(sorted_flight2->arrival_IATA, destiny) && 
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
    free_flights_list(sorted_flights);
}

void show_flights_shortest_distance_2connections(char *origin, char *destiny, char *sort_type,  Flight_list *flights_list) {

    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    double distance_min = 100000000;

    Flight *flight1 = flights_list->head;
    while (flight1 != NULL) {
        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {
            Flight *flight3 = flights_list->head;
            while (flight3 != NULL) {
                if (!strcmp(flight1->depart_IATA, origin) && !strcmp(flight3->arrival_IATA, destiny) &&
                !strcmp(flight1->arrival_IATA, flight2->depart_IATA) && !strcmp(flight2->arrival_IATA, flight3->depart_IATA)) {
                    if (distance_min > flight1->distance + flight2->distance + flight3->distance) {
                        distance_min = flight1->distance + flight2->distance + flight3->distance;
                        clean_flights_list(sorted_flights);
                        sorted_flights = realloc(sorted_flights, sizeof(Flight_list));
                    }
                    else if (distance_min == flight1->distance + flight2->distance + flight3->distance) {
                        Flight *new_flight = create_flight(flight1);
                        add_flight_to_list(sorted_flights, new_flight);   
                    }
                }
                flight3 = flight3->next;
            }
            flight2 = flight2->next;
        }
        flight1 = flight1->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {
        Flight *sorted_flight2 = flights_list->head;
        while(sorted_flight2 != NULL) {
            Flight *sorted_flight3 = flights_list->head;
            while(sorted_flight3 != NULL) {
                if (distance_min == sorted_flight->distance + sorted_flight2->distance + sorted_flight3->distance && 
                !strcmp(sorted_flight3->arrival_IATA, destiny) && check_3_flights(sorted_flight, sorted_flight2, sorted_flight3)) {
                    print_flight_information(sorted_flight);
                    printf("connects to ");
                    print_flight_information(sorted_flight2);
                    printf("connects to ");
                    print_flight_information(sorted_flight3);
                    printf("\n");
                }
                sorted_flight3 = sorted_flight3->next;
            }
            sorted_flight2 = sorted_flight2->next;
        }
        sorted_flight = sorted_flight->next;
    }
    free_flights_list(sorted_flights);
}