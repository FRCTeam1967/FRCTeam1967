//climbing code with manual stop button options in state machine
package org.janksters.CommonClassesThisYear;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.jankyLib.*;

public class ClimbMBR extends JankyStateMachine{
    private JankyTalonFX winchMotorL, winchMotorR;

    private jankyXboxJoystick XboxController;
    private double rightYAxis;

    //states
    private final int IDLE = 0, LIFTER_DOWN = 1, MANUAL_ARM = 2, MANUAL_ROBOT = 3;
    // private final int LIFTER_DOWN = 1;
    // private final int MANUAL_ARM = 2;
    // private final int MANUAL_ROBOT = 3;

    public ClimbMBR(int winchMotorChannelL, int winchMotorChannelR){
        winchMotorL = new JankyTalonFX(winchMotorChannelL);
        winchMotorR = new JankyTalonFX(winchMotorChannelR);
        XboxController = new jankyXboxJoystick(2);
        
        singleWinchMotorSetUp(winchMotorL);
        singleWinchMotorSetUp(winchMotorR);

        SetMachineName("JankyStateMachineClimb");

        SetName(IDLE,"Idle");
        // SetName(LIFTER_DOWN, "LifterDown");
        SetName(MANUAL_ARM, "ManualArm");
        SetName(MANUAL_ROBOT, "ManualRobot");

        start();
    }

    private void singleWinchMotorSetUp(JankyTalonFX motor){
        motor.configFactoryDefault();
        motor.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_K_TIMEOUT_MS);
        motor.setInverted(TalonFXInvertType.Clockwise);
        motor.configNominalOutputForward(0, Constants.CLIMB_K_TIMEOUT_MS);
        motor.configNominalOutputReverse(0, Constants.CLIMB_K_TIMEOUT_MS);
        motor.configPeakOutputForward(1, Constants.CLIMB_K_TIMEOUT_MS);
        motor.configPeakOutputReverse(-1, Constants.CLIMB_K_TIMEOUT_MS);
        motor.setNeutralMode(NeutralMode.Brake);
    }
    
    private void moveWinchString(double winchSpeed){
        winchMotorL.set(winchSpeed);
        winchMotorR.set((-1.0)*winchSpeed);
    }

    private void stopWinchString(){ 
        winchMotorL.set(Constants.CLIMB_WINCH_MOTOR_STOP);
        winchMotorR.set((-1.0)*Constants.CLIMB_WINCH_MOTOR_STOP);
    }

    public void killStateMachine(){
        Terminate();
    }

    public boolean isInIdle(){
        return GetCurrentState() == IDLE;
    }

    public void StateEngine(int curState, boolean onStateEntered){
        SmartDashboard.putNumber("climb state", GetCurrentState());
        switch (curState){
            case IDLE:
                if (XboxController.GetRightStickButton()){
                    NewState(MANUAL_ARM, "right stick button was pressed");
                }
                break;

            case LIFTER_DOWN:
                //pivot.flagClimbConfig();
                //if(pivot.checkClimbConfigAchieved()){
                    NewState(MANUAL_ARM, "lifter is down");
                //}
                break;

            case MANUAL_ARM:
                if (onStateEntered){
                    stopWinchString();
                }

                rightYAxis = XboxController.GetRightYAxis();
                if (Math.abs(rightYAxis) > Constants.CLIMB_DEADBAND) {
                    moveWinchString(Constants.CLIMB_MBR_WINCH_ARM_FACTOR * rightYAxis);
                } else {
                    stopWinchString();
                }

                if (XboxController.GetButtonY()) { //changed from button x to y
                    NewState(MANUAL_ROBOT, "going to lift up the robot");
                }
                break;

            case MANUAL_ROBOT:
                rightYAxis = XboxController.GetRightYAxis();
                if (Math.abs(rightYAxis) > Constants.CLIMB_DEADBAND) {
                    moveWinchString(Constants.CLIMB_MBR_WINCH_ROBOT_FACTOR * rightYAxis);
                } else {
                    stopWinchString();
                }
                break;
        }
    }
}
