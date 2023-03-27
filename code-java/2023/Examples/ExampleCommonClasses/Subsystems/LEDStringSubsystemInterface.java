package org.janksters.ExampleCommonClasses.Subsystems;

import edu.wpi.first.wpilibj.util.Color;

public interface LEDStringSubsystemInterface {
    public void setPixel(int pixelID, Color color);
    public void setPixel(int pixelID, int red, int green, int blue);
    public void setPixelHSV(int pixelID, int hue, int saturation, int value);
    
    public void setColor(int red, int green, int blue);
    public void setColor(Color color);

    public int length();
}
