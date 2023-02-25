package org.janksters.ExampleCommonClasses.Drawing;

import edu.wpi.first.wpilibj.util.Color;

public interface BitmapDrawingContext {
    public void setPixelByXY(int x, int y, Color color);
    public void setPixelByXY(int x, int y, int red, int green, int blue);
    public void clearScreen(Color color);

    public int width();
    public int height();
}
