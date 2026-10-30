//2026FA-ENGR-2304-3-GUFFEY-ADAM-P06

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int getNumber(char prompt[]);
int readFile(char filename[], int data[][9]);
void statistics(int data[][9], int count);
void report(int data[][9], int count);
void search(int data[][9], int count);

int main(void)
{
    int data[2000][9], count, choice;
    char filename[512];

    printf("Enter CSV or text filename (without quotes): ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return 0;
    filename[strcspn(filename, "\r\n")] = '\0';
    count = readFile(filename, data);
    if (count <= 0) {
        printf("No data loaded. Check the filename and file contents.\n");
        return 1;
    }
    printf("Loaded %d records.\n", count);
    do {
        printf("\n1. Plant data analysis\n2. Production report\n");
        printf("3. Search production data\n0. Exit\n");
        choice = getNumber("Select an option: ");
        switch (choice) {
            case 1: statistics(data, count); break;
            case 2: report(data, count); break;
            case 3: search(data, count); break;
            case 0: printf("Program ended.\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 0);
    return 0;
}

/* Read one integer and ask again if the input is invalid. */
int getNumber(char prompt[])
{
    char line[100], extra;
    int number;
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (sscanf(line, "%d %c", &number, &extra) == 1) return number;
        printf("Enter one whole number.\n");
    }
}

/* FILE * identifies the open file. fgets reads one line from that file. */
int readFile(char filename[], int data[][9])
{
    FILE *file = fopen(filename, "r");
    char line[512], extra;
    int count = 0, values;
    if (file == NULL) {
        printf("Could not open: %s\n", filename);
        return -1;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (count == 0 && strstr(line, "Year") != NULL) continue;
        if (strspn(line, " \t\r\n") == strlen(line)) continue;
        if (count == 2000) {
            printf("File exceeds the 2000-record limit.\n");
            fclose(file);
            return -1;
        }
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ',') line[i] = ' ';
        }
        values = sscanf(line, "%d %d %d %d %d %d %d %d %d %c",
                        &data[count][0], &data[count][1], &data[count][2],
                        &data[count][3], &data[count][4], &data[count][5],
                        &data[count][6], &data[count][7], &data[count][8], &extra);
        if (values != 9) {
            printf("Each record must contain nine integers.\n");
            fclose(file);
            return -1;
        }
        count++;
    }
    if (ferror(file)) count = -1;
    fclose(file);
    return count;
}

void statistics(int data[][9], int count)
{
    printf("\nPLANT DATA ANALYSIS: %d records per plant\n", count);
    printf("Population variance and standard deviation divide by N.\n");
    for (int column = 3; column < 9; column++) {
        int minimum = data[0][column], maximum = minimum;
        int mode = minimum, most = 0, frequency;
        double total = 0, mean, variance = 0, difference;
        for (int i = 0; i < count; i++) {
            total += data[i][column];
            if (data[i][column] < minimum) minimum = data[i][column];
            if (data[i][column] > maximum) maximum = data[i][column];
            /* Count how often this value appears in this plant's column. */
            frequency = 0;
            for (int j = 0; j < count; j++) {
                if (data[j][column] == data[i][column]) frequency++;
            }
            if (frequency > most) {
                most = frequency;
                mode = data[i][column];
            }
        }
        mean = total / count;
        for (int i = 0; i < count; i++) {
            difference = data[i][column] - mean;
            variance += difference * difference;
        }
        variance /= count;
        printf("\nL%d: Total = %.0f units\n", column - 2, total);
        printf("Min = %d, Max = %d, Range = %.0f\n",
               minimum, maximum, (double)maximum - minimum);
        printf("Mean = %.3f\n", mean);
        if (most == 1) printf("Mode: none (no value repeats)\n");
        else printf("Mode = %d (appears %d times)\n", mode, most);
        printf("Population variance = %.3f\n", variance);
        printf("Population standard deviation = %.3f\n", sqrt(variance));
    }
}

void report(int data[][9], int count)
{
    double totals[6] = {0}, daily, bestDaily = 0;
    int best = 0, worst = 0, bestDate = 0;
    int peak = data[0][3], peakPlant = 0, peakDate = 0;
    for (int plant = 0; plant < 6; plant++) bestDaily += data[0][plant + 3];
    for (int i = 0; i < count; i++) {
        daily = 0;
        for (int plant = 0; plant < 6; plant++) {
            totals[plant] += data[i][plant + 3];
            daily += data[i][plant + 3];
            if (data[i][plant + 3] > peak) {
                peak = data[i][plant + 3];
                peakPlant = plant;
                peakDate = i;
            }
        }
        if (daily > bestDaily) {
            bestDaily = daily;
            bestDate = i;
        }
    }
    printf("\nPRODUCTION REPORT\nOverall performance uses total units.\n");
    printf("For ties, this program shows the first result encountered.\n");
    for (int plant = 0; plant < 6; plant++) {
        printf("L%d total: %.0f\n", plant + 1, totals[plant]);
        if (totals[plant] > totals[best]) best = plant;
        if (totals[plant] < totals[worst]) worst = plant;
    }
    printf("Best plant: L%d (%.0f units)\n", best + 1, totals[best]);
    printf("Worst plant: L%d (%.0f units)\n", worst + 1, totals[worst]);
    printf("Best single plant day: L%d, %d units on %04d-%02d-%02d\n",
           peakPlant + 1, peak, data[peakDate][0], data[peakDate][1], data[peakDate][2]);
    printf("Best combined day: %.0f units on %04d-%02d-%02d\n",
           bestDaily, data[bestDate][0], data[bestDate][1], data[bestDate][2]);
}

void search(int data[][9], int count)
{
    int plant, year, month, day, found = 0;
    plant = getNumber("Plant (1-6), or 0 for all plants: ");
    if (plant < 0 || plant > 6) {
        printf("Invalid plant number.\n");
        return;
    }
    year = getNumber("Year: ");
    month = getNumber("Month: ");
    day = getNumber("Day: ");
    for (int i = 0; i < count; i++) {
        if (data[i][0] == year && data[i][1] == month && data[i][2] == day) {
            found = 1;
            printf("%04d-%02d-%02d\n", year, month, day);
            for (int p = 1; p <= 6; p++) {
                if (plant == 0 || plant == p)
                    printf("L%d: %d units\n", p, data[i][p + 2]);
            }
        }
    }
    if (found == 0) printf("No production record found for that date.\n");
}