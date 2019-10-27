#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define GNUPLOT "/usr/local/Cellar/gnuplot/5.2.7_1/bin/gnuplot -persist" // change this based on where your gnuplot executable file is located. To locate it, type `brew ls gnuplot` in the terminal

char DATASET_FILEPATH[1000];

void plotGraph(float slope, float yIntercept);

int main()
{
    float coordX[10000], coordY[10000];
    char x[1000], *y, countChar;
    FILE *fptr;
    int count = 0;

    //declare the delim
    const char delim[2] = ",";

    printf("\nPlease enter the file path for your dataset: ");
    scanf("%s", &DATASET_FILEPATH);
    fptr = fopen(DATASET_FILEPATH, "r");

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

        //get the x coordinates and store in x
        fscanf(fptr,"%10000s[^\n]", x);

        //get the first token (getting the y coordinates)
        y = strtok(x, delim);
        
        // walk through other tokens while the token is null
        while( y != NULL ) {
            //printf( " %s\n", token );
            coordY[count] = (float)atof(y); //store token to the coordY for every count
            y = strtok(NULL, delim); //make the token null after storing
        }

        //store the x coorindates in array coordX
        coordX[count] = (float)atof(x);

        //take next character from file.
        countChar = getc(fptr);
    }

    fclose(fptr); //close file.
    //end of pulling coordinates from file


    
    float sumX = 0.0, sumY = 0.0, xMean, yMean, numer = 0.0, denom = 0.0, b0, b1; // variables needed for calculating equation for the regression line
    float sumXY, sumXSq, sumYSq, sqSumX, sqSumY, r, rSq; // variables needed for calculating correlation coefficient & coefficient of determination

    int n = sizeof(coordY)/sizeof(coordY[0]);

    for (int i = 0; i < n; i++)
    {
        float x = coordX[i];
        float y = coordY[i];

        // calculate all summations needed
        sumX += x;
        sumY += y;
        sumXY += x*y;
        sumXSq += pow(x, 2);
        sumYSq += pow(y, 2);
    }

    // calculate square of the summation of x
    sqSumX += pow(sumX, 2);
    // calculate square of the summation of y
    sqSumY += pow(sumY, 2);

    //calculate the mean of x and y
    xMean = sumX / n;
    yMean = sumY / n;

    // Calculate correlation coefficient (r)
    float rNumer = (10000*sumXY)-(sumX*sumY);
    float rDenom = sqrt(((10000*sumXSq)-(sqSumX))*((10000*sumYSq)-sqSumY));
    r = rNumer/rDenom;
    rSq = pow(r, 2);

    for (int i = 0; i < n; i++)
    {
        //calculate x-xMean and y-yMean
        float xMinusXMean = coordX[i] - xMean;
        float yMinusYMean = coordY[i] - yMean;

        //calculate the denominator of equation (x - xMean)^2
        denom += pow(xMinusXMean, 2);

        //calculate the numeration (x-xMean)*(y-yMean)
        numer += xMinusXMean * yMinusYMean;
    }

    //calculate b0 and b1
    b1 = numer / denom;
    b0 = yMean - (b1*xMean);

    printf("y = %0.2f + %0.2fx\n", b0, b1);
    printf("sum of x and y: %0.2f and %0.2f\n", sumX, sumY);
    printf("Mean of x and y: %0.2f and %0.2f\n", xMean, yMean);
    printf("The correlation coefficient is %0.2f\n", r);
    printf("The coefficient of determination is %0.2f\n", rSq);

    plotGraph(b1, b0);

    return 0;
}

void plotGraph(float slope, float yIntercept)
{
    // Plot graph
    FILE *gp;
    gp = popen(GNUPLOT, "w"); // pipe to gnuplot program
    if (gp == NULL) {
        printf("Error opening pipe to GNU plot.\n"
            "Install with 'sudo apt-get install gnuplot' or 'brew install gnuplot'.\n");
        exit(0);
    }

    fprintf(gp, "set datafile separator comma\n");
    fprintf(gp, "plot '%s', y=%0.2fx+%0.2f\n", DATASET_FILEPATH, slope, yIntercept);
    fclose(gp);

}
