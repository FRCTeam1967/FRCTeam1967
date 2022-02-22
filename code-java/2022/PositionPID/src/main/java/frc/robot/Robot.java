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
 * The PositionClosedLoop example demonstrates the Position closed-loop servo.
 * Tested with Logitech F350 USB Gamepad inserted into Driver Station
 * 
 * Be sure to select the correct feedback sensor using configSelectedFeedbackSensor() below.
 * Use Percent Output Mode (Holding A and using Left Joystick) to confirm talon is driving 
 * forward (Green LED on Talon) when the position sensor is moving in the positive 
 * direction. If this is not the case, flip the boolean input in setSensorPhase().
 * 
 * Controls:
 * Button 1: When pressed, start and run Position Closed Loop on Talon
 * Button 2: When held, start and run Percent Output
 * Left Joytick Y-Axis:
 * 	+ Position Closed Loop: Servo Talon forward and reverse [-10, 10] rotations
 * 	+ Percent Ouput: Throttle Talon forward and reverse
 * 
 * Gains for Position Closed Loop may need to be adjusted in Constants.java
 * 
 * Supported Version:
 * - Talon FX: 20.2.3.0
 */
package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
//import edu.wpi.first.wpilibj.Joystick;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

//import frc.robot.sim.PhysicsSim;

public class Robot extends TimedRobot {
    /** Hardware */
	WPI_TalonFX _talon = new WPI_TalonFX(1, "rio");
	jankyXboxJoystick _joy = new jankyXboxJoystick(2);
	
    /** Used to create string thoughout loop */
	StringBuilder _sb = new StringBuilder();
	String debugString = new String();
	int _loops = 0;
	//StringBuilder is mutable, meaning when you change the value of the string it will update the value in the existing sring
    /** Track button state for single press event */
	boolean _lastButton1 = false;

	/** Save the target position to servo to */
	double targetPositionRotations;

    private final int PULSES_PER_REVOLUTIONS = 2048;

	/*
    public void simulationInit() {
		PhysicsSim.getInstance().addTalonFX(_talon, 0.5, 6800);
	}
	public void simulationPeriodic() {
		PhysicsSim.getInstance().run();
	}
    */

	public void robotInit() {
		/* Factory Default all hardware to prevent unexpected behaviour */
		_talon.configFactoryDefault();
		
		/* Config the sensor used for Primary PID and sensor direction */
        _talon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 
                                            Constants.kPIDLoopIdx,
				                            Constants.kTimeoutMs);

		/* Ensure sensor is positive when output is positive */
		_talon.setSensorPhase(Constants.kSensorPhase);

		/**
		 * Set based on what direction you want forward/positive to be.
		 * This does not affect sensor phase. 
		 */ 
		_talon.setInverted(Constants.kMotorInvert);
		/*
		 * Talon FX does not need sensor phase set for its integrated sensor
		 * This is because it will always be correct if the selected feedback device is integrated sensor (default value)
		 * and the user calls getSelectedSensor* to get the sensor's position/velocity.
		 * 
		 * https://phoenix-documentation.readthedocs.io/en/latest/ch14_MCSensor.html#sensor-phase
		 */
        // _talon.setSensorPhase(true);

		/* Config the peak and nominal outputs, 12V means full */
		_talon.configNominalOutputForward(0, Constants.kTimeoutMs);
		_talon.configNominalOutputReverse(0, Constants.kTimeoutMs);
		_talon.configPeakOutputForward(1, Constants.kTimeoutMs);
		_talon.configPeakOutputReverse(-1, Constants.kTimeoutMs);

		/**
		 * Config the allowable closed-loop error, Closed-Loop output will be
		 * neutral within this range. See Table in Section 17.2.1 for native
		 * units per rotation.
		 */
		_talon.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);

		/* Config Position Closed Loop gains in slot0, tsypically kF stays zero. */
		_talon.config_kF(Constants.kPIDLoopIdx, Constants.kGains.kF, Constants.kTimeoutMs);
		_talon.config_kP(Constants.kPIDLoopIdx, Constants.kGains.kP, Constants.kTimeoutMs);
		_talon.config_kI(Constants.kPIDLoopIdx, Constants.kGains.kI, Constants.kTimeoutMs);
		_talon.config_kD(Constants.kPIDLoopIdx, Constants.kGains.kD, Constants.kTimeoutMs);
    }
    
	void commonLoop() {
		/* Gamepad processing */
		double leftYstick = _joy.GetRightYAxis();
		boolean button1 = _joy.GetButtonX();	// X-Button
		boolean button2 = _joy.GetButtonA();	// A-Button
		boolean buttonB = _joy.GetButtonB(); 
		boolean buttonY = _joy.GetButtonY();

		/* Get Talon's current output percentage */
		double motorOutput = _talon.getMotorOutputPercent();
        SmartDashboard.putNumber("Motor Output", motorOutput);

		/* Deadband gamepad */
		if (Math.abs(leftYstick) < 0.10) {
			/* Within 10% of zero */
			leftYstick = 0;
		}

		/* Prepare line to print */
		_sb.append("\tout:");
		/* Cast to int to remove decimal places */
		_sb.append((int) (motorOutput * 100));
		_sb.append("%");	// Percent
		//debugString = "\tout:" + (int) (motorOutput * 100) + "%";

		_sb.append("\tpos:");
		_sb.append(_talon.getSelectedSensorPosition(0));
        SmartDashboard.putNumber("selected sensor position", _talon.getSelectedSensorPosition(0));
		SmartDashboard.putNumber("selected position", _talon.getSelectedSensorPosition(0)/PULSES_PER_REVOLUTIONS);
		_sb.append("u"); 	// Native units
		//debugString = stringName + "\tpos:" + _talon.getSelectedSensorPosition(0) + "selected sensor position", _talon.getSelectedSensorPosition(0) + "u";

		/**
		 * When button 1 is pressed, perform Position Closed Loop to selected position,
		 * indicated by Joystick position x10, [-10, 10] rotations
		 */
		if (!_lastButton1 && button1) {
			/* Position Closed Loop */

			/* 10 Rotations * 2048 u/rev in either direction */
			/*
			targetPositionRotations = leftYstick * 10.0 * PULSES_PER_REVOLUTIONS;
			_talon.set(TalonFXControlMode.Position, targetPositionRotations);
			SmartDashboard.putNumber("Position: ", targetPositionRotations);
			*/
			targetPositionRotations = 0;
			_talon.set(TalonFXControlMode.Position, targetPositionRotations);
		} else if (button2) { /* When button 2 is held, just straight drive */
			/* Percent Output */
			_talon.set(TalonFXControlMode.PercentOutput, leftYstick);
		} else if(buttonB){
			targetPositionRotations = 50 * PULSES_PER_REVOLUTIONS;
			_talon.set(TalonFXControlMode.Position, targetPositionRotations);
		} else if(buttonY){
			targetPositionRotations = 200 * PULSES_PER_REVOLUTIONS;
			_talon.set(TalonFXControlMode.Position, targetPositionRotations);
		}
		else if (_joy.GetButtonStart()){
			_talon.set(0);
		}
		SmartDashboard.putNumber("Target Rotation Position: ", targetPositionRotations);
		SmartDashboard.putNumber("Target Position: ", targetPositionRotations/PULSES_PER_REVOLUTIONS);

		/* If Talon is in position closed-loop, print some more info */
		if (_talon.getControlMode() == TalonFXControlMode.Position.toControlMode()) {
			/* ppend more signals to print when in speed mode. */
			_sb.append("\terr:");
			_sb.append(_talon.getClosedLoopError(0));
			_sb.append("u");	// Native Units
			//debugString = debugString + "\terr:" + _talon.getClosedLoopError(0) + "u";

			_sb.append("\ttrg:");
			_sb.append(targetPositionRotations);
			_sb.append("u");	/// Native Units
			//debugString = debugString + "\ttrg:" + targetPositionRotations + "u";
		}

		/**
		 * Print every ten loops, printing too much too fast is generally bad
		 * for performance.
		 */
		if (++_loops >= 10) {
			_loops = 0;
			System.out.println(_sb.toString());
			//System.out.println(debugString);
		}

		/* Reset built string for next loop */
		_sb.setLength(0);
		//debugString = "";
		
		/* Save button state for on press detect */
		_lastButton1 = button1;
    }
    
	/**
	 * This function is called periodically during operator control
	 */
	public void teleopPeriodic() {
		commonLoop();
	}
}