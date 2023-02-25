package org.janksters.ExampleCommonClasses.Drawing;

public class Rectangle implements Cloneable {
    // Similar to AWT's definition
    public int x;
    public int y;
    public int width;
    public int height;

    public Rectangle(int x, int y, int width, int height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public Rectangle(Rectangle rect) {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;
    }
    
    public Rectangle(Point p, Dimension d) {
        x = p.x;
        y = p.y;
        width = d.width;
        height = d.height;
    }  

    @Override
    public Rectangle clone() throws CloneNotSupportedException {
        return (Rectangle)super.clone();
    }
}
