#include "read.h"

void read_airports(FILE *airports, Airport_list *airports_list){

    char line[200];
    while (fgets(line, sizeof(line), airports) != NULL){ //Read the information from the file until the end of the file
        
        Airport *airport = malloc(sizeof(Airport));
        airport->ICAO = malloc(5 * sizeof(char));       //Allocate memory for the variables to the airport
        airport->IATA = malloc(5 * sizeof(char));
        airport->city = malloc(50 * sizeof(char));
        airport->timezone = 0;
       
        sscanf(line,"%s %s %d %d %d %c %d %d %d %c %s %d", airport->ICAO, airport->IATA, 
        &airport->latitude.degrees, &airport->latitude.minutes, &airport->latitude.seconds, &airport->latitude.direction,
        &airport->longitude.degrees, &airport->longitude.minutes, &airport->longitude.seconds, &airport->longitude.direction, 
        airport->city, &airport->timezone); 
        
        if (check_data_airport(airport)){   //Check if the data is valid
            airport->next = NULL;
            airport->prev = NULL;
            
            if (airports_list->head == NULL) {
                airports_list->head = airport;      //Add the airport to the list
                airports_list->tail = airport;
            }
            else {
                airport->prev = airports_list->tail;
                airports_list->tail->next = airport;
                airports_list->tail = airport;
            }
        }else{
            printf("Invalid data in airport: %s\n", airport->IATA);
            free(airport->ICAO);
            free(airport->IATA);            //If the data is invalid, free the memory
            free(airport->city);
            free(airport);
        }
    }
}

void read_fligths(FILE *routes , Flight_list *flights_list){
    char line[200];
    char current_airline[10]; 

    while (fgets(line, sizeof(line), routes) != NULL) {  //Read the information from the file until the end of the file
        if (sscanf(line, "AIRLINE: %s", current_airline) == 1) { //Check if the line is the airline
            continue;   
        }
        else if (line[0] == '\n' || line[0] == '\0') {  //Check if the line is empty
            continue;
        } else {
            Flight *flight = malloc(sizeof(Flight));
            flight->airline = malloc(10 * sizeof(char));    //Allocate memory for the variables to the airport
            flight->flight_code = malloc(10 * sizeof(char));
            flight->depart_IATA= malloc(5 * sizeof(char));
            flight->arrival_IATA = malloc(5 * sizeof(char));
        
            sscanf(line, "%s %s %d:%d %s %d:%d", flight->flight_code, flight->depart_IATA,
                   &flight->depart_time_hour, &flight->depart_time_minute, flight->arrival_IATA,
                   &flight->arrival_time_hour, &flight->arrival_time_minute);

            strcpy(flight->airline, current_airline);
 
            if (check_data_flight(flight)){  //Check if the data is valid
                 
                flight->next = NULL;
                flight->prev = NULL;
                                                    //Add the flight to the list
                if (flights_list->head == NULL) {
                    flights_list->head = flight;
                    flights_list->tail = flight;
                } else {
                    flights_list->tail->next = flight;
                    flight->prev = flights_list->tail;
                    flights_list->tail = flight;
                }
            }
            else{                          //If the data is invalid, free the memory
                printf("Invalid data in flight: %s\n", flight->flight_code);
                free(flight->airline);
                free(flight->flight_code);
                free(flight->depart_IATA);
                free(flight->arrival_IATA);
                free(flight);
            }
        }
    }
}


// Check if the data in the flight is valid
bool check_data_flight(Flight * flight){
    if (flight->airline == NULL || flight->flight_code == NULL || flight->depart_IATA == NULL || flight->arrival_IATA == NULL){
        return false;
    }
    if (flight->depart_time_hour < 0 || flight->depart_time_hour > 23){
        return false;
    }
    if (flight->depart_time_minute < 0 || flight->depart_time_minute > 59){
        return false;
    }
    if (flight->arrival_time_hour < 0 || flight->arrival_time_hour > 23){
        return false;
    }
    if (flight->arrival_time_minute < 0 || flight->arrival_time_minute > 59){
        return false;
    }
    return true;
}

// Check if the data in the airport is valid
bool check_data_airport(Airport * airport){
    if (airport->ICAO == NULL || airport->IATA == NULL || airport->city == NULL){
        return false;
    }
    if (airport->latitude.degrees < -90 || airport->latitude.degrees > 90){
        return false;
    }
    if (airport->latitude.minutes < 0 || airport->latitude.minutes > 59){
        return false;
    }
    if (airport->latitude.seconds < 0 || airport->latitude.seconds > 59){
        return false;
    }
    if (airport->latitude.direction != 'N' && airport->latitude.direction != 'S'){
        return false;
    }
    if (airport->longitude.degrees < -180 || airport->longitude.degrees > 180){
        return false;
    }
    if (airport->longitude.minutes < 0 || airport->longitude.minutes > 59){
        return false;
    }
    if (airport->longitude.seconds < 0 || airport->longitude.seconds > 59){
        return false;
    }
    if (airport->longitude.direction != 'E' && airport->longitude.direction != 'W'){
        return false;
    }
    if (airport->timezone < -12 || airport->timezone > 14){
        return false;
    }
    return true;

}