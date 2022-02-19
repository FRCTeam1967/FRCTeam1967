// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.util.Color;

import java.io.File;
import javax.imageio.ImageIO;
import java.awt.image.*;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.Filesystem;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private static final String kDefaultAuto = "Default";
  private static final String kCustomAuto = "My Auto";
  private String m_autoSelected;
  private final SendableChooser<String> m_chooser = new SendableChooser<>();

  // LED stuff
  private int m_rainbowFirstPixelHue = 0;
  private AddressableLED m_led;
  private AddressableLEDBuffer m_ledBuffer;
  private BufferedImage m_image;
  private static final int m_ledWidth = 60; // 48
  private static final int m_ledHeight = 16;
  private static final double m_brightness = 0.25;


  // Images
  BufferedImage m_image_blackAndWhite;
  BufferedImage m_image_black;
  BufferedImage m_image_gray;
  BufferedImage m_image_redGreen;
  BufferedImage m_image_original;

  // Hack to change images
  private int m_tickCount = 0;
  static int m_imageNum = 0;

      
  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    // PWM port 0
    // Must be a _PWM_ header
    m_led = new AddressableLED(0);

    // Reuse buffer
    // Length is expensive to set, so only set it once, then just update data
    m_ledBuffer = new AddressableLEDBuffer(m_ledHeight * m_ledWidth);
    m_led.setLength(m_ledBuffer.getLength());

    // Set the data
    m_led.setData(m_ledBuffer);
    setColor(0, 0, 0);
    m_led.start();

    // Load our images
    m_image_original = loadImage("48x16 Logo.png");
    m_image_blackAndWhite = loadImage("48x16 Logo B&W.png");
    m_image_black = loadImage("48x16 Logo Black.png");
    m_image_gray = loadImage("48x16 Logo Gray.png");
    m_image_redGreen = loadImage("48x16 Logo.png");
  
    m_image = m_image_redGreen;

    pushLoadedImage();
  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    // Fill the buffer with a rainbow
    // updateRainbow();
    // pushLoadedImage();
    // setColor(0, 0, 255);

    m_tickCount++;

    if ((m_tickCount % 200) == 0) {
      m_imageNum++;
      if (m_imageNum > 4) {
        m_imageNum = 0;
      }

      System.out.println("Switching to image: " + m_imageNum);

      switch (m_imageNum) {
        case 4:
          m_image = m_image_blackAndWhite;
          break;
        case 3:
          m_image = m_image_black;
          break;
        case 2:
         m_image = m_image_gray;
         break;
        case 1:
          m_image = m_image_redGreen;
          break;
        case 0:
          m_image = m_image_original;
          break;
      }
      pushLoadedImage();
    }
    
    // Set the LEDs (commit)
    m_led.setData(m_ledBuffer);
  }

  /**
   * This autonomous (along with the chooser code above) shows how to select between different
   * autonomous modes using the dashboard. The sendable chooser code works with the Java
   * SmartDashboard. If you prefer the LabVIEW Dashboard, remove all of the chooser code and
   * uncomment the getString line to get the auto name from the text box below the Gyro
   *
   * <p>You can add additional auto modes by adding additional comparisons to the switch structure
   * below with additional strings. If using the SendableChooser make sure to add them to the
   * chooser code above as well.
   */
  @Override
  public void autonomousInit() {
    m_autoSelected = m_chooser.getSelected();
    // m_autoSelected = SmartDashboard.getString("Auto Selector", kDefaultAuto);
    System.out.println("Auto selected: " + m_autoSelected);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    switch (m_autoSelected) {
      case kCustomAuto:
        // Put custom auto code here
        break;
      case kDefaultAuto:
      default:
        // Put default auto code here
        break;
    }
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {}

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {}

  /** This function is called periodically when disabled. */
  @Override
  public void disabledPeriodic() {}

  /** This function is called once when test mode is enabled. */
  @Override
  public void testInit() {}

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}

  private void updateRainbow() {
    // For every pixel
    for (var i = 0; i < m_ledBuffer.getLength(); i++) {
      // Calculate the hue - hue is easier for rainbows because the color
      // shape is a circle so only one value needs to precess
      final var hue = (m_rainbowFirstPixelHue + (int)(i * 180.0 / m_ledBuffer.getLength())) % 180;
      // Set the value
      m_ledBuffer.setHSV(i, hue, 255, (int)(255 * m_brightness));
    }
    // Increase by to make the rainbow "move"
    m_rainbowFirstPixelHue += 3;
    // Check bounds
    m_rainbowFirstPixelHue %= 180;
  }

  private void setColor(int red, int green, int blue) {
    // Change every pixel blue
    for (var i = 0; i < m_ledBuffer.getLength(); i++) {
      setRGB(i, red, green, blue);
    }
  }

  private int getPixelIDForRowAndColumn(int row, int col, int rowOffset, int colOffset) {
    // Make sure we're within bounds
    if (row >= m_ledHeight) {
      System.out.println("Bad row passed to getPixelID: " + row);
      return -1;
    }
    if (col >= m_ledWidth) {
      System.out.println("Bad col passed to getPixelID: " + col);
      return -1;
    }

    // Pixel index is the count of columns before this one times the number of rows, 
    // and then the number of rows before (for even columns, which stride down), or
    // after (for odd columns, which stride upward).
    int pixelIndex = col * m_ledHeight;
    if ((col % 2) == 0) {
      // Even column (0, 2, 4...). Pixel indices go from top to bottom in this column.
      pixelIndex += row;
    } else {
      // Odd column. Pixel indices got from bottom to top in this column
      pixelIndex += (m_ledHeight - row - 1); // At the top (row = 0), we'd skip maxRows-1 items
    }

    return pixelIndex;
  }

  private BufferedImage loadImage(String fileName) {
    File deployDirectory = Filesystem.getDeployDirectory();
    File imageFile = new File(deployDirectory, fileName);
    System.out.println("loadImage() called with fn: " + fileName);

    BufferedImage loadedImage;

    try {
      loadedImage = ImageIO.read(imageFile);
       System.out.println("Successfully loaded image file!");
       System.out.println("Image dimensions are: " + loadedImage.getWidth() + " x " + loadedImage.getHeight());
    }
    catch(Exception e) {
      System.out.println("Couldn't load image: " + imageFile.getPath());
      return null;
    }

    return loadedImage;

  }

  private void pushLoadedImage() {
    // We might want to be more efficient about this, but if we're changing images,
    // we need to blank the pixels the new image isn't drawing directly.
    setColor(0, 0, 0);

    // Constrain output to actual dimensions
    int imageWidth = m_image.getWidth();
    int imageHeight = m_image.getHeight();
    int actualCols = imageWidth > m_ledWidth ? m_ledWidth : imageWidth;
    int actualRows = imageHeight > m_ledHeight ? m_ledHeight : imageHeight;
    
    // Iterate over the pixels of the images that we're using
    for (int x = 0; x < actualCols; x++) {
      for (int y = 0; y < actualRows; y++) {
        //Retrieve the R G B values
        int pixelRGB = m_image.getRGB(x,y);
        int red = (pixelRGB >> 16) & 0xFF;
        int green = (pixelRGB >> 8) & 0xFF;
        int blue = pixelRGB & 0xFF;
        int pixelID = getPixelIDForRowAndColumn(y, x, 0, 0);

        if (pixelID >= 0) {
          setRGB(pixelID, red, green, blue);
        }
      }
    }
  }

  // Abstrated so we can apply brightness or gamme if we want to get really fancy
  private void setRGB(int pixelID, int red, int green, int blue) {
    int scaledRed = (int)(red * m_brightness);
    int scaledGreen = (int)(green * m_brightness);
    int scaledBlue = (int)(blue * m_brightness);

    m_ledBuffer.setRGB(pixelID, (int)scaledRed, (int)scaledGreen, (int)scaledBlue);
  }

}