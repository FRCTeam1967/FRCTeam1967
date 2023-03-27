package org.janksters.ExampleCommonClasses.Subsystems;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.util.Color;

import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class LEDStringSubsystem extends SubsystemBase implements LEDStringSubsystemInterface {
    protected int m_length = 1;
    public double brightness = 0.25;
    
    private AddressableLED m_led;
    private AddressableLEDBuffer m_ledBuffer;

    public LEDStringSubsystem(int length, int pwmPin) {
        m_length = length > 0 ? length : 1;

        // Setup the buffer
        m_ledBuffer = new AddressableLEDBuffer(m_length);
        setColor(0, 0, 0);

        // Setup the LED updating system
        m_led = new AddressableLED(pwmPin);
        m_led.setLength(m_ledBuffer.getLength());
        m_led.setData(m_ledBuffer);
        m_led.start();
    }

    public void setPixel(int pixelID, Color color) {
        setPixel(pixelID, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
    }

    public void setPixel(int pixelID, int red, int green, int blue) {
        if (pixelID >= 0 && pixelID < m_ledBuffer.getLength()) {
            int scaledRed = (int)((double)red * brightness);
            int scaledGreen = (int)((double)green * brightness);
            int scaledBlue = (int)((double)blue * brightness);

            m_ledBuffer.setRGB(pixelID, (int)scaledRed, (int)scaledGreen, (int)scaledBlue);
        }
    }

    public void setPixelHSV(int pixelID, int hue, int saturation, int value) {
        m_ledBuffer.setHSV(pixelID, hue, saturation, (int)((double)value * brightness));
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


    // Commit the buffer so it's written to the pixels at the next update
    @Override
    public void periodic() {
        super.periodic();
        m_led.setData(m_ledBuffer);
    }

    public int length() {
        return m_length;
    }
}