#include "structs.h"
#include "prototypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


Airport_list *airports_list = NULL;

void read_airports(FILE *airports){
    

}





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

    if (airports == NULL || routes == NULL) {
        printf("Error opening files\n");
        return 1;
    }
    
    switch (argc) {
        case 1:
            printf("No arguments\n");
            break;
        case 2:
            if (!strcmp(argv[1], "-voos")) { case_flights(); }
            else if (!strcmp(argv[1], "-aeroportos")) { case_airports(); }
            else { printf("Invalid argument\n"); }
            break;
        case 5:

            break;
        case 6:
            
            break;

        case 7:
            
            break;

        
        default:
            printf("Invalid number of arguments\n");
            break;
    }
}