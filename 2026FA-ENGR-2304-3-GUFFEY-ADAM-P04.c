//2026FA-ENGR-2304-3-GUFFEY-ADAM-P04

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int Vs, R1, R2, R3;
    int choice;

    double i1, i2;
    double v1, v2, v3;
    double D;

    double user_i1, user_i2;
    double user_v1, user_v2, user_v3;

    double correct_i1, correct_i2;
    double correct_v1, correct_v2, correct_v3;

    srand(time(NULL));

    do
    {
        Vs = rand() % 10 + 1;
        R1 = rand() % 20 + 1;
        R2 = rand() % 20 + 1;
        R3 = rand() % 20 + 1;

        D = (R1 + R2) * (R2 + R3) - (R2 * R2);

        i1 = Vs * (R2 + R3) / D;
        i2 = Vs * R2 / D;

        v1 = i1 * R1;
        v2 = (i1 - i2) * R2;
        v3 = i2 * R3;

        correct_i1 = (int)(i1 * 1000) / 1000.0;
        correct_i2 = (int)(i2 * 1000) / 1000.0;

        correct_v1 = (int)(v1 * 1000) / 1000.0;
        correct_v2 = (int)(v2 * 1000) / 1000.0;
        correct_v3 = (int)(v3 * 1000) / 1000.0;

        printf("\n-----------------------------\n");
        printf("Electrical Circuit Generator\n");
        printf("-----------------------------\n");

        printf("Vs = %d V\n", Vs);
        printf("R1 = %d Ohms\n", R1);
        printf("R2 = %d Ohms\n", R2);
        printf("R3 = %d Ohms\n", R3);

        printf("\nEnter i1: ");
        scanf("%lf", &user_i1);

        printf("Enter i2: ");
        scanf("%lf", &user_i2);

        printf("Enter v1: ");
        scanf("%lf", &user_v1);

        printf("Enter v2: ");
        scanf("%lf", &user_v2);

        printf("Enter v3: ");
        scanf("%lf", &user_v3);


        printf("\nResults:\n");

        if (user_i1 == correct_i1)
            printf("i1: Correct!\n");
        else
            printf("i1: Incorrect. Correct answer = %.3f\n", correct_i1);

        if (user_i2 == correct_i2)
            printf("i2: Correct!\n");
        else
            printf("i2: Incorrect. Correct answer = %.3f\n", correct_i2);

        if (user_v1 == correct_v1)
            printf("v1: Correct!\n");
        else
            printf("v1: Incorrect. Correct answer = %.3f\n", correct_v1);

        if (user_v2 == correct_v2)
            printf("v2: Correct!\n");
        else
            printf("v2: Incorrect. Correct answer = %.3f\n", correct_v2);

        if (user_v3 == correct_v3)
            printf("v3: Correct!\n");
        else
            printf("v3: Incorrect. Correct answer = %.3f\n", correct_v3);

        /* Ask whether to continue */

        printf("\nEnter 1 to generate another circuit.\n");
        printf("Enter 0 to exit.\n");
        scanf("%d", &choice);

    } while (choice != 0);

    printf("\nProgram ended.\n");

    return 0;
}