// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

/**
 * The Constants class provides a convenient place for teams to hold robot-wide numerical or boolean
 * constants. This class should not be used for any other purpose. All constants should be declared
 * globally (i.e. public static). Do not put anything functional in this class.
 *
 * <p>It is advised to statically import this class (or one of its inner classes) wherever the
 * constants are needed, to reduce verbosity.
 */

public final class Constants {
    public final class LEDConstants {
        public static final int kLEDPWMPin = 0;
        public static final int kMatrixHeight = 16;
        public static final int kMatrixWidth= 16 * 3;

        public static final double kBrightness = 0.2;
    };

    public static final class Sound {
        public final String filename;
        public final int lengthMS;
        private Sound(String filename, int lengthMS) {
            this.filename = filename;
            this.lengthMS = lengthMS;
        }

        public static final Sound kStartMatch = new Sound("FRC-MatchStart.chrp", 2000);
        public static final Sound kSweetCaroline = new Sound("SweetCaroline.chrp", 216000);
        public static final Sound kSimpsons = new Sound("Simpsons.chrp", 300000);
    };

    public final class Images {
        public static final String kJanksterBow = "48x16 Logo Black.png";
    }

    public static final int kJoystickID = 0;

    // GerryRigSE
    // public static final int kFirstFalconOrchestraID = 4;
    // public static final int kSecondFalconOrchestraID = 13;

    // GerryRig
    public static final int kFirstFalconOrchestraID = 4;
    public static final int kSecondFalconOrchestraID = 13;
}
