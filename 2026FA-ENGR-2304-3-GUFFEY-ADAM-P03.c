#include <stdio.h>

double determinant2x2(double a, double b, double c, double d)
{
    return (a * d) - (b * c);
}

void solve2x2(void)
{
    double a1, b1, c1;
    double a2, b2, c2;

    double D, Dx, Dy;
    double x, y;

    printf("\n--- 2x2 Cramer's Rule ---\n");

    printf("Enter a1, b1, and c1 for a1*x + b1*y = c1: ");
    scanf("%lf %lf %lf", &a1, &b1, &c1);

    printf("Enter a2, b2, and c2 for a2*x + b2*y = c2: ");
    scanf("%lf %lf %lf", &a2, &b2, &c2);

    D = determinant2x2(a1, b1, a2, b2);

    if (D == 0)
    {
        printf("\nThe determinant is zero.\n");
        printf("There is no unique solution using Cramer's Rule.\n");
        return;
    }

    /* Replace the x-column with the constants */
    Dx = determinant2x2(c1, b1, c2, b2);

    /* Replace the y-column with the constants */
    Dy = determinant2x2(a1, c1, a2, c2);

    x = Dx / D;
    y = Dy / D;

    printf("\nD  = %.2f\n", D);
    printf("Dx = %.2f\n", Dx);
    printf("Dy = %.2f\n", Dy);

    printf("\nx = %.2f\n", x);
    printf("y = %.2f\n", y);
}


/* Find a cofactor of a 3x3 matrix */
double cofactor(double matrix[3][3], int row, int column)
{
    double minor[2][2];
    int i, j;
    int minorRow = 0;
    int minorColumn;

    /* Create the 2x2 minor by removing the selected row and column */
    for (i = 0; i < 3; i++)
    {
        if (i == row)
            continue;

        minorColumn = 0;

        for (j = 0; j < 3; j++)
        {
            if (j == column)
                continue;

            minor[minorRow][minorColumn] = matrix[i][j];
            minorColumn++;
        }

        minorRow++;
    }

    /* Find the determinant of the 2x2 minor */
    double result = determinant2x2(
        minor[0][0], minor[0][1],
        minor[1][0], minor[1][1]
    );

    /* Cofactor signs follow + - + / - + - / + - + */
    if ((row + column) % 2 != 0)
    {
        result = -result;
    }

    return result;
}


/* Find the determinant of a 3x3 matrix using cofactors */
double determinant3x3(double matrix[3][3])
{
    double D;

    D = matrix[0][0] * cofactor(matrix, 0, 0)
      + matrix[0][1] * cofactor(matrix, 0, 1)
      + matrix[0][2] * cofactor(matrix, 0, 2);

    return D;
}


/* Copy one 3x3 matrix into another */
void copyMatrix(double original[3][3], double copy[3][3])
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            copy[i][j] = original[i][j];
        }
    }
}


/* Replace one column with the constants */
void replaceColumn(double matrix[3][3], double constants[3], int column)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        matrix[i][column] = constants[i];
    }
}


/* Solve a system of 3 equations */
void solve3x3(void)
{
    double A[3][3];

    double Ax[3][3];
    double Ay[3][3];
    double Az[3][3];

    double constants[3];

    double D, Dx, Dy, Dz;
    double x, y, z;

    int i;

    printf("\n--- 3x3 Cramer's Rule ---\n");
    printf("Enter each equation as: a b c d\n");
    printf("for a*x + b*y + c*z = d\n\n");

    for (i = 0; i < 3; i++)
    {
        printf("Equation %d: ", i + 1);
        scanf("%lf %lf %lf %lf",
              &A[i][0],
              &A[i][1],
              &A[i][2],
              &constants[i]);
    }

    /* Original determinant */
    D = determinant3x3(A);

    if (D == 0)
    {
        printf("\nThe determinant is zero.\n");
        printf("There is no unique solution using Cramer's Rule.\n");
        return;
    }

    /*
       Cramer's Rule:
       x = Dx / D
       y = Dy / D
       z = Dz / D
    */

    copyMatrix(A, Ax);
    copyMatrix(A, Ay);
    copyMatrix(A, Az);

    /* Replace x, y, and z columns with the constants */
    replaceColumn(Ax, constants, 0);
    replaceColumn(Ay, constants, 1);
    replaceColumn(Az, constants, 2);

    /* Find Dx, Dy, and Dz */
    Dx = determinant3x3(Ax);
    Dy = determinant3x3(Ay);
    Dz = determinant3x3(Az);

    /* Solve for x, y, and z */
    x = Dx / D;
    y = Dy / D;
    z = Dz / D;

    printf("\nD  = %.2f\n", D);
    printf("Dx = %.2f\n", Dx);
    printf("Dy = %.2f\n", Dy);
    printf("Dz = %.2f\n", Dz);

    printf("\nSolution:\n");
    printf("x = %.2f\n", x);
    printf("y = %.2f\n", y);
    printf("z = %.2f\n", z);

    /* Display the cofactors used for D */
    printf("\nCofactors from the first row:\n");
    printf("C11 = %.2f\n", cofactor(A, 0, 0));
    printf("C12 = %.2f\n", cofactor(A, 0, 1));
    printf("C13 = %.2f\n", cofactor(A, 0, 2));
}


int main(void)
{
    int choice;

    printf("=================================\n");
    printf("       CRAMER'S RULE SOLVER\n");
    printf("=================================\n");

    printf("\n1. Solve 2 equations\n");
    printf("2. Solve 3 equations\n");
    printf("3. Exit\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            solve2x2();
            break;

        case 2:
            solve3x3();
            break;

        case 3:
            printf("\nGoodbye.\n");
            break;

        default:
            printf("\nInvalid choice.\n");
    }

    return 0;
}
