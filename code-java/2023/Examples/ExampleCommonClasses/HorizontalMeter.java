package org.janksters.ExampleCommonClasses;

import edu.wpi.first.wpilibj.util.Color;


// HorizontalMeter
// A collection of rows in an LED panel that display one or more meter channels in something that
// resembles a sound equalizer display. It can be constrained to a portion of the LED panel
// configuration by setting the start column with setStartColumn. The height of the channel
// bar graph(s) are automatically adjusted to fix the height of the display, while making sure
// each channel has the same height; this means some display may be unused.

public class HorizontalMeter {
    private static int m_startColumn;
    private static LEDPanel m_ledPanel;
    private static int m_numChannels;
    private static int m_width = 0;
    private static int m_channelHeight = 1;

    public HorizontalMeter(LEDPanel panel, int numChannels) {
        m_ledPanel = panel;
        m_numChannels = numChannels;
        m_width = panel.m_width;
        m_channelHeight = panel.m_height / m_numChannels; // Integer math will take the floor()
    }

    // Sets ths starting column for the meter display. The area to the left can be used for 
    // something else.
    public void setStartColumn(int startColumn) {
        m_startColumn = startColumn;
        m_width = m_ledPanel.m_width - startColumn;
    }

    public void clearUnusedArea() {
        for (int row = m_numChannels * m_channelHeight; row < m_ledPanel.m_height; row++) {
            for (int col = 0; col < m_width; col++) {
                m_ledPanel.setPixelByXY(m_startColumn + col, row, Color.kBlack);
            }
        }
    }

    // Draws an output channel from the left with the given foreground and background color, and a range
    // of [0.0, 1.0].
    public void setOutputChannel(int meterIndex, double fraction, Color meterColor, Color backgroundColor) {
        assert(meterIndex < m_numChannels);
        fraction = Math.min(1.0, Math.max(0.0, fraction));  // Contrain to [0.0, 1.0]
        int meterValue = (int)(fraction * m_width + 0.5);
        int startRow = meterStartRow(meterIndex);
        for (int col = 0; col < m_width; col++) {
          for (int meterRow = 0; meterRow < m_channelHeight; meterRow++) {
            m_ledPanel.setPixelByXY(m_startColumn + col, startRow + meterRow, col < meterValue ? meterColor : backgroundColor);
          }
        }
    }

    // Draws an output channel from the left with the given foreground color, and a range
    // of [0.0, 1.0].
    public void setOutputChannel(int meterIndex, double fraction, Color meterColor) {
        setOutputChannel(meterIndex, fraction, meterColor, Color.kBlack);
    }

    // Draws an output channel from the center with given negative and positive colors, a background color, and
    // a range of [-1.0, 1.0].

    public void setCenterOutputChannel(int meterIndex, double fraction, Color posColor, Color negColor,
    Color bgColor, Color centerColor) {
        assert(meterIndex < m_numChannels);
        fraction = Math.min(1.0, Math.max(-1.0, fraction)); // Contrain to [-1.0, 1.0]
        int startRow = meterStartRow(meterIndex);
        double center = m_width / 2.0;
        int negativeOrigin = (int)Math.round(center - 0.1); // Center may or may not be an integer depending on the panel width
        int positiveOrigin = (int)Math.round(center + 0.1);

        // Compute the last pixel colored (either the left or right edge of the bar graph)
        // Negative column range is [0, negativeOrigin)
        // Positive column range is (positiveOrigin, width-1]
        int barGraphEdge = 0;
        if (fraction >= 0) {
            double range = (m_width - 1) - positiveOrigin;
            double value = range * fraction;
            barGraphEdge = positiveOrigin + (int)value;
        } else {
            double range = negativeOrigin - 0;
            double value = range * Math.abs(fraction);
            barGraphEdge = negativeOrigin - (int)value;
        }

        for (int col = 0; col < m_width; col++) {
            Color pixelColor = bgColor;
            if (col >= negativeOrigin && col <= positiveOrigin) {
                // Center of the meter
                pixelColor = centerColor;
            } else if (fraction < 0.0 && col < negativeOrigin) {
                // Negative meter. Only consider the left half of the meter
                pixelColor = col >= barGraphEdge ? negColor : bgColor;
            } else if (fraction > 0.0 && col > positiveOrigin) {
                // Positive meter. Only consider the right half of the meter
                pixelColor = col <= barGraphEdge ? posColor : bgColor;
            }

            // Apply the color to every row in the channel
            for (int meterRow = 0; meterRow < m_channelHeight; meterRow++) {
                m_ledPanel.setPixelByXY(m_startColumn + col, startRow + meterRow, pixelColor);
            }
        }
    }

    public void setCenterOutputChannel(int meterIndex, double fraction, Color negColor, Color posColor) {
        setCenterOutputChannel(meterIndex, fraction, negColor, posColor, Color.kBlack, Color.kPurple);
    }

    public int meterStartRow(int meterIndex) {
        return meterIndex * m_channelHeight;
    }
}
