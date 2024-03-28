#include "structs.h"
#include "prototypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

Airport_list *airports_list = NULL;
Flight_list *flights_list = NULL;

int main(int argc, char *argv[]) {

    FILE *airports, *routes;
    airports = fopen("aeroportos.txt", "r");
    routes = fopen("rotas.txt", "r");
    if (airports == NULL || routes == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    if (airports_list == NULL) {
        airports_list = malloc(sizeof(Airport_list));
        airports_list->head = NULL;
        airports_list->tail = NULL;
    }
    if (flights_list == NULL) {
        flights_list = malloc(sizeof(Flight_list));
        flights_list->head = NULL;
        flights_list->tail = NULL;
    }

    read_airports(airports);
    read_fligths(routes);

    switch (argc) {
        case 2:
            if (!strcmp(argv[1], "-voos")) { case_flights(); } // 1
            else if (!strcmp(argv[1], "-aeroportos")) { case_airports(); } // 2
            else { printf("Invalid argument\n"); }
            break;
        case 5:
            int i = atoi(argv[4]);
            if (i == 0) { show_direct_flights(argv[1], argv[2]); } // 3
            else if (i == 1) { show_flights_1connection(argv[1], argv[2]); } // 5
            else if (i == 2) { show_flights_2connections(argv[1], argv[2]); } // case for 2 connections
            else { printf("Invalid argument\n"); }
            break;
        case 6:
            int j = atoi(argv[4]);
            if (j == 0) { show_direct_flights_sorted(argv[1], argv[2], argv[5]); } // 4
            else if (j == 1) { show_flights_1connection_sorted(argv[1], argv[2], argv[5]); } // 6
            else if (j == 2) { show_flights_2connections_sorted(argv[1], argv[2], argv[5]); } // case for 2 connections
            else { printf("Invalid argument\n"); }
            break;
        case 7:
            int k = atoi(argv[4]);
            if (k == 1) { show_flights_shortest_distance_1connection(argv[1], argv[2], argv[5]); }
            else if (k == 2) { show_flights_shortest_distance_2connections(argv[1], argv[2], argv[5]); }
            break;
        default:
            printf("Invalid number of arguments\n");
            break;
    }
    free_system();
}

void read_airports(FILE *airports){

    char line[200];
    while (fgets(line, sizeof(line), airports) != NULL){
        
        Airport *airport = malloc(sizeof(Airport));
        airport->ICAO = malloc(5 * sizeof(char));
        airport->IATA = malloc(5 * sizeof(char));
        airport->city = malloc(50 * sizeof(char));
        airport->timezone = 0;
       
        sscanf(line,"%s %s %d %d %d %c %d %d %d %c %s %d", airport->ICAO, airport->IATA, 
        &airport->latitude.degrees, &airport->latitude.minutes, &airport->latitude.seconds, &airport->latitude.direction,
        &airport->longitude.degrees, &airport->longitude.minutes, &airport->longitude.seconds, &airport->longitude.direction, 
        airport->city, &airport->timezone); 
        
        airport->next = NULL;
        airport->prev = NULL;
        
        if (airports_list->head == NULL) {
            airports_list->head = airport;
            airports_list->tail = airport;
        }else {
            airport->prev = airports_list->tail;
            airports_list->tail->next = airport;
            airports_list->tail = airport;
        }
    }
}

void read_fligths(FILE *routes) {
    char line[200];
    char current_airline[10]; 

    while (fgets(line, sizeof(line), routes) != NULL) {
        if (sscanf(line, "AIRLINE: %s", current_airline) == 1) {
            continue;
        }
        else if (line[0] == '\n' || line[0] == '\0') {
            continue;
        } else {
            Flight *flight = malloc(sizeof(Flight));
            flight->airline = malloc(10 * sizeof(char));
            flight->flight_code = malloc(10 * sizeof(char));
            flight->depart_IATA= malloc(5 * sizeof(char));
            flight->arrival_IATA = malloc(5 * sizeof(char));
        
            sscanf(line, "%s %s %d:%d %s %d:%d", flight->flight_code, flight->depart_IATA,
                   &flight->depart_time_hour, &flight->depart_time_minute, flight->arrival_IATA,
                   &flight->arrival_time_hour, &flight->arrival_time_minute);

            strcpy(flight->airline, current_airline);

            flight->next = NULL;
            flight->prev = NULL;

            if (flights_list->head == NULL) {
                flights_list->head = flight;
                flights_list->tail = flight;
            } else {
                flights_list->tail->next = flight;
                flight->prev = flights_list->tail;
                flights_list->tail = flight;
            }
        }
    }
}

void case_flights() {
    Flight *flight = flights_list->head;
    
    while (flight != NULL) {
        
        double distance = 0 ;
        distance = calc_airport_distance(flight->depart_IATA, flight->arrival_IATA);
        print_flight_information(flight);
        printf("Distance:%f\n", distance);
        
        flight = flight->next;
    }   
}

void case_airports() {
    Airport *airport = airports_list->head;
    
    while (airport != NULL) {
        LatLong *latitude = &airport->latitude;
        LatLong *longitude = &airport->longitude;
        
        printf("%s %s ", airport->ICAO, airport->IATA);
        printf("%d %d %d %c ", latitude->degrees, latitude->minutes, latitude->seconds, latitude->direction);
        printf("%d %d %d %c ", longitude->degrees, longitude->minutes, longitude->seconds, longitude->direction);
        printf("%s ", airport->city);
        printf("%d\n", airport->timezone);
        
        airport = airport->next;
    }
}

void show_direct_flights(char *origin, char *destiny) {
    Flight *flight = flights_list->head;
    
    while (flight != NULL) {
        if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight->arrival_IATA, destiny)) {
            print_flight_information(flight);
            printf("\n");
        }
        flight = flight->next;
    }
}

void show_flights_1connection(char *origin, char *destiny) {

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

void show_flights_2connections(char *origin, char *destiny) {

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

void show_direct_flights_sorted(char *origin, char *destiny, char *sort_type) {

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

void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type) {
    
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

void show_flights_2connections_sorted(char *origin, char *destiny, char *sort_type) {

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

void show_flights_shortest_distance_1connection(char *origin, char *destiny, char *sort_type) {

}

void show_flights_shortest_distance_2connections(char *origin, char *destiny, char *sort_type) {

}

double calc_airport_distance(char *airportA_IATA, char *airportB_IATA) {

    int R = 6371 + 10;
    double latA = 0, lonA = 0, latB = 0, lonB = 0, flagA = 0, flagB = 0;
    Airport *airport = airports_list->head;

    while (airport != NULL) {
        if (!strcmp(airport->IATA, airportA_IATA)) { 
            latA = (double)airport->latitude.degrees + (double)airport->latitude.minutes/60 +
                (double)airport->latitude.seconds/3600;
            lonA = (double)airport->longitude.degrees + (double)airport->longitude.minutes/60 + 
                (double)airport->longitude.seconds/3600;
            flagA = 1;
        }
        if (!strcmp(airport->IATA, airportB_IATA)) { 
            latB = (double)airport->latitude.degrees + (double)airport->latitude.minutes/60 + 
                (double)airport->latitude.seconds/3600;
            lonB = (double)airport->longitude.degrees + (double)airport->longitude.minutes/60 + 
                (double)airport->longitude.seconds/3600;
            flagB = 1;
        }
        
        airport = airport->next;

        if (flagA && flagB) { break; }
    }

    double xA = R * cos(latA) * cos(lonA);
    double yA = R * cos(latA) * sin(lonA);
    double zA = R * sin(latA);

    double xB = R * cos(latB) * cos(lonB);
    double yB = R * cos(latB) * sin(lonB);
    double zB = R * sin(latB);

    double modA = sqrt(xA*xA + yA*yA + zA*zA);
    double modB = sqrt(xB*xB + yB*yB + zB*zB);

    double int_product = xA * xB + yA * yB + zA * zB;
    
    double cos_theta = int_product / (modA * modB);
    double theta = acos(cos_theta);

    return R * theta;
}

void print_flight_information(Flight *flight) {
    printf("%s %s ", flight->flight_code, flight->depart_IATA);
    printf("%.2d:%.2d ", flight->depart_time_hour, flight->depart_time_minute);
    printf("%s ", flight->arrival_IATA);
    printf("%.2d:%.2d ", flight->arrival_time_hour, flight->arrival_time_minute);
}

void free_system() {

}