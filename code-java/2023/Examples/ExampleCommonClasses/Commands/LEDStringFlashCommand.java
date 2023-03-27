package org.janksters.ExampleCommonClasses.Commands;

import java.util.ArrayList;
import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Subsystems.LEDStringSubsystemInterface;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class LEDStringFlashCommand extends CommandBase {
    protected LEDStringSubsystemInterface m_ledString;
    protected ArrayList<Color> m_colors;
    protected double m_flashDelay;

    public boolean isReversed = false;

    private DoubleSupplier m_timeBase;
    private Timer m_timer;
    private int m_colorIndex = -1;

    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringFlashCommand(ArrayList<Color> colors, double flashDuration, T subsystem) {
        m_timer = new Timer();
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_colors = colors;
        m_flashDelay = flashDuration;
        m_timeBase = () -> m_timer.get();
        
        addRequirements(subsystem);
    }

    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringFlashCommand(ArrayList<Color> colors, double flashDuration, DoubleSupplier timeBase, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_colors = colors;
        m_flashDelay = flashDuration;
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
        // Figure out which color we should display based on the time elapsed
        int newColorIndex = (int)Math.floor(m_timeBase.getAsDouble() / m_flashDelay) % m_colors.size();

        if (m_colorIndex != newColorIndex) {
            m_colorIndex = newColorIndex;
            m_ledString.setColor(m_colors.get(m_colorIndex));
        }
    }

    public void drawCurrentColor() {
        m_ledString.setColor(m_colors.get(m_colorIndex));
    }

    @Override
    public boolean isFinished() {
        return false;
    }
}
