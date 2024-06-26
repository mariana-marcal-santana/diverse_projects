#include "support.h"

void case_flights(Flight_list * flights_list,Airport_list *airports_list){
   
    Flight *flight = flights_list->head;
    while (flight != NULL) {
        
        double distance = 0 ;
        distance = calc_airport_distance(flight->depart_IATA, flight->arrival_IATA, airports_list);
       
        print_flight_information(flight);
        if (distance == -1) {
            printf("Distance: N/A (Some airport not found)\n");
        } else {
            printf("Distance: %.9f\n", distance);
        }   
        
        flight = flight->next;
    }   
}

void case_airports(Airport_list *airports_list ) {
    
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

double calc_airport_distance(char *airportA_IATA, char *airportB_IATA, Airport_list *airports_list) {

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

    if (!flagA || !flagB) { return -1; }

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

void free_system(Airport_list *airport_list, Flight_list *flight_list) {
    free_airports_list(airport_list);
    free_flights_list(flight_list);
}

void free_airports_list(Airport_list *airport_list) {
    Airport *airport = airport_list->head;
    Airport *next_airport = NULL;
    
    while (airport != NULL) {
        next_airport = airport->next;
        free(airport->ICAO);
        free(airport->IATA);
        free(airport->city);
        free(airport);
        airport = next_airport;
    }
    free(airport_list);
}

void free_flights_list(Flight_list *flight_list) {
    Flight *flight = flight_list->head;
    Flight *next_flight = NULL;
    
    while (flight != NULL) {
        next_flight = flight->next;
        free(flight->airline);
        free(flight->flight_code);
        free(flight->depart_IATA);
        free(flight->arrival_IATA);
        free(flight);
        flight = next_flight;
    }
    free(flight_list);
}

void clean_flights_list(Flight_list *flight_list) {
    Flight *flight = flight_list->head;
    Flight *next_flight = NULL;
    
    while (flight != NULL) {
        next_flight = flight->next;
        free(flight->airline);
        free(flight->flight_code);
        free(flight->depart_IATA);
        free(flight->arrival_IATA);
        free(flight);
        flight = next_flight;
    }
    flight_list->head = NULL;
    flight_list->tail = NULL;
}

Flight *create_flight(Flight *flight) {
    Flight *new_flight = malloc(sizeof(Flight));
    new_flight->airline = malloc(10 * sizeof(char));
    new_flight->flight_code = malloc(10 * sizeof(char));
    new_flight->depart_IATA = malloc(5 * sizeof(char));
    new_flight->arrival_IATA = malloc(5 * sizeof(char));
    strcpy(new_flight->airline, flight->airline);
    strcpy(new_flight->flight_code, flight->flight_code);
    strcpy(new_flight->depart_IATA, flight->depart_IATA);
    strcpy(new_flight->arrival_IATA, flight->arrival_IATA);
    new_flight->depart_time_hour = flight->depart_time_hour;
    new_flight->depart_time_minute = flight->depart_time_minute;
    new_flight->arrival_time_hour = flight->arrival_time_hour;
    new_flight->arrival_time_minute = flight->arrival_time_minute;
    new_flight->distance = flight->distance;
    new_flight->next = NULL;
    new_flight->prev = NULL;
    return new_flight;
}

void add_flight_to_list(Flight_list *flight_list, Flight *flight) {
    if (flight_list->head == NULL) {
        flight_list->head = flight;
        flight_list->tail = flight;
    } else {
        flight_list->tail->next = flight;
        flight->prev = flight_list->tail;
        flight_list->tail = flight;
    }
}

bool check_3_flights(Flight *flight1, Flight *flight2, Flight *flight3) {
    return !strcmp(flight1->arrival_IATA, flight2->depart_IATA) &&
        !strcmp(flight2->arrival_IATA, flight3->depart_IATA) &&
        (flight1->arrival_time_hour < flight2->depart_time_hour ||
        (flight1->arrival_time_hour == flight2->depart_time_hour &&
        flight1->arrival_time_minute < flight2->depart_time_minute)) &&
        (flight2->arrival_time_hour < flight3->depart_time_hour ||
        (flight2->arrival_time_hour == flight3->depart_time_hour &&
        flight2->arrival_time_minute < flight3->depart_time_minute));
}