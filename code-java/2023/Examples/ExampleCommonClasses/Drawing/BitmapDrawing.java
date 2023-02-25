package org.janksters.ExampleCommonClasses.Drawing;

import edu.wpi.first.wpilibj.util.Color;
import java.awt.image.BufferedImage;

public class BitmapDrawing {
    /**
     * Draw a rectangle with origin (upper left) at (x,y) and given width and height
     * @param origin Top left coordinate
     * @param width Rectangle width * @param height Rectangle height
     * @param color Rectangle color
     * @param context The drawing context
     */
    static public void drawRect(Rectangle rect, Color color, BitmapDrawingContext context) {
        for (int xOffset = 0; xOffset < rect.width; xOffset++) {
            for (int yOffset = 0; yOffset < rect.height; yOffset++) {
                context.setPixelByXY(rect.x + xOffset, rect.y + yOffset, color);
            }
        }
    }

    /**
     * Draws an image (or what fits of it) onto the drawing context
     * @param image BufferedImage to display
     */
    public void setImage(BufferedImage image, BitmapDrawingContext context) {
        // Constrain output to actual dimensions
        int imageWidth = image.getWidth();
        int imageHeight = image.getHeight();
        int contextWidth = context.width();
        int contextHeight = context.height();
        int actualWidth = Math.min(imageWidth, contextWidth);
        int actualHeight = Math.min(imageHeight, contextHeight);
    
        // Iterate over the pixels of the images that we're using
        for (int x = 0; x < actualWidth; x++) {
            for (int y = 0; y < actualHeight; y++) {
                //Retrieve the R G B values
                int pixelRGB = image.getRGB(x,y);
                int red = (pixelRGB >> 16) & 0xFF;
                int green = (pixelRGB >> 8) & 0xFF;
                int blue = pixelRGB & 0xFF;

                context.setPixelByXY(x, y, red, green, blue);
            }
        }
    }

}
