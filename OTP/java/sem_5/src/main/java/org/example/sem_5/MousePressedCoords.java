package org.example.demo3;

public class MousePressedCoords {

    private double X,Y;
    public void setX(double x) {
        this.X = x;
    }

    public void setY(double y) {
        this.Y = y;
    }

    public double getX() {
        return X;
    }

    public double getY() {
        return Y;
    }

    public double movingCoords(double startYone, double startXone,
                               double sizev2one, double sizev1one,
                               double startYtwo, double startXtwo,
                               double sizev2two, double sizev1two) {

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
}
