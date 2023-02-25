// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import java.io.File;

import org.janksters.ExampleCommonClasses.Commands.DrawRectangleCommand;
import org.janksters.ExampleCommonClasses.Commands.ScrollingTextCommand;
import org.janksters.ExampleCommonClasses.Commands.TextCommand;
import org.janksters.ExampleCommonClasses.Commands.TextShiftingCommand;
import org.janksters.ExampleCommonClasses.Commands.ValueDisplayCommand;
import org.janksters.ExampleCommonClasses.Drawing.BitmapFont;
import org.janksters.ExampleCommonClasses.Drawing.BitmapFontManager;
import org.janksters.ExampleCommonClasses.Drawing.Point;
import org.janksters.ExampleCommonClasses.Drawing.Rectangle;
import org.janksters.ExampleCommonClasses.Subsystems.LEDPanelSubsystem;
import org.janksters.ExampleCommonClasses.Subsystems.LEDRegionSubsystem;

import com.ctre.phoenix.sensors.CANCoderConfiguration;
import com.ctre.phoenix.sensors.CANCoderStatusFrame;
import com.ctre.phoenix.sensors.SensorTimeBase;
import com.ctre.phoenix.sensors.WPI_CANCoder;

import edu.wpi.first.wpilibj.Filesystem;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.ParallelCommandGroup;
import edu.wpi.first.wpilibj2.command.ParallelDeadlineGroup;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import edu.wpi.first.wpilibj2.command.button.Trigger;

/**
 * This class is where the bulk of the robot should be declared. Since Command-based is a
 * "declarative" paradigm, very little robot logic should actually be handled in the {@link Robot}
 * periodic methods (other than the scheduler calls). Instead, the structure of the robot (including
 * subsystems, commands, and trigger mappings) should be declared here.
 */
public class RobotContainer {
  // The robot's subsystems and commands are defined here...
  private final LEDPanelSubsystem m_ledSubsystem;
  private final LEDRegionSubsystem m_upperHalf;
  private final LEDRegionSubsystem m_lowerHalf;

  private final WPI_CANCoder m_encoder;

  // Replace with CommandPS4Controller or CommandJoystick if needed
  // private final CommandJoystick m_joystickController =
  //     new CommandJoystick(OperatorConstants.kDriverControllerPort);

  private BitmapFontManager m_fontManager = new BitmapFontManager();

  /** The container for the robot. Contains subsystems, OI devices, and commands. */
  public RobotContainer() {
    m_ledSubsystem = new LEDPanelSubsystem(Constants.LEDConstants.kMatrixWidth, Constants.LEDConstants.kMatrixHeight, Constants.LEDConstants.kLEDPWMPin);
    m_ledSubsystem.brightness = 0.10;

    m_upperHalf = new LEDRegionSubsystem(m_ledSubsystem, Constants.LEDConstants.LEDRegions.kUpperHalf);
    m_lowerHalf = new LEDRegionSubsystem(m_ledSubsystem, Constants.LEDConstants.LEDRegions.kLowerHalf);

    // Configure the trigger bindings
    configureBindings();

    // Configure the Encoder
    m_encoder = new WPI_CANCoder(Constants.HardwareConstants.encoderCANID);
    initEncoder();

    // m_ledSubsystem.setDefaultCommand(new RainbowCommand(m_ledSubsystem).ignoringDisable(true));

    File deployDirectory = Filesystem.getDeployDirectory();
    m_fontManager.addFont(new BitmapFont(new File(deployDirectory, Constants.Fonts.kSmallFontFile)));
    // m_fontManager.addFont(new BitmapFont(new File(deployDirectory, Constants.Fonts.kMediumFontFile)));
    m_fontManager.addFont(new BitmapFont(new File(deployDirectory, Constants.Fonts.kLargeFontFile)));
    // m_fontManager.addFont(new BitmapFont(new File(deployDirectory, Constants.Fonts.kMediumAltFontFile)));

    // Note: Subsystem default commands must require their subsystem. So at least one of the commands in a 
    // group assigned to m_ledSubsystem must require it.
    if (false) {
      m_ledSubsystem.setDefaultCommand(fontTestCommand());
    } else {
      var smallFont = m_fontManager.getFont(Constants.Fonts.kSmallFontFile);
      m_upperHalf.setDefaultCommand(
        new TextShiftingCommand("Janksters!", () -> (m_encoder.getAbsolutePosition() - 180) / 180.0, new Point(0, 0), smallFont, Color.kRed, m_upperHalf)
          .ignoringDisable(true)
      );
      m_lowerHalf.setDefaultCommand(
        new ValueDisplayCommand(() -> m_encoder.getAbsolutePosition(), new Point(0, 0), smallFont, Color.kBlue, m_lowerHalf)
          .ignoringDisable(true)
      );
    }
  }

  // Requires both half displays as well as the main display subsystem
  public Command fontTestCommand() {
    var smallFont = m_fontManager.getFont(Constants.Fonts.kSmallFontFile);
    var largeFont = m_fontManager.getFont(Constants.Fonts.kLargeFontFile);

    return new SequentialCommandGroup(
      new DrawRectangleCommand(new Rectangle(0, 0, m_ledSubsystem.width(), m_ledSubsystem.height()), Color.kBlack, m_ledSubsystem),
      new ParallelCommandGroup(
        new WaitCommand(2.0),
        new TextCommand("Hello", new Point(0, 1), smallFont, Color.kBlue, m_ledSubsystem)
      ),
      new ParallelCommandGroup(
        new WaitCommand(2.0),
        new TextCommand("World", new Point(0, 9), smallFont, Color.kGreen, m_ledSubsystem)
      ),
      new DrawRectangleCommand(new Rectangle(0, 0, m_ledSubsystem.width(), m_ledSubsystem.height()), Color.kBlack, m_ledSubsystem),
      new ParallelCommandGroup(
        new WaitCommand(2.0),
        new TextCommand("Janksters!!", new Point(0, 5), smallFont, Color.kRed, m_ledSubsystem)
      ),
      new ScrollingTextCommand("Watch \"Jeopardy!\", Alex Trebek's fun TV quiz game.", new Point(0, 1), 16, largeFont, Color.kBlue, m_ledSubsystem),
      new DrawRectangleCommand(new Rectangle(0, 0, m_ledSubsystem.width(), m_ledSubsystem.height()), Color.kBlack, m_ledSubsystem),
      new ParallelDeadlineGroup(
        // To have two different regions scrolling simultaneously, we need to use the upper/lower half subsystems
        // because two commands can't use the same subsystem at the same time
        new ScrollingTextCommand("Watch \"Jeopardy!\", Alex Trebek's fun TV quiz game.", new Point(0, 1), 16, smallFont, Color.kGreen, m_lowerHalf),
        new ScrollingTextCommand("The quick brown fox jumps over the lazy dog!", new Point(0, 1), 50, smallFont, Color.kRed, m_upperHalf).repeatedly()
      )
    ).repeatedly().ignoringDisable(true);
  }

  /**
   * Use this method to define your trigger->command mappings. Triggers can be created via the
   * {@link Trigger#Trigger(java.util.function.BooleanSupplier)} constructor with an arbitrary
   * predicate, or via the named factories in {@link
   * edu.wpi.first.wpilibj2.command.button.CommandGenericHID}'s subclasses for {@link
   * CommandXboxController Xbox}/{@link edu.wpi.first.wpilibj2.command.button.CommandPS4Controller
   * PS4} controllers or {@link edu.wpi.first.wpilibj2.command.button.CommandJoystick Flight
   * joysticks}.
   */
  private void configureBindings() {
    // Schedule `ExampleCommand` when `exampleCondition` changes to `true`
    // new Trigger(m_exampleSubsystem::exampleCondition)
    //     .onTrue(new ExampleCommand(m_exampleSubsystem));

    // Schedule `exampleMethodCommand` when the Xbox controller's B button is pressed,
    // cancelling on release.
    // m_joystickController.button(1).whileTrue(new TextCommand("Hello World!", m_ledSubsystem));
  }

  /**
   * Use this to pass the autonomous command to the main {@link Robot} class.
   *
   * @return the command to run in autonomous
   */
  public Command getAutonomousCommand() {
    // An example command will be run in autonomous
    return null; // Autos.exampleAuto(m_exampleSubsystem);
  }

  public void initEncoder() {
    CANCoderConfiguration config = new CANCoderConfiguration();
    config.unitString = "deg";
    config.sensorTimeBase = SensorTimeBase.PerSecond;
    m_encoder.configAllSettings(config);
    m_encoder.setStatusFramePeriod(CANCoderStatusFrame.SensorData, 10); // changes the period of the sensor data frame to 10ms
  }
}



