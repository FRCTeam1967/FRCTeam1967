package org.janksters.ExampleCommonClasses.Subsystems;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class LEDSubstringSubsystem extends SubsystemBase implements LEDStringSubsystemInterface {
    protected LEDStringSubsystem m_ledSubsystem;
    protected int m_startOffset = 0;
    protected int m_length = 0;
    protected boolean m_isReversed = false;

    public LEDSubstringSubsystem(LEDStringSubsystem ledSubsystem, int offset, int length, boolean isReversed) {
        m_ledSubsystem = ledSubsystem;
        m_startOffset = offset;
        m_length = length;
        m_isReversed = isReversed;
    }

    public int mapPixelToRealString(int substringPixel) {
        // If the string is reversed, pixel 0 is as the end (length - 1)
        int pixel = m_isReversed ? (m_length - 1) - substringPixel : substringPixel;

        return pixel + m_startOffset;
    }

    public void setPixel(int pixelID, Color color) {
        setPixel(pixelID, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
    }
    
    public void setPixel(int pixelID, int red, int green, int blue) {
        // Bounds check
        if (pixelID < 0 || pixelID >= m_length) {
            return;
        }

        m_ledSubsystem.setPixel(mapPixelToRealString(pixelID), red, green, blue);
    }

    public void setPixelHSV(int pixelID, int hue, int saturation, int value) {
        // Bounds check
        if (pixelID < 0 || pixelID >= m_length) {
            return;
        }

        m_ledSubsystem.setPixelHSV(mapPixelToRealString(pixelID), hue, saturation, value);
    }

    // Set all pixels to the same color
    public void setColor(int red, int green, int blue) {
        for (int i = 0; i < m_length; i++) {
            setPixel(i, red, green, blue);
        }
    }

    public void setColor(Color color) {
        for (int i = 0; i < m_length; i++) {
            setPixel(i, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
        }
    }

    public int length() {
        return m_length;
    }

}
