package org.janksters.ExampleCommonClasses.Commands;

import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Subsystems.LEDStringSubsystemInterface;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class LEDStringRainbowCommand extends CommandBase {
    protected LEDStringSubsystemInterface m_ledString;
    protected double m_animationDelay;

    private Timer m_timer;
    private DoubleSupplier m_timeBase;
    private int m_startingHue = 0;
    private int m_intervalCount = -1;

    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringRainbowCommand(double animationDelay, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_animationDelay = animationDelay;
        m_timer = new Timer();
        m_timeBase = () -> m_timer.get();
        
        addRequirements(subsystem);
    }

    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringRainbowCommand(double animationDelay, DoubleSupplier timeBase, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_animationDelay = animationDelay;
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
        int currentInterval = (int)Math.floor(m_timeBase.getAsDouble() / m_animationDelay);

        // If we've crossed into a new interval (number of m_animationDelays that have passed), update the
        // rainbow. Note that if time is passing faster than we can update, we might get behind. We could fix
        // that by calling drawRainbow multiple times to figure out what the starting hue should be, but we
        // just ignore that for now.
        if (m_intervalCount != currentInterval) {
            m_intervalCount = currentInterval;
            m_startingHue = drawRainbow(m_startingHue);
        }
    }

    /**
     * Draw a rainbow across the LED string starting at the given hue.
     * @param startingHue Starting hue value for the rainbow (0-180)
     * @return Next hue value to use if you want the rainbow to animate
     */
    public int drawRainbow(int startingHue) {
        int stringLength = m_ledString.length();

        // For every pixel
        for (var i = 0; i < stringLength; i++) {
            // Calculate the hue - hue is easier for rainbows because the color
            // shape is a circle so only one value needs to precess
            final var hue = (startingHue + (int)(i * 180.0 / stringLength)) % 180;
            // Set the value
            m_ledString.setPixelHSV(i, hue, 255, 255);
        }
        // Increase by to make the rainbow "move"
        startingHue += 3;
        // Check bounds
        startingHue %= 180;

        return startingHue;
    }

    @Override
    public boolean isFinished() {
        return false;
    }
}
