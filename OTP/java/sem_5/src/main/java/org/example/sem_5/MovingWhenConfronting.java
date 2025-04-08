package org.example.demo3;

import java.util.Arrays;

public class MovingWhenConfronting {
    private double startXone;
    private double startYone;
    private double sizev1one;
    private double sizev2one;
    private double startXtwo;
    private double startYtwo;
    private double sizev1two;
    private double sizev2two;

    public double movingCoordsX(double startXone, double startYone, double sizev1one, double sizev2one,
                                double startXtwo, double startYtwo, double sizev1two, double sizev2two) {

        if (startXone+sizev1one >= startXtwo && startXone+sizev1one <= startXtwo+10) {
            if(startYone <= startYtwo+sizev2two && startYone+sizev2one >= startYtwo) {
                return startXone+sizev1one+3;
            }
        }

        if (startXtwo+sizev1two >= startXone && startXtwo+sizev1two <= startXone+10) {
            if(startYone <= startYtwo+sizev2two && startYone+sizev2one >= startYtwo) {
                return startXone-sizev1two-3;
            }
        }
        return startXtwo;
    }

    public double movingCoordsY(double startXone, double startYone, double sizev1one, double sizev2one,
                                double startXtwo, double startYtwo, double sizev1two, double sizev2two) {

        if (startYone+sizev2one >= startYtwo && startYone+sizev2one <= startYtwo+10) {
        if(startXone <= startXtwo+sizev1two && startXone+sizev1one >= startXtwo) {
            return startYone+sizev2one+3;
            }
        }

        if (startYtwo+sizev2two >= startYone && startYtwo+sizev2two <= startYone+10) {
        if(startXone <= startXtwo+sizev1two && startXone+sizev1one >= startXtwo) {
            return startYone-sizev2two-3;
            }
        }
        return startYtwo;
    }

    public double intersection(double startXone, double startYone, double sizev1one, double sizev2one,
                               double startXtwo, double startYtwo, double sizev1two, double sizev2two) {

        double[] coordsX = {startXone, startXone + sizev1one, startXtwo, startXtwo + sizev1two};
        coordsX = quickSort(coordsX, 0, 3);

        double[] coordsY = {startYone, startYone + sizev2one, startYtwo, startYtwo + sizev2two};
        coordsY = quickSort(coordsY, 0, 3);

        if (startXone + sizev1one >= startXtwo && startXone + sizev1one <= startXtwo + sizev1two) {
            if (startYone <= startYtwo + sizev2two && startYone + sizev2one >= startYtwo) {
                return (coordsX[2] - coordsX[1]) * (coordsY[2] - coordsY[1]);
            }
        }
        if (startXtwo + sizev1two >= startXone && startXtwo + sizev1two <= startXone + sizev1two) {
            if (startYone <= startYtwo + sizev2two && startYone + sizev2one >= startYtwo) {
                return (coordsX[2] - coordsX[1]) * (coordsY[2] - coordsY[1]);
            }
        }

        if (startYone + sizev2one >= startYtwo && startYone + sizev2one <= startYtwo + sizev2two) {
            if (startXone <= startXtwo + sizev1two && startXone + sizev1one >= startXtwo) {
                return (coordsX[2] - coordsX[1]) * (coordsY[2] - coordsY[1]);
            }
        }

        if (startYtwo + sizev2two >= startYone && startYtwo + sizev2two <= startYone + sizev2two) {
            if (startXone <= startXtwo + sizev1two && startXone + sizev1one >= startXtwo) {
                return (coordsX[2] - coordsX[1]) * (coordsY[2] - coordsY[1]);
            }
        }
        return 0;
    }
    public static double[] quickSort(double[] sortArr, int low, int high) {

        //выбираем опорный элемент
        int middle = low + (high - low) / 2;
        double border = sortArr[middle];

        //разделияем на подмассивы и меняем местами
        int i = low, j = high;
        while (i <= j) {
            while (sortArr[i] < border) i++;
            while (sortArr[j] > border) j--;
            if (i <= j) {
                double swap = sortArr[i];
                sortArr[i] = sortArr[j];
                sortArr[j] = swap;
                i++;
                j--;
            }
        }
        //рекурсия для сортировки левой и правой части
        if (low < j) quickSort(sortArr, low, j);
        if (high > i) quickSort(sortArr, i, high);

        return sortArr;
    }
}

