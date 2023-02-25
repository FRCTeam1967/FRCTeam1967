package org.janksters.ExampleCommonClasses.Drawing;

public class Point implements Cloneable {
    public int x;
    public int y;

    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public Point(Point p) {
        x = p.x;
        y = p.y;
    }

    public void translate(int dx, int dy) {
        x += dx;
        y += dy;
    }

    @Override
    public Point clone() throws CloneNotSupportedException {
        return (Point)super.clone();
    }
}
