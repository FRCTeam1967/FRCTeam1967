package org.janksters.ExampleCommonClasses;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.util.Color;

import java.awt.image.*;

public class LEDPanel {
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

    public int m_width = 1;
    public int m_height = 1;
    public double m_brightness = 0.25;
    
    private AddressableLED m_led;
    private AddressableLEDBuffer m_ledBuffer;

    public LEDPanel(int width, int height, int pwmPin) {
        m_width = width > 0 ? width : 1;
        m_height = height > 0 ? height : 1;

        // Setup the buffer
        m_ledBuffer = new AddressableLEDBuffer(m_height * m_width);
        setColor(0, 0, 0);

        // Setup the LED updating system
        m_led = new AddressableLED(pwmPin);
        m_led.setLength(m_ledBuffer.getLength());
        m_led.setData(m_ledBuffer);
        m_led.start();
    }

    public void setPixelByID(int pixelID, int red, int green, int blue) {
        int scaledRed = (int)((double)red * m_brightness);
        int scaledGreen = (int)((double)green * m_brightness);
        int scaledBlue = (int)((double)blue * m_brightness);

        m_ledBuffer.setRGB(pixelID, (int)scaledRed, (int)scaledGreen, (int)scaledBlue);
    }

    public void setPixelHSVByID(int pixelID, int hue, int saturation, int value) {
        m_ledBuffer.setHSV(pixelID, hue, saturation, (int)((double)value * m_brightness));
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
        for (int i = 0; i < m_width * m_height; i++) {
            setPixelByID(i, red, green, blue);
        }
    }

    public void setColor(Color color) {
        for (int i = 0; i < m_width * m_height; i++) {
            setPixelByID(i, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
        }
    }

    // Set pixels to display the given image (or what fits)
    public void setImage(BufferedImage image) {
        // We might want to be more efficient about this, but if we're changing images,
        // we need to blank the pixels the new image isn't drawing directly.
        setColor(0, 0, 0);

        // Constrain output to actual dimensions
        int imageWidth = image.getWidth();
        int imageHeight = image.getHeight();
        int actualWidth = imageWidth > m_width ? m_width : imageWidth;
        int actualHeight = imageHeight > m_height ? m_height : imageHeight;
    
        // Iterate over the pixels of the images that we're using
        for (int x = 0; x < actualWidth; x++) {
            for (int y = 0; y < actualHeight; y++) {
                //Retrieve the R G B values
                int pixelRGB = image.getRGB(x,y);
                int red = (pixelRGB >> 16) & 0xFF;
                int green = (pixelRGB >> 8) & 0xFF;
                int blue = pixelRGB & 0xFF;

                setPixelByXY(x, y, red, green, blue);
            }
        }
    }

    // Pass a starting hue (0-180). Will return the next hue that should be passed in if you want
    // to make it move.
    public int setRainbow(int startingHue) {
        // For every pixel
        for (var i = 0; i < m_ledBuffer.getLength(); i++) {
            // Calculate the hue - hue is easier for rainbows because the color
            // shape is a circle so only one value needs to precess
            final var hue = (startingHue + (int)(i * 180.0 / m_ledBuffer.getLength())) % 180;
            // Set the value
            m_ledBuffer.setHSV(i, hue, 255, (int)(255 * m_brightness));
        }
        // Increase by to make the rainbow "move"
        startingHue += 3;
        // Check bounds
        startingHue %= 180;

        return startingHue;
    }
    

    // Commit the buffer so it's written to the pixels at the next update
    public void commit() {
        m_led.setData(m_ledBuffer);
    }


    // Helper methods

    private int getPixelIDForXY(int x, int y) {
        // Make sure we're within bounds
        if (y >= m_height) {
          System.out.println("Bad y passed to getPixelID: " + y);
          return -1;
        }
        if (x >= m_width) {
          System.out.println("Bad x passed to getPixelID: " + x);
          return -1;
        }
    
        // Pixel index is the count of columns before this one times the number of rows, 
        // and then the number of rows before (for even columns, which stride down), or
        // after (for odd columns, which stride upward).
        int pixelIndex = x * m_height;
        if ((x % 2) == 0) {
          // Even column (0, 2, 4...). Pixel indices go from top to bottom in this column.
          pixelIndex += y;
        } else {
          // Odd column. Pixel indices got from bottom to top in this column
          pixelIndex += (m_height - y - 1); // At the top (row = 0), we'd skip maxRows-1 items
        }
    
        return pixelIndex;
      }
    
    }
