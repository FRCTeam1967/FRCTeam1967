package org.janksters.ExampleCommonClasses.Subsystems;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.util.Color;

import org.janksters.ExampleCommonClasses.Drawing.BitmapDrawingContext;

import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class LEDPanelSubsystem extends SubsystemBase implements BitmapDrawingContext {
    // This class supports one or more LED panels arranged horizontally,
    // and in a serpentine layout. Pixel 0 is at the top-left, and pixel
    // numbers increase going down until all pixels in the the first 
    // column are accounted for. The next pixel is immediately to the 
    // right, and pixel numbering continues upward.
    // 0  9  10 ...
    // 1  8  11 ...
    // 2  7  12 ...
    // 3  6  13 ...
    // 4  5  14 ...

    protected int width = 1;
    protected int height = 1;
    public double brightness = 0.25;
    
    private AddressableLED m_led;
    private AddressableLEDBuffer m_ledBuffer;

    public LEDPanelSubsystem(int width, int height, int pwmPin) {
        this.width = width > 0 ? width : 1;
        this.height = height > 0 ? height : 1;

        // Setup the buffer
        m_ledBuffer = new AddressableLEDBuffer(height * width);
        setColor(0, 0, 0);

        // Setup the LED updating system
        m_led = new AddressableLED(pwmPin);
        m_led.setLength(m_ledBuffer.getLength());
        m_led.setData(m_ledBuffer);
        m_led.start();
    }

    public void setPixelByID(int pixelID, int red, int green, int blue) {
        if (pixelID >= 0 && pixelID < m_ledBuffer.getLength()) {
            int scaledRed = (int)((double)red * brightness);
            int scaledGreen = (int)((double)green * brightness);
            int scaledBlue = (int)((double)blue * brightness);

            m_ledBuffer.setRGB(pixelID, (int)scaledRed, (int)scaledGreen, (int)scaledBlue);
        }
    }

    public void setPixelHSVByID(int pixelID, int hue, int saturation, int value) {
        m_ledBuffer.setHSV(pixelID, hue, saturation, (int)((double)value * brightness));
    }

    public void setPixelByXY(int x, int y, Color color) {
        setPixelByXY(x, y, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
    }

    public void setPixelByXY(int x, int y, int red, int green, int blue) {
        int pixelID = getPixelIDForXY(x, y);
        if (pixelID >= 0) {
            setPixelByID(pixelID, red, green, blue);
        }
    }

    // Set all pixels to the same color
    public void setColor(int red, int green, int blue) {
        for (int i = 0; i < width * height; i++) {
            setPixelByID(i, red, green, blue);
        }
    }

    public void setColor(Color color) {
        for (int i = 0; i < width * height; i++) {
            setPixelByID(i, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
        }
    }

    public void clearScreen(Color color) {
        setColor(color);
    }

    /**
     * Draw a rainbow across the LED panel starting at the given hue.
     * @param startingHue Starting hue value for the rainbow (0-180)
     * @return Next hue value to use if you want the rainbow to animate
     */
    public int setRainbow(int startingHue) {
        // For every pixel
        for (var i = 0; i < m_ledBuffer.getLength(); i++) {
            // Calculate the hue - hue is easier for rainbows because the color
            // shape is a circle so only one value needs to precess
            final var hue = (startingHue + (int)(i * 180.0 / m_ledBuffer.getLength())) % 180;
            // Set the value
            m_ledBuffer.setHSV(i, hue, 255, (int)(255 * brightness));
        }
        // Increase by to make the rainbow "move"
        startingHue += 3;
        // Check bounds
        startingHue %= 180;

        return startingHue;
    }

    public int width() {
        return width;
    }

    public int height() {
        return height;
    }

    // Commit the buffer so it's written to the pixels at the next update
    @Override
    public void periodic() {
        super.periodic();
        m_led.setData(m_ledBuffer);
    }

    public void commit() {
        m_led.setData(m_ledBuffer);
    }


    // Helper methods

    private int getPixelIDForXY(int x, int y) {
        // Make sure we're within bounds
        if (y >= height || y < 0) {
          System.out.println("Bad y passed to getPixelID: " + y);
          return -1;
        }
        if (x >= width || x < 0) {
          System.out.println("Bad x passed to getPixelID: " + x);
          return -1;
        }
    
        // Pixel index is the count of columns before this one times the number of rows, 
        // and then the number of rows before (for even columns, which stride down), or
        // after (for odd columns, which stride upward).
        int pixelIndex = x * height;
        if ((x % 2) == 0) {
          // Even column (0, 2, 4...). Pixel indices go from top to bottom in this column.
          pixelIndex += y;
        } else {
          // Odd column. Pixel indices got from bottom to top in this column
          pixelIndex += (height - y - 1); // At the top (row = 0), we'd skip maxRows-1 items
        }
    
        return pixelIndex;
      }
    
    }
