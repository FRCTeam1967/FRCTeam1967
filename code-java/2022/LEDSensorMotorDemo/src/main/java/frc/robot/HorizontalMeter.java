package frc.robot;

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

    public void setStartColumn(int startColumn) {
        m_startColumn = startColumn;
        m_width = m_ledPanel.m_width - startColumn;
    }

    public void setOutputChannel(int meterIndex, double fraction, Color meterColor, Color backgroundColor) {
        assert(meterIndex < m_numChannels);
        int meterValue = (int)(fraction * m_width + 0.5);
        int startRow = meterIndex * m_channelHeight;
        for (int col = 0; col < m_width; col++) {
          for (int meterRow = 0; meterRow < m_channelHeight; meterRow++) {
            m_ledPanel.setPixelByXY(m_startColumn + col, startRow + meterRow, col < meterValue ? meterColor : backgroundColor);
          }
        }
    }

    public void setOutputChannel(int meterIndex, double fraction, Color meterColor) {
        setOutputChannel(meterIndex, fraction, meterColor, Color.kBlack);
    }

}
