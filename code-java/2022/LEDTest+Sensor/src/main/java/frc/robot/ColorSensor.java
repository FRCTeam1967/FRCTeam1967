package frc.robot;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj.I2C;

import com.revrobotics.ColorSensorV3;
import com.revrobotics.ColorMatchResult;
import com.revrobotics.ColorMatch;


public class ColorSensor {
    private ColorSensorV3 m_colorSensor;
    private ColorMatch m_colorMatcher;
  
    public ColorSensor(I2C.Port i2cPort) {
        m_colorSensor = new ColorSensorV3(i2cPort);
    }

    public void setColorMatches(Color matchColors[]) {
        m_colorMatcher = new ColorMatch();

        for (Color matchColor : matchColors) {
            m_colorMatcher.addColorMatch(matchColor);
        }
    }

    public Color closestColor() {
        ColorMatchResult match = closestMatch();
        return match.color;
    }

    public ColorMatchResult closestMatch() {
        /**
         * The method GetColor() returns a normalized color value from the sensor and can be
         * useful if outputting the color to an RGB LED or similar. To
         * read the raw color, use GetRawColor().
         * 
         * The color sensor works best when within a few inches from an object in
         * well lit conditions (the built in LED is a big help here!). The farther
         * an object is the more light from the surroundings will bleed into the 
         * measurements and make it difficult to accurately determine its color.
         */
        Color detectedColor = rawColor();

        /**
         * Run the color match algorithm on our detected color
         */
        ColorMatchResult match = m_colorMatcher.matchClosestColor(detectedColor);
        return match;
    }

    public Color rawColor() {
        return m_colorSensor.getColor();
    }

    // Will return null if there's no match within the confidence level
    public ColorMatchResult match() {
        Color detectedColor = rawColor();
        return m_colorMatcher.matchColor(detectedColor);
    }

    public int getProximity() {
        return m_colorSensor.getProximity();
    }

    public void setConfidenceThreshold(double threshhold) {
        m_colorMatcher.setConfidenceThreshold(threshhold);
    }
}
