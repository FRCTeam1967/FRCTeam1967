// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.hal.PowerDistributionFaults;
import edu.wpi.first.hal.PowerDistributionStickyFaults;
import edu.wpi.first.hal.PowerDistributionVersion;
import edu.wpi.first.wpilibj.PowerDistribution;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import java.lang.reflect.InvocationTargetException;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private static final String kDefaultAuto = "Default";
  private static final String kCustomAuto = "My Auto";
  private String m_autoSelected;
  private final SendableChooser<String> m_chooser = new SendableChooser<>();

  private static final boolean shouldClearStickyFaults = false;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    PowerDistribution pdp = new PowerDistribution(0, PowerDistribution.ModuleType.kCTRE);
    PowerDistributionStickyFaults stickyFaults = pdp.getStickyFaults();
    logStickyFaults(stickyFaults);
    if (shouldClearStickyFaults) {
      System.out.println("CLEARING STICKY FAULTS");
      pdp.clearStickyFaults();
    } else {
      System.out.println("NOT clearing sticky faults (if any)");
    }
    PowerDistributionFaults faults = pdp.getFaults();
    logFaults(faults);

    PowerDistributionVersion version = pdp.getVersion();
    logVersion(version);

    pdp.close();
  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {}

  /**
   * This autonomous (along with the chooser code above) shows how to select between different
   * autonomous modes using the dashboard. The sendable chooser code works with the Java
   * SmartDashboard. If you prefer the LabVIEW Dashboard, remove all of the chooser code and
   * uncomment the getString line to get the auto name from the text box below the Gyro
   *
   * <p>You can add additional auto modes by adding additional comparisons to the switch structure
   * below with additional strings. If using the SendableChooser make sure to add them to the
   * chooser code above as well.
   */
  @Override
  public void autonomousInit() {
    m_autoSelected = m_chooser.getSelected();
    // m_autoSelected = SmartDashboard.getString("Auto Selector", kDefaultAuto);
    System.out.println("Auto selected: " + m_autoSelected);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    switch (m_autoSelected) {
      case kCustomAuto:
        // Put custom auto code here
        break;
      case kDefaultAuto:
      default:
        // Put default auto code here
        break;
    }
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {}

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {}

  /** This function is called periodically when disabled. */
  @Override
  public void disabledPeriodic() {}

  /** This function is called once when test mode is enabled. */
  @Override
  public void testInit() {}

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}

  void logStickyFaults(PowerDistributionStickyFaults stickyFaults) {
    if (stickyFaults == null) return;

    String logs = "";
    if (stickyFaults.Brownout) {
      logs += "BROWNOUT detected\n";
    }
    if (stickyFaults.CanBusOff) {
      logs += "CANbus OFF detected\n";
    }
    if (stickyFaults.CanWarning) {
      logs += "CANbus warning detected\n";
    }
    if (stickyFaults.HasReset) {
      logs += "Has Reset is set\n";
    }

    // All the fields are named ChannelXXBreakerFault. Use Java hackery to generate all the accesses
    // to the various fields because I'm lazy.
    for (int channel = 0; channel < 24; channel++) {
      String fieldName = "Channel" + channel + "BreakerFault";
      try {
        java.lang.reflect.Field field = stickyFaults.getClass().getField(fieldName);
        Boolean hasFault = field.getBoolean(stickyFaults);
        if (hasFault) {
          logs += fieldName + " detected";
        }
      } 
      catch (SecurityException e) {}
      catch (NoSuchFieldException e) {
        System.out.println("No such field?! -- " + fieldName);
      }
      catch (IllegalArgumentException e) {}
      catch (IllegalAccessException e) {}
    }

    if (logs.length() > 0) {
      System.out.println("PDP HAS STICKY FAULTS:");
      System.out.println(logs);
    } else {
      System.out.println("No sticky faults detected.");
    }
  }

  void logFaults(PowerDistributionFaults faults) {
    if (faults == null) return;

    String logs = "";

    // All the fields are named ChannelXXBreakerFault. Use Java hackery to generate all the accesses
    // to the various fields because I'm lazy.
    for (int channel = 0; channel < 24; channel++) {
      String fieldName = "Channel" + channel + "BreakerFault";
      try {
        java.lang.reflect.Field field = faults.getClass().getField(fieldName);
        Boolean hasFault = field.getBoolean(faults);
        if (hasFault) {
          logs += fieldName + " detected";
        }
      } 
      catch (SecurityException e) {}
      catch (NoSuchFieldException e) {
        System.out.println("No such field?! -- " + fieldName);
      }
      catch (IllegalArgumentException e) {}
      catch (IllegalAccessException e) {}
    }

    if (faults.HardwareFault) {
      logs += "HARDWARE FAULT detected\n";
    }

    if (logs.length() > 0) {
      System.out.println("PDP HAS ACTIVE FAULTS:");
      System.out.println(logs);
    } else {
      System.out.println("No active faults detected.");
    }
  }

  void logVersion(PowerDistributionVersion version) {
    // My PDP always reports zeroes. Maybe this only works for the RevRobotics PDH?
    System.out.printf("PDP firmware version: %d.%d.%d\n", version.firmwareMajor, version.firmwareMinor, version.firmwareFix);
    System.out.printf("PDP hardware version: %d.%d\n", version.hardwareMajor, version.hardwareMinor);
    System.out.println(version);
  }
}