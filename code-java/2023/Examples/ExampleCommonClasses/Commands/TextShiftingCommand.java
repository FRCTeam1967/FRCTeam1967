package org.janksters.ExampleCommonClasses.Commands;

import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Drawing.BitmapFont;
import org.janksters.ExampleCommonClasses.Drawing.Point;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class TextShiftingCommand extends TextCommand {
    protected DoubleSupplier m_supplier;
    protected int m_stringPixelWidth = 0;

    public <T extends Subsystem, BitmapDrawinContext> TextShiftingCommand(String text, DoubleSupplier offsetSupplier, Point origin, BitmapFont font, Color color, T subsystem) {
        super(text, origin, font, color, subsystem);
        m_supplier = offsetSupplier;
        m_stringPixelWidth = font.bounds(text).width;
    }

    @Override
    public void execute() {
        // Adjust the origin by what the current value of the offset
        origin = new Point((int)(m_stringPixelWidth * m_supplier.getAsDouble()), origin.y);

        // Clear the screen
        m_ledSubsystem.clearScreen(Color.kBlack);
        
        super.execute();
    }

    @Override
    public boolean isFinished() {
        return false;
    }
}
