package org.janksters.ExampleCommonClasses.Commands;

import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Subsystems.LEDStringSubsystemInterface;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class LEDStringFadeInCommand extends CommandBase {
    protected LEDStringSubsystemInterface m_ledString;
    protected Color m_color;
    protected int m_lastAnimationStep = -1;
    protected double m_animationDelay;

    protected Timer m_timer;
    protected DoubleSupplier m_timeBase;

    public boolean isReversed = false;

    // This version creates a timer internally.
    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringFadeInCommand(Color color, double animationDelay, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_color = color;
        m_animationDelay = animationDelay;
        m_timer = new Timer();
        m_timeBase = () -> m_timer.get();
        
        addRequirements(subsystem);
    }

    // This version takes a DoubleSupplier to act as the time base so it can share a synchronized timer with 
    // other commands.
    public <T extends Subsystem & LEDStringSubsystemInterface> LEDStringFadeInCommand(Color color, double animationDelay, DoubleSupplier timeBase, T subsystem) {
        m_ledString = (LEDStringSubsystemInterface)subsystem;
        m_color = color;
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
        int currentAnimationStep = (int)Math.floor(m_timeBase.getAsDouble() / m_animationDelay);
        if (currentAnimationStep != m_lastAnimationStep) {
            m_lastAnimationStep = currentAnimationStep;

            // We start with black, and do 10 brightnesses of the selected color. 
            // So mid animiation, it will look like:
            // 100 100 90 80 70 60 50 40 30 20 10 0 0 0 ....
            // Pixel n at iteration i will be at brightness (i - n) * 10 with a floor at 0, and ceiling at 100
            for (int n = 0; n < m_ledString.length(); n++) {
                double unboundedBrightness = 0.0;
                unboundedBrightness = ((currentAnimationStep - n) * 0.1);
                double brightness = Math.min(1.0, Math.max(0.0, unboundedBrightness));
                Color color = colorAtBrightness(m_color, brightness);

                int pixel = isReversed ? n : m_ledString.length() - 1 - n;
                m_ledString.setPixel(pixel, color);
            }

        }
    }

    @Override
    public boolean isFinished() {
        // It takes n iterations to start lighting the last pixel, and then 10 steps to fill it.
        return m_lastAnimationStep > m_ledString.length() + 10;
    }

    public Color colorAtBrightness(Color color, double brightness) {
        // If we're at 100%, avoid the math.
        if (brightness >= 0.99) {
            return color;
        }

        // This will work best with primary colors. We should probably convert to HSV and then adjust just
        // the V, but that's a pain.
        double red = color.red * brightness;
        double green = color.green * brightness;
        double blue = color.blue * brightness;

        return new Color(red, green, blue);
    }
}
