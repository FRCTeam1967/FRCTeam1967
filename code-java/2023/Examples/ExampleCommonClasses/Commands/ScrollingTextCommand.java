package org.janksters.ExampleCommonClasses.Commands;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.Subsystem;
import edu.wpi.first.wpilibj.Timer;

import org.janksters.ExampleCommonClasses.Drawing.BitmapDrawing;
import org.janksters.ExampleCommonClasses.Drawing.BitmapDrawingContext;
import org.janksters.ExampleCommonClasses.Drawing.BitmapFont;
import org.janksters.ExampleCommonClasses.Drawing.Dimension;
import org.janksters.ExampleCommonClasses.Drawing.Point;
import org.janksters.ExampleCommonClasses.Drawing.Rectangle;

public class ScrollingTextCommand extends TextCommand {
    protected double m_secPerPixel;
    protected Timer m_timer;
    protected int m_currentOffset;
    protected Dimension m_bounds;

    public <T extends Subsystem & BitmapDrawingContext> ScrollingTextCommand(String text, Point origin, double pixelsPerSecond, BitmapFont font, Color color, T subsystem) {
        super(text, origin, font, color, subsystem);
        m_secPerPixel = 1.0/pixelsPerSecond;
        m_bounds = font.bounds(text);
    }

    @Override
    public void initialize() {
        super.initialize();

        m_timer = new Timer();
        m_timer.start();

        // Position the origin so only the first column is showing
        origin = new Point(m_ledSubsystem.width() - 1, origin.y);
    }

    @Override
    public void execute() {
        if (m_timer.hasElapsed(m_secPerPixel)) {
            m_timer.reset();

            // Shift the origin
            origin.translate(-1, 0);

            BitmapDrawing.drawRect(new Rectangle(0, origin.y, m_ledSubsystem.width(), m_bounds.height), Color.kBlack, m_ledSubsystem);
        }

        super.execute();
    }

    @Override
    public boolean isFinished() {
        // We're done with the string has moved completely off the left side
        return (origin.x + m_bounds.width < 0);
    }

    @Override
    public void end(boolean interrupted) {
        m_timer.stop();
        m_timer.reset();
    }
}