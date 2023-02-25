// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import org.janksters.ExampleCommonClasses.Drawing.Rectangle;

/**
 * The Constants class provides a convenient place for teams to hold robot-wide numerical or boolean
 * constants. This class should not be used for any other purpose. All constants should be declared
 * globally (i.e. public static). Do not put anything functional in this class.
 *
 * <p>It is advised to statically import this class (or one of its inner classes) wherever the
 * constants are needed, to reduce verbosity.
 */
public final class Constants {
  public static class OperatorConstants {
    public static final int kDriverControllerPort = 0;
  }

  public static class HardwareConstants {
    public static final int encoderCANID = 22;
  }

  public final static class LEDConstants {
    public static final int kLEDPWMPin = 0;
    public static final int kMatrixHeight = 16;
    public static final int kMatrixWidth= 16 * 3;

    public static final double kBrightness = 0.2;

    public final static class LEDRegions {
      public final static Rectangle kUpperHalf = new Rectangle(0, 0, kMatrixWidth, kMatrixHeight/2);
      public final static Rectangle kLowerHalf = new Rectangle(0, kMatrixHeight/2, kMatrixWidth, kMatrixHeight/2);
    }
  }

  public final class Fonts {
    public static final String kSmallFontFile = "CG-pixel-4x5-mono.bdf";
    public static final String kLargeFontFile = "6x13.bdf";
    public static final String kMediumFontFile = "5x8.bdf";
    public static final String kMediumAltFontFile = "tb-8.bdf";
  }
}
