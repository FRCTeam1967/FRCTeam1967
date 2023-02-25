package org.janksters.ExampleCommonClasses.Commands;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

import org.janksters.ExampleCommonClasses.Drawing.BitmapDrawingContext;
import org.janksters.ExampleCommonClasses.Drawing.BitmapFont;
import org.janksters.ExampleCommonClasses.Drawing.Point;

public class TextCommand extends CommandBase {
    protected BitmapDrawingContext m_ledSubsystem;
    protected String m_text;
    protected BitmapFont m_font;
    protected Color m_color;

    public Point origin = new Point(0, 0);

    /**
     * Create a command that displays text on the LED panel. This command
     * completes immediately. If other things draw to the LED panel, you may
     * need to combine this with a wait command to keep it onscreen.
     * @param text The string to show on the LED panel
     * @param font The BitmapFont to use
     * @param color The color to use for drawing
     * @param panelSubsystem The LEDPanelSubsystem to use
     */
    public <T extends Subsystem, BitmapDrawinContext> TextCommand(String text, BitmapFont font, Color color, T subsystem) {
        this(text, null, font, color, subsystem);
    }

    /**
     * Create a command that displays text on the LED panel. This command
     * completes immediately. If other things draw to the LED panel, you may
     * need to combine this with a wait command to keep it onscreen.
     * @param text The string to show on the LED panel
     * @param origin Origin of the text (top left corner)
     * @param font The BitmapFont to use
     * @param color The color to use for drawing
     * @param panelSubsystem The LEDPanelSubsystem to use
     */
    public <T extends Subsystem, BitmapDrawinContext> TextCommand(String text, Point origin, BitmapFont font, Color color, T subsystem) {
        m_ledSubsystem = (BitmapDrawingContext)subsystem;
        m_text = text;
        m_font = font;
        m_color = color;
        this.origin = origin != null ? new Point(origin) : new Point(0,0);

        addRequirements(subsystem);
    }
    @Override
    public void initialize() {
        super.initialize();
    }

    @Override
    public void execute() {
        m_font.drawString(m_text, origin, m_color, m_ledSubsystem);
    }

    @Override
    public boolean isFinished() {
        return true;
    }

}
