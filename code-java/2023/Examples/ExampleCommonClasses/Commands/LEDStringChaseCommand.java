package org.janksters.ExampleCommonClasses.Commands;

import java.util.ArrayList;
import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Subsystems.LEDStringSubsystemInterface;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class LEDStringChaseCommand extends CommandBase {
    protected LEDStringSubsystemInterface m_ledString;
    protected ArrayList<Color> m_colors;
    protected int m_chaseLength;
    protected double m_chaseDelay;

    public boolean isReversed = false;

    protected DoubleSupplier m_timeBase;
    private Timer m_timer;
    private int m_chaseStart = 0;

    // This version creates a timer internally.
    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringChaseCommand(ArrayList<Color> colors, int chaseWidth, double chaseDelay, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_colors = colors;
        m_chaseLength = chaseWidth;
        m_chaseDelay = chaseDelay;
        m_timer = new Timer();
        m_timeBase = () -> m_timer.get();
        
        addRequirements(subsystem);
    }

    // This version takes a DoubleSupplier to act as the time base so it can share a synchronized timer with 
    // other commands.
    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringChaseCommand(ArrayList<Color> colors, int chaseWidth, double chaseDelay, DoubleSupplier timeBase, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_colors = colors;
        m_chaseLength = chaseWidth;
        m_chaseDelay = chaseDelay;
        m_timeBase = timeBase;
        
        addRequirements(subsystem);
    }


    @Override
    public void initialize() {
        // If we are using our own timer, start it.
        if (m_timer != null) {
            m_timer.start();
        }
    }

    @Override
    public void execute() {
        // The basic chase pattern looks like:
        // <Color0>..<Color0><Color1>..<Color1>... where each sequence of a given color is m_chaseLength long
        // As we loop through all of the pixels we have, we keep an index into the pattern so we know where
        // we are. The color for a given index into that pattern is simply the color # (index / m_chaseLength).
        // 
        // To make things animate, each time we draw, we don't start
        // at index 0 into the pattern. Instead, we move backwards so the pattern appears to move to the right.
        int totalPatternLength = m_colors.size() * m_chaseLength;

        // Our current place in the chase is how many full chase periods have elapsed since the timer
        // was started.
        int currentPatternIndex = (int)Math.floor(m_timeBase.getAsDouble() / m_chaseDelay) * (isReversed ? 1 : -1);
        currentPatternIndex %= totalPatternLength;
        if (currentPatternIndex < 0) {
            currentPatternIndex += totalPatternLength;
        }
        assert(currentPatternIndex >= 0);

        if (currentPatternIndex != m_chaseStart) {
            m_chaseStart = currentPatternIndex;

            for (int i = 0; i < m_ledString.length(); i++) {
                int colorIndex = currentPatternIndex / m_chaseLength;
                Color pixelColor = m_colors.get(colorIndex);
                m_ledString.setPixel(i, pixelColor);

                currentPatternIndex = (currentPatternIndex + 1) % totalPatternLength;
            }
        }
    }

    @Override
    public boolean isFinished() {
        return false;
    }
}
