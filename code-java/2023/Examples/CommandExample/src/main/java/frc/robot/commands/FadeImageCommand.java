package frc.robot.commands;

import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

import edu.wpi.first.wpilibj.Filesystem;
import edu.wpi.first.wpilibj.RobotController;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.subsystems.GerryRigLEDSubsystem;

public class FadeImageCommand extends CommandBase {
    private String m_imageFn;
    private BufferedImage m_image;
    private int m_fadeTimeMS;
    private boolean m_isFadeIn;
    private GerryRigLEDSubsystem m_ledSubsystem;

    // We can't use a Timer because we need resolution better than 1 seccond
    private double m_startTime;

    public FadeImageCommand(String imageFilename, int fadeDurationMS, boolean fadeIn, GerryRigLEDSubsystem subsystem) {
        m_imageFn = imageFilename;
        m_fadeTimeMS = fadeDurationMS;
        m_isFadeIn = fadeIn;
        m_ledSubsystem = subsystem;

        addRequirements(subsystem);
    }

    @Override
    public void initialize() {
        super.initialize();

        m_image = loadImage(m_imageFn);
        m_startTime = getCurrentTimeMS();
    }

    @Override
    public void execute() {
        super.execute();

        double timeElapsed = getCurrentTimeMS() - m_startTime;
        double percentComplete = timeElapsed / (double)m_fadeTimeMS;
        double alpha = m_isFadeIn ? percentComplete : 1 - percentComplete;
        if (alpha < 0.0) alpha = 0.0;
        if (alpha > 1.0) alpha = 1.0;
        m_ledSubsystem.setImage(m_image, alpha);
    }
    
    @Override
    public boolean isFinished() {
        double timeElapsed = getCurrentTimeMS() - m_startTime;
        return timeElapsed >= m_fadeTimeMS;
    }

    private BufferedImage loadImage(String fileName) {
        File deployDirectory = Filesystem.getDeployDirectory();
        File imageFile = new File(deployDirectory, fileName);
    
        BufferedImage loadedImage;
    
        try {
          loadedImage = ImageIO.read(imageFile);
        }
        catch(Exception e) {
          return null;
        }
    
        return loadedImage;
    
    }

    private double getCurrentTimeMS() {
        return (double)RobotController.getFPGATime() / 1000;
    }
}
