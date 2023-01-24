
package frc.robot.subsystems;

import edu.wpi.first.wpilibj.util.Color;

import frc.robot.LEDPanel;
import frc.robot.Constants.LEDConstants;

import java.awt.image.BufferedImage;

public class GerryRigLEDSubsystem extends LoggableSubsystem {
    private LEDPanel m_panel;
    private int m_hueIndex = 0;

    /** Creates a new GerryRigLEDSubsystem. */
    public GerryRigLEDSubsystem() {
        m_panel = new LEDPanel(LEDConstants.kMatrixWidth, LEDConstants.kMatrixHeight, LEDConstants.kLEDPWMPin);
        m_panel.m_brightness = LEDConstants.kBrightness;
    }

    @Override
    public void periodic() {
      // This method will be called once per scheduler run

      // If this subsystem becomes more complicated, we might want to commit here
      // but right now, we really only need to do something when a specific drawing
      // command is executed, and that should reduce load on the roborio.
      // m_panel.commit();
    }

    @Override
    public void simulationPeriodic() {
        // This method will be called once per scheduler run during simulation
    }

    public void drawRainbow() {
        m_hueIndex = m_panel.setRainbow(m_hueIndex);
        m_panel.commit();
    }

    public void setColor(Color color) {
        m_panel.setColor(color);
        m_panel.commit();
    }

    public void setImage(BufferedImage image) {
        m_panel.setImage(image);
        m_panel.commit();
    }

    public void setImage(BufferedImage image, double alpha) {
        m_panel.setImage(image, alpha);
        m_panel.commit();
    }

}
