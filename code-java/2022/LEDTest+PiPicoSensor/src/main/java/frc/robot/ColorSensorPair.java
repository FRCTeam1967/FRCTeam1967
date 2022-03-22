package frc.robot;

import edu.wpi.first.wpilibj.util.Color;

import com.revrobotics.ColorMatchResult;
import com.revrobotics.ColorMatch;

// Extends PicoColorSensor with color matching capability
public class ColorSensorPair extends PicoColorSensor {
    private ColorMatch m_colorMatcher = new ColorMatch();
    private double m_confidenceThreshold = -1;
  
    /**
     * Configure the color matcher. The same color matcher will be used for
     * both sensor.
     * 
     * @param matchColors array of Color objects to match against
     */
    public void setColorMatches(Color matchColors[]) {
        m_colorMatcher = new ColorMatch();
        if (m_confidenceThreshold >= 0) {
            m_colorMatcher.setConfidenceThreshold(m_confidenceThreshold);
        }
        for (Color matchColor : matchColors) {
            m_colorMatcher.addColorMatch(matchColor);
        }
    }

    /**
     * Configure the confidence interval used when matching colors
     * 
     * @param threshhold confidence threshhold to use (0.0 - 1.0)
     */
    public void setConfidenceThreshold(double threshhold) {
        m_confidenceThreshold = threshhold;
        if (m_colorMatcher != null) {
            m_colorMatcher.setConfidenceThreshold(threshhold);
        }
    }


    /**
     * Get the most recently seen Color object for sensor 0
     * 
     * @return Color
     */
    public Color getColor0() {
        return colorFromRawColor(getRawColor0());
    }

    /**
     * Get the most recently seen Color object for sensor 1
     * 
     * @return Color
     */
    public Color getColor1() {
       return colorFromRawColor(getRawColor1());
    }


    /**
     * Return the configured match target color that most closely matches the currently observed color. This method
     * forces a match to occur even if it isn't confident in the result. Sensor 0
     * 
     * @return ColorMatchResult for closest match (even if not confident)
     */
    public ColorMatchResult closestMatch0() {
        return m_colorMatcher.matchClosestColor(getColor0());
    }

    /**
     * Return the configured match target color that most closely matches the currently observed color. This method
     * forces a match to occur even if it isn't confident in the result. Sensor 1
     * 
     * @return ColorMatchResult for closest match (even if not confident)
     */
    public ColorMatchResult closestMatch1() {
        return m_colorMatcher.matchClosestColor(getColor1());

    }

    /**
     * Run the color matching algorithm against the color currently seen by sensor 0. 
     * 
     * @return ColorMatchResult for a confident match, or null if no such match
     */
    public ColorMatchResult match0() {
        return m_colorMatcher.matchColor(getColor0());
    }

    /**
     * Run the color matching algorithm against the color currently seen by sensor 1. 
     * 
     * @return ColorMatchResult for a confident match, or null if no such match
     */
    public ColorMatchResult match1() {
        return m_colorMatcher.matchColor(getColor1());
    }


    // Helper methods

    /**
      * Convert the raw color into a Color object using the same mechanism that ColorSensorV3 uses.
      *
      * @param RawColor RawColor data from the sensor
      * @return Color
      */
    protected Color colorFromRawColor(RawColor rawColor) {
        double r = (double) rawColor.red;
        double g = (double) rawColor.green;
        double b = (double) rawColor.blue;
        double mag = r + g + b;
        return new Color(r / mag, g / mag, b / mag);
     }
}
