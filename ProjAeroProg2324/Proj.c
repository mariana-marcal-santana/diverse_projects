// Proj Aero Intro Prog 2023/24
#include <stdio.h>
#include <string.h>

/* Function to calculate the maximum value of y */
int calcMaxY(float N, float a, float b, float kmax, FILE *file) {
    int index, i, size = kmax;
    // Initialize the arrays and first elements
    float max = 0, x[size], y[size], z[size];
    x[0] = (N - 1)/N;
    y[0] = 1/N;
    z[0] = 0;
    for (i = 0; i < size; i++) {
        // Calculate the next values
        x[i + 1] = x[i] - a*x[i]*y[i];
        y[i + 1] = y[i] + a*x[i]*y[i] - b*y[i];
        z[i + 1] = z[i] + b*y[i];
        // Check for max value and index
        if (y[i] > max) {
            max = y[i];
            index = i;
        }
    }
    // Print the results
    fprintf(file, " OK %d %.4f %.4f %.4f\n", index, x[index], y[index], z[index]);
    return 0;
}

/* Main function for parsing of file with parameters */
int main() {
    // Create and open files
    FILE *file1, *file2;
    file1 = fopen("parametros.txt", "r");
    file2 = fopen("resultadosGnnn.txt", "w");
    // Check if files were opened correctly
    if (file1 == NULL || file2 == NULL) {
        printf("Couldn't open file(s)\n");
        return 1;
    }
    char line[1000];
    float N, a, b, kmax;
    // Read until the end of the file
    while (fgets(line, sizeof(line), file1) != NULL) {
        // Remove the newline character from the string and print it
        line[strcspn(line, "\n")] = '\0';
        fprintf(file2, "%s", line);
        // Check if the line has the correct number of arguments
        if (sscanf(line, "%f %f %f %f", &N, &a, &b, &kmax) != 4) { 
            fprintf(file2, "ERRO\n"); 
            break;
        }
        // Check if the arguments are valid
        if (N <= 0 || a <= 0 || b <= 0 || kmax <= 0) { fprintf(file2, " ERRO\n"); }
        else { calcMaxY(N, a, b, kmax, file2); }
    }
    // Close the files
    fclose(file1);
    fclose(file2);
    return 0;
}
