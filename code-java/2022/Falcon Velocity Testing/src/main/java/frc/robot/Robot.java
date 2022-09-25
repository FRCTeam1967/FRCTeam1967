/**
 * Phoenix Software License Agreement
 *
 * Copyright (C) Cross The Road Electronics.  All rights
 * reserved.
 * 
 * Cross The Road Electronics (CTRE) licenses to you the right to 
 * use, publish, and distribute copies of CRF (Cross The Road) firmware files (*.crf) and 
 * Phoenix Software API Libraries ONLY when in use with CTR Electronics hardware products
 * as well as the FRC roboRIO when in use in FRC Competition.
 * 
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * CROSS THE ROAD ELECTRONICS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE
 */

/**
 * Description:
 * The VelocityClosedLoop example demonstrates the velocity closed-loop servo.
 * Tested with Logitech F350 USB Gamepad inserted into Driver Station.
 * 
 * Be sure to select the correct feedback sensor using configSelectedFeedbackSensor() below.
 * Use Percent Output Mode (Holding A and using Left Joystick) to confirm talon is driving 
 * forward (Green LED on Talon/Victor) when the position sensor is moving in the positive 
 * direction. If this is not the case, flip the boolean input in setSensorPhase().
 * 
 * Controls:
 * Button 1: When held, start and run Velocity Closed Loop on Talon/Victor
 * Left Joystick Y-Axis:
 * 	+ Percent Output: Throttle Talon forward and reverse, use to confirm hardware setup
 * 	+ Velocity Closed Loop: Servo Talon forward and reverse [-500, 500] RPM
 * 
 * Gains for Velocity Closed Loop may need to be adjusted in Constants.java
 * 
 * Supported Version:
 * - Talon FX: 20.2.3.0
 */
package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Joystick;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;

import frc.robot.sim.PhysicsSim;


public class Robot extends TimedRobot {
	/* Hardware */
	WPI_TalonFX _talonL = new WPI_TalonFX(4, "rio");
	WPI_TalonFX _talonR = new WPI_TalonFX(13, "rio");
    Joystick _joy = new Joystick(0);
    
    /* String for output */
    StringBuilder _sb = new StringBuilder();
    
    /* Loop tracker for prints */
	int _loops = 0;

	public void simulationInit() {
		PhysicsSim.getInstance().addTalonFX(_talonL, 0.75, 20660);
	}
	public void simulationPeriodic() {
		PhysicsSim.getInstance().run();
	}

	public void robotInit() {
		/* Factory Default all hardware to prevent unexpected behaviour */
		_talonL.configFactoryDefault();
		_talonR.configFactoryDefault();
		
		/* Config neutral deadband to be the smallest possible */
		_talonL.configNeutralDeadband(0.001);
		_talonR.configNeutralDeadband(0.001);

		/* Config sensor used for Primary PID [Velocity] */
        _talonL.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor,
                                            Constants.kPIDLoopIdx, 
											Constants.kTimeoutMs);
        _talonR.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor,
                                            Constants.kPIDLoopIdx, 
											Constants.kTimeoutMs);
											

		/* Config the peak and nominal outputs */
		_talonL.configNominalOutputForward(0, Constants.kTimeoutMs);
		_talonL.configNominalOutputReverse(0, Constants.kTimeoutMs);
		_talonL.configPeakOutputForward(1, Constants.kTimeoutMs);
		_talonL.configPeakOutputReverse(-1, Constants.kTimeoutMs);

		_talonR.configNominalOutputForward(0, Constants.kTimeoutMs);
		_talonR.configNominalOutputReverse(0, Constants.kTimeoutMs);
		_talonR.configPeakOutputForward(1, Constants.kTimeoutMs);
		_talonR.configPeakOutputReverse(-1, Constants.kTimeoutMs);

		/* Config the Velocity closed loop gains in slot0 */
		_talonL.config_kF(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kF, Constants.kTimeoutMs);
		_talonL.config_kP(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kP, Constants.kTimeoutMs);
		_talonL.config_kI(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kI, Constants.kTimeoutMs);
		_talonL.config_kD(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kD, Constants.kTimeoutMs);

		_talonR.config_kF(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kF, Constants.kTimeoutMs);
		_talonR.config_kP(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kP, Constants.kTimeoutMs);
		_talonR.config_kI(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kI, Constants.kTimeoutMs);
		_talonR.config_kD(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kD, Constants.kTimeoutMs);
		
		_talonL.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, 200);
		_talonL.setStatusFramePeriod(StatusFrameEnhanced.Status_10_Targets, 200);
		
		_talonR.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, 200);
		_talonR.setStatusFramePeriod(StatusFrameEnhanced.Status_10_Targets, 200);

		
		
		
		/*
		 * Talon FX does not need sensor phase set for its integrated sensor
		 * This is because it will always be correct if the selected feedback device is integrated sensor (default value)
		 * and the user calls getSelectedSensor* to get the sensor's position/velocity.
		 * 
		 * https://phoenix-documentation.readthedocs.io/en/latest/ch14_MCSensor.html#sensor-phase
		 */
        // _talon.setSensorPhase(true);
	}

	/**
	 * This function is called periodically during operator control
	 */
	public void teleopPeriodic() {
		/* Get gamepad axis */
		double leftYstick = -1 * _joy.getY();

		/* Get Talon/Victor's current output percentage */
		double motorOutputL = _talonL.getMotorOutputPercent();
		double motorOutputR = _talonR.getMotorOutputPercent();
		
		/* Prepare line to print */
		_sb.append("\tout:");
		/* Cast to int to remove decimal places */
		_sb.append((int) (motorOutputL * 100));
		_sb.append("%");	// Percent

		_sb.append("\tspd:");
		_sb.append(_talonL.getSelectedSensorVelocity(Constants.kPIDLoopIdx));
		_sb.append("u"); 	// Native units

        /** 
		 * When button 1 is held, start and run Velocity Closed loop.
		 * Velocity Closed Loop is controlled by joystick position x500 RPM, [-500, 500] RPM
		 */
		if (_joy.getRawButton(1)) {
			/* Velocity Closed Loop */

			/**
			 * Convert 2000 RPM to units / 100ms.
			 * 2048 Units/Rev * 2000 RPM / 600 100ms/min in either direction:
			 * velocity setpoint is in units/100ms
			 */
			double targetVelocity_UnitsPer100ms = leftYstick * 2000.0 * 2048.0 / 600.0;
			/* 2000 RPM in either direction */
			_talonL.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
			_talonR.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms * -1);

			/* Append more signals to print when in speed mode. */
			_sb.append("\terr:");
			_sb.append(_talonL.getClosedLoopError(Constants.kPIDLoopIdx));
			_sb.append("\ttrg:");
			_sb.append(targetVelocity_UnitsPer100ms);
		} else {
			/* Percent Output */

			_talonL.set(TalonFXControlMode.PercentOutput, leftYstick);
			_talonR.set(TalonFXControlMode.PercentOutput, leftYstick * -1);
		}

        /* Print built string every 10 loops */
		if (++_loops >= 10) {
			_loops = 0;
			System.out.println(_sb.toString());
        }
        /* Reset built string */
		_sb.setLength(0);
	}
}
