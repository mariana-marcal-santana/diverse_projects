#include "read.h"

void read_airports(FILE *airports, Airport_list *airports_list){

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

void read_fligths(FILE *routes , Flight_list *flights_list){
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