package org.janksters.ExampleCommonClasses.Drawing;

public class Dimension implements Cloneable {
    public int width;
    public int height;

    public Dimension(int width, int height) {
        this.width = width;
        this.height = height;
    }

    public Dimension(Dimension d) {
        width = d.width;
        height = d.height;
    }

    @Override
    public Dimension clone() throws CloneNotSupportedException {
        return (Dimension)super.clone();
    }

}
