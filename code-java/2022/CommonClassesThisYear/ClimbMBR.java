//climbing code with manual stop button options in state machine
package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
// import java.util.Set;
import com.ctre.phoenix.ErrorCode;
import com.ctre.phoenix.motorcontrol.NeutralMode;
//import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import org.janksters.jankyLib.*;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;

public class ClimbMBR extends JankyStateMachine{
    private JankyTalonFX winchMotorL; //left
    private JankyTalonFX winchMotorR; //right

    private jankyXboxJoystick XboxController;
    private PivotMagic pivot;
    private LED led;

    //states
    private final int IDLE = 0; //keep bar down
    private final int LIFTER_DOWN = 1;
    private final int MANUAL_ARM = 2;
    private final int MANUAL_ROBOT = 3;

    public ClimbMBR(int winchMotorChannelL, int winchMotorChannelR, PivotMagic _pivot, LED _led){
        winchMotorL = new JankyTalonFX(winchMotorChannelL);
        winchMotorR = new JankyTalonFX(winchMotorChannelR);
        XboxController= new jankyXboxJoystick(2);
        pivot = _pivot;
        led = _led;

        setUpWinchBrake();

        SetMachineName("JankyStateMachineClimb");

        SetName(IDLE,"Idle");
        SetName(LIFTER_DOWN, "LifterDown");
        SetName(MANUAL_ARM, "ManualArm");
        SetName(MANUAL_ROBOT, "ManualRobot");
        
        start();
    } 

     private void setUpWinchBrake(){
        winchMotorL.setNeutralMode(NeutralMode.Brake);
        winchMotorR.setNeutralMode(NeutralMode.Brake);
    }

    private void moveWinchString(double winchSpeed){
        winchMotorL.set(winchSpeed);
        winchMotorR.set(winchSpeed);
    }

    private void stopWinchString(){ //make sure this is when you want to hold, not actually stop
        winchMotorL.set(Constants.CLIMB_WINCH_MOTOR_STOP);
        winchMotorR.set(Constants.CLIMB_WINCH_MOTOR_STOP);
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
                    NewState(LIFTER_DOWN, "right stick button was pressed");
                }
                break;

            case LIFTER_DOWN:
                //pivot.flagClimbConfig(); //TODO: uncomment once we want to use lifter
                //if(!testing && pivot.checkClimbConfigAchieved()){
                    NewState(READY_TO_CLIMB, "lifter is down");
                //}
                break;
                
            case MANUAL_ARM:
                if (onStateEntered){
                    stopWinchString();
                    led.setColor(0, 255, 0); //makes it green to show that we are in arm speed
                    //TODO: make sure other groups don't use at this point
                }
                
                double rightYAxis = XboxController.GetRightYAxis();
                if (Math.abs(rightYAxis) > Constants.CLIMB_DEADBAND) {
                    moveWinchString(Constants.CLIMB_MBR_WINCH_ARM_FACTOR * rightYAxis);
                } else {
                    stopWinchString();
                }

                if (XboxController.GetButtonX()) {
                    NewState(MANUAL_ROBOT, "going to lift up the robot");
                }
                break;
            
            case MANUAL_ROBOT:
                if(onStateEntered){
                    led.setColor(255, 0, 0); //makes it red
                }

                //joystick
                double rightYAxis = XboxController.GetRightYAxis();
                if (Math.abs(rightYAxis) > Constants.CLIMB_DEADBAND) {
                    moveWinchString(Constants.CLIMB_MBR_WINCH_ROBOT_FACTOR * rightYAxis);
                } else {
                    stopWinchString();
                }

                break;
        }
    }  
}