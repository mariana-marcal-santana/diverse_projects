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
        case 1:
            printf("No arguments\n");
            break;
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
            else if (j == 2) { printf("show_flights_2connections_sorted\n"); }
            else { printf("Invalid argument\n"); }
            break;
        case 7:
            
            break;
        default:
            printf("Invalid number of arguments\n");
            break;
    }
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
            airports_list->head->prev = airport;
            airport->next = airports_list->head;
            airports_list->head = airport;
        }
    }
}

void read_fligths(FILE *routes) {
    char line[200];
    char current_airline[10]; 

    while (fgets(line, sizeof(line), routes) != NULL) {
        if (sscanf(line, "AIRLINE: %s", current_airline) == 1) {
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
                flights_list->head->prev = flight;
                flight->next = flights_list->head;
                flights_list->head = flight;
            }
        }
    }
}

void case_flights() {
    Flight *flight = flights_list->head;
    
    while (flight != NULL) {
        
        double distance = calc_airport_distance(flight->depart_IATA, flight->arrival_IATA);
        printf("%s %s ", flight->flight_code, flight->depart_IATA);
        printf("%.2d:%.2d ", flight->depart_time_hour, flight->depart_time_minute);
        printf("%s ", flight->arrival_IATA);
        printf("%.2d:%.2d ", flight->arrival_time_hour, flight->arrival_time_minute);
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
            printf("%s %s ", flight->flight_code, flight->depart_IATA);
            printf("%.2d:%.2d ", flight->depart_time_hour, flight->depart_time_minute);
            printf("%s ", flight->arrival_IATA);
            printf("%.2d:%.2d\n", flight->arrival_time_hour, flight->arrival_time_minute);
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
        printf("%s %s ", sorted_flight->flight_code, sorted_flight->depart_IATA);
        printf("%.2d:%.2d ", sorted_flight->depart_time_hour, sorted_flight->depart_time_minute);
        printf("%s ", sorted_flight->arrival_IATA);
        printf("%.2d:%.2d\n", sorted_flight->arrival_time_hour, sorted_flight->arrival_time_minute);
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

void show_flights_1connection(char *origin, char *destiny) {

    Flight *flight = flights_list->head;
    while (flight != NULL) {

        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {

            if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight2->arrival_IATA, destiny) &&
            !strcmp(flight->arrival_IATA, flight2->depart_IATA)) {

                printf("%s %s ", flight->flight_code, flight->depart_IATA);
                printf("%.2d:%.2d ", flight->depart_time_hour, flight->depart_time_minute);
                printf("%s ", flight->arrival_IATA);
                printf("%.2d:%.2d ", flight->arrival_time_hour, flight->arrival_time_minute);
                printf("connects to ");
                printf("%s %s ", flight2->flight_code, flight2->depart_IATA);
                printf("%.2d:%.2d ", flight2->depart_time_hour, flight2->depart_time_minute);
                printf("%s ", flight2->arrival_IATA);
                printf("%.2d:%.2d\n", flight2->arrival_time_hour, flight2->arrival_time_minute);
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
                    
                    printf("%s %s ", flight->flight_code, flight->depart_IATA);
                    printf("%.2d:%.2d ", flight->depart_time_hour, flight->depart_time_minute);
                    printf("%s ", flight->arrival_IATA);
                    printf("%.2d:%.2d ", flight->arrival_time_hour, flight->arrival_time_minute);
                    printf("connects to ");
                    printf("%s %s ", flight2->flight_code, flight2->depart_IATA);
                    printf("%.2d:%.2d ", flight2->depart_time_hour, flight2->depart_time_minute);
                    printf("%s ", flight2->arrival_IATA);
                    printf("%.2d:%.2d ", flight2->arrival_time_hour, flight2->arrival_time_minute);
                    printf("connects to ");
                    printf("%s %s ", flight3->flight_code, flight3->depart_IATA);
                    printf("%.2d:%.2d ", flight3->depart_time_hour, flight3->depart_time_minute);
                    printf("%s ", flight3->arrival_IATA);
                    printf("%.2d:%.2d\n", flight3->arrival_time_hour, flight3->arrival_time_minute);
                }
                flight3 = flight3->next;
            }
            flight2 = flight2->next;
        }
        flight = flight->next;
    }
    
}

void show_flights_1connection_sorted(char *origin, char *destiny, char *sort_type) {
    
    Flight_list *sorted_flights = malloc(sizeof(Flight_list));
    sorted_flights->head = NULL;
    sorted_flights->tail = NULL;
    
    Flight *flight = flights_list->head;
    while (flight != NULL) {

        Flight *flight2 = flights_list->head;
        while (flight2 != NULL) {

            if (!strcmp(flight->depart_IATA, origin) && !strcmp(flight2->arrival_IATA, destiny) &&
            !strcmp(flight->arrival_IATA, flight2->depart_IATA) && (flight->arrival_time_hour < flight2->depart_time_hour ||
            (flight->arrival_time_hour == flight2->depart_time_hour && flight->arrival_time_minute < flight2->depart_time_minute))) {

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
            flight2 = flight2->next;
        }
        flight = flight->next;
    }

    if (!strcmp(sort_type, "-TC")) { sort_flights_ascending(sorted_flights); }
    else if (!strcmp(sort_type, "-TD")) { sort_flights_descending(sorted_flights); }

    Flight *sorted_flight = sorted_flights->head;
    while (sorted_flight != NULL) {
        printf("%s %s ", sorted_flight->flight_code, sorted_flight->depart_IATA);
        printf("%.2d:%.2d ", sorted_flight->depart_time_hour, sorted_flight->depart_time_minute);
    }
}

double calc_airport_distance(char *airportA_IATA, char *airportB_IATA) {

    int R = 6371 + 10;
    Airport *airportA = NULL, *airportB = NULL, *airport = airports_list->head;

    while (airport != NULL) {
        if (!strcmp(airport->IATA, airportA_IATA)) { airportA = airport; }
        if (!strcmp(airport->IATA, airportB_IATA)) { airportB = airport; }

        airport = airport->next;
        
        if (airportA != NULL && airportB != NULL) { break; }
    }

    double latA = (double)airportA->latitude.degrees + (double)airportA->latitude.minutes/60 + (double)airportA->latitude.seconds/3600;
    double lonA = (double)airportA->longitude.degrees + (double)airportA->longitude.minutes/60 + (double)airportA->longitude.seconds/3600;
    double latB = (double)airportB->latitude.degrees + (double)airportB->latitude.minutes/60 + (double)airportB->latitude.seconds/3600;
    double lonB = (double)airportB->longitude.degrees + (double)airportB->longitude.minutes/60 + (double)airportB->longitude.seconds/3600;
    
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