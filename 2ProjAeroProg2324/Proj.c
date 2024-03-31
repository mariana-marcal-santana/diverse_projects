#include "support.h"
#include "read.h"
#include "show.h"
#include "sort.h"
#include "common.h"

Airport_list *airports_list = NULL;    //Initialize the lists to airport and flights
Flight_list *flights_list = NULL;

int main(int argc, char *argv[]) {

    FILE *airports, *routes;
    airports = fopen("aeroportos.txt", "r");    //Open the files with the information
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

    read_airports(airports,airports_list);   //Read the information from the files
    read_fligths(routes,flights_list);      //Read the information from the files   

    switch (argc) {  //Switch to choose the case to execute
        case 2:
            if (!strcmp(argv[1], "-voos")) { case_flights(flights_list,airports_list); } // 1
            else if (!strcmp(argv[1], "-aeroportos")) { case_airports(airports_list); } // 2
            else { printf("Invalid argument\n"); }
            break;
        case 5:
            char *endptr;
            int i = strtol(argv[4], &endptr, 10);
            if (endptr == argv[4]) {
                printf("No digits were found\n");
                return -1;
            }
            if (i == 0) { show_direct_flights(argv[1], argv[2], flights_list); } // 3
            else if (i == 1) { show_flights_1connection(argv[1], argv[2] ,flights_list); } // 5
            else if (i == 2) { show_flights_2connections(argv[1], argv[2], flights_list); } // case for 2 connections
            else { printf("Invalid argument\n"); }
            break;
        case 6:
            char *endptr2;
            int j = strtol(argv[4], &endptr2, 10);
            if (endptr2 == argv[4]) {
                printf("No digits were found\n");
                return -1;
            }
            if (j == 0) { show_direct_flights_sorted(argv[1], argv[2], argv[5], flights_list); } // 4
            else if (j == 1) { show_flights_1connection_sorted(argv[1], argv[2], argv[5], flights_list); } // 6
            else if (j == 2) { show_flights_2connections_sorted(argv[1], argv[2], argv[5], flights_list); } // case for 2 connections
            else { printf("Invalid argument\n"); }
            break;
        case 7:
            /*
            char *endptr3;
            int k = strtol(argv[4], &endptr3, 10);
            if (endptr3 == argv[4]) {
                printf("No digits were found\n");
                return -1;
            }
            if (k == 1) { show_flights_shortest_distance_1connection(argv[1], argv[2], argv[5], flights_list); }
            else if (k == 2) { show_flights_shortest_distance_2connections(argv[1], argv[2], argv[5], flights_list); }
            */
            break;
        default:
            printf("Invalid number of arguments\n");
            break;
    }
    fclose(airports);  //Close the files
    fclose(routes);
    free_system(airports_list, flights_list);  //Free the memory
}
