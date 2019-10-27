#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    double coordX[10000];
    double coordY[10000];
    float tempX, tempY;
    char x[1000], *y, countChar, filename[1000];
    FILE *fptr, *emptyfptr, *yCoordfptr;
    int count = 0;

    //declare the delim
    const char delim[2] = ",";

    printf("\nPlease enter file name:");
    scanf("%s", &filename);
    fptr = fopen(filename, "r");

    // file path of Mag's desktop - /Users/magdalene/Desktop/SIT-UofG/programMeth/progMeth_assg1/Group9_15.txt
    if (fptr == NULL)
    {
        printf("Error! opening file\n");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    //loop while countChar is not at the End of File
    while (countChar != EOF)
    {
        //Count whenever new line is encountered
        if (countChar == '\n')
        {
            count = count + 1;
        }

        //get the x coordinates adn store in x
        fscanf(fptr,"%10000s[^\n]", x);

        //get the first token (getting the y coordinates)
        y = strtok(x, delim);
        
        // walk through other tokens while the token is null
        while( y != NULL ) {
            //printf( " %s\n", token );
            coordY[count] = atof(y); //store token to the coordY for every count
            y = strtok(NULL, delim); //make the token null after storing
        }

        //store the x coorindates in array coordX
        coordX[count] = atof(x);

        //take next character from file.
        countChar = getc(fptr);
    }

    fclose(fptr); //close file.
    // printf("There are %d lines \n", count);
    //end of pulling coordinates from file



    //linear regregession calculation
    float sumX = 0.0, sumY = 0.0, xMean, yMean, numer, denom, b0, b1;
    float  xMeandiff[5], yMeandiff[5];
    int n = sizeof(coordY)/sizeof(coordY[0]);

    for (int i = 0; i < n; i++)
    {
        //sum all x and Y
        sumX = sumX + coordX[i];
        sumY = sumY + coordY[i];
    }

    //calculate the mean of x and y
    xMean = sumX / n;
    yMean = sumY / n;

    for (int i = 0; i < n; i++)
    {
        //calculate x-xMean and y-yMean
        xMeandiff[i] = coordX[i] - xMean;
        yMeandiff[i] = coordY[i] - yMean;

        //calculate the denominator of equation (x - xMean)^2
        denom += pow(xMeandiff[i], 2);

        //calculate the numeration (x-xMean)*(y-yMean)
        numer += xMeandiff[i] * yMeandiff[i];
    }

    //calculate b0 and b1
    b1 = numer / denom;
    b0 = yMean - (b1*xMean);

    printf("y = %0.2f + %0.2fx\n", b0, b1);
    printf("sum of x and y: %0.2f and %0.2f\n", sumX, sumY);
    printf("Mean of x and y: %0.2f and %0.2f\n", xMean, yMean);
   
    return 0;
}
