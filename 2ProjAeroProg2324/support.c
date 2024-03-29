
#include "support.h"

void case_flights(Flight_list * flights_list,Airport_list *airports_list){
    Flight *flight = flights_list->head;
    
    while (flight != NULL) {
        
        double distance = 0 ;
        distance = calc_airport_distance(flight->depart_IATA, flight->arrival_IATA, airports_list);
        print_flight_information(flight);
        printf("Distance:%f\n", distance);
        
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

double calc_airport_distance(char *airportA_IATA, char *airportB_IATA,Airport_list *airports_list) {

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