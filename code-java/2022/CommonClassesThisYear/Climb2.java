//climbing code with manual stop button options in state machine

package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.jankyLib.JankyStateMachine;

public class Climb2 extends JankyStateMachine{
    private WPI_TalonFX winchMotorL; //left
    private WPI_TalonFX winchMotorR; //right
    
    private Solenoid midBarLatchL;
    private Solenoid midBarLatchR;
    private Solenoid highBarLatchL;
    private Solenoid highBarLatchR;

    private Solenoid pivotHookSwingL;
    private Solenoid pivotHookSwingR;

    private Timer climbTimer;

    //constants
    private final double WINCH_MOTOR_SPEED = 0.3;
    private final double WINCH_MOTOR_STOP = 0.0;
    private final double PULSES_PER_REVOLUTION = 2048; //2048 for falcon integrated, 4096 for MAG
    private final double LATCH_TIME = 1.0; //check value - in seconds
    private final double HOOK_TIME = 1.0; //check value - in seconds
    private final double WINCH_UP_DISTANCE = 28; //in inches
    private final int WINCH_FINAL_DISTANCE = 2; //in inches
    private final int kTimeoutMs = 1;
    private final double WINCH_ENCODER_DIFFERENCE = 2000;
    private final double WINCH_ENCODER_DOWN = 1000;

    //states
    private final int IDLE = 0;
    private final int LATCH_MID_BAR = 1;
    private final int WINCH_UP = 2;
    private final int PIVOT_HOOK_DOWN = 3;
    private final int LATCH_HIGH_BAR = 4;
    private final int UNLATCH_MID_BAR = 5;
    private final int WINCH_FINAL = 6;
    private final int DO_NOTHING = 7;
    private final int UNLATCH_HIGH_BAR = 8;
    private final int WINCH_DOWN = 9;
    private final int NEW_UNLATCH_MID_BAR = 10;

    boolean startClimbStateMachine = false;
    boolean stopClimbStateMachine = false;

    public Climb2(int winchMotorChannelL, int winchMotorChannelR,
    int PCMChannel, int midLatchChannelL, int midLatchChannelR,
    int highLatchChannelL, int highLatchChannelR,
    int pivotHookChannelL, int pivotHookChannelR){
        winchMotorL = new WPI_TalonFX(winchMotorChannelL);
        winchMotorR = new WPI_TalonFX(winchMotorChannelR);

        midBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelL);
        midBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelR);
        highBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, highLatchChannelL);
        highBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, highLatchChannelR);

        pivotHookSwingL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, pivotHookChannelL);
        pivotHookSwingR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, pivotHookChannelR);
        climbTimer = new Timer();

        climbTimer = new Timer();
        SetMachineName("JankyStateMachineClimb");
        SetName(IDLE,"Idle");
        SetName(LATCH_MID_BAR,"LatchMidBar");
        SetName(WINCH_UP,"WinchUp");
        SetName(PIVOT_HOOK_DOWN,"PivotHookDown");
        SetName(LATCH_HIGH_BAR,"LatchHighBar");
        SetName(UNLATCH_MID_BAR,"UnlatchMidBar");
        SetName(WINCH_FINAL,"WinchFinal");
        SetName(DO_NOTHING,"DoNothing");
        SetName(UNLATCH_HIGH_BAR,"UnlatchHighBar");
        SetName(WINCH_DOWN,"WinchDown");
        SetName(NEW_UNLATCH_MID_BAR, "NewUnlatchMidBar");
        
        start();
    } 

    public void raiseWinchString(){
        winchMotorL.set(WINCH_MOTOR_SPEED);
        winchMotorR.set(WINCH_MOTOR_SPEED);
    }

    public void lowerWinchString(){
        winchMotorL.set(-1.0 * WINCH_MOTOR_SPEED);
        winchMotorR.set(-1.0 * WINCH_MOTOR_SPEED);
    }

    public void stopWinchString(){
        winchMotorL.set(WINCH_MOTOR_STOP);
        winchMotorR.set(WINCH_MOTOR_STOP);
    }

    public void latchMidBar(){
        midBarLatchL.set(true);
        midBarLatchR.set(true);
    }

    public void unlatchMidBar(){
        midBarLatchL.set(false);
        midBarLatchR.set(false);
    }

    public void latchHighBar(){
        highBarLatchL.set(true);
        highBarLatchR.set(true);
    }

    public void unlatchHighBar(){
        highBarLatchL.set(false);
        highBarLatchR.set(false);
    }

    public void hookPivotDown(){
        pivotHookSwingL.set(true);
        pivotHookSwingR.set(true);
    }

    public void hookPivotUp(){
        pivotHookSwingL.set(false);
        pivotHookSwingR.set(false);
    }

    public void setUpWinchEncoder(){
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, 0, kTimeoutMs);
        winchMotorL.setSensorPhase(true);

        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, 0, kTimeoutMs);
        winchMotorR.setSensorPhase(true);
    }

    public double getWinchEncoderCountL(){
        return winchMotorL.getSensorCollection().getIntegratedSensorPosition();
    }

    public double getWinchEncoderCountR(){
        return winchMotorR.getSensorCollection().getIntegratedSensorPosition();
    }

    public void resetWinchEncoders(){
        winchMotorL.getSensorCollection().setIntegratedSensorPosition(0, 10);
        winchMotorR.getSensorCollection().setIntegratedSensorPosition(0, 10);
    }

    public double getAvgWinchEncoderCount(){
        //condition for when one is wrong
        if (Math.abs(getWinchEncoderCountL() - getWinchEncoderCountR()) > WINCH_ENCODER_DIFFERENCE) { //change value later
            SmartDashboard.putBoolean("climb winch encoders working?", false);
        } else {
            SmartDashboard.putBoolean("climb winch encoders working?", true);
        }

        return (getWinchEncoderCountL() + getWinchEncoderCountR()) / 2;
    }

    public double getAvgWinchEncoderDistance(){ //in inches
        return getAvgWinchEncoderCount() * (PULSES_PER_REVOLUTION); //to do: figure out distance per revolution
    }

    public void startClimbStateMachine() {
        startClimbStateMachine = true;
    }

    public void stopClimbStateMachine(){
        stopClimbStateMachine = true;
    }

    public void StateEngine(int curState, boolean onStateEntered){
        switch (curState){
            case IDLE:
                stopClimbStateMachine = false;
                if (startClimbStateMachine){
                    NewState(LATCH_MID_BAR,"x button pressed to start sequence");
                }
                break;
            case LATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchMidBar();
                if (stopClimbStateMachine) {  //stop button pressed
                    NewState(NEW_UNLATCH_MID_BAR,"no longer going to winch up");
                }
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WINCH_UP,"mid bar latch timer finished");
                }
                break;
            case NEW_UNLATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(IDLE,"done with unlatching after stop button");
                }
                break;
            case WINCH_UP:
                if (onStateEntered){
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                }
                raiseWinchString();
                if (stopClimbStateMachine) {  //stop button pressed
                    NewState(WINCH_DOWN,"no longer going to winch up");
                }
                if (getAvgWinchEncoderDistance()>=WINCH_UP_DISTANCE){
                    NewState(PIVOT_HOOK_DOWN,"reached full winch distance");
                }
                break;
            case WINCH_DOWN:
                lowerWinchString();
                if (stopClimbStateMachine) {
                    NewState(NEW_UNLATCH_MID_BAR,"done winching down");
                }
                if (getAvgWinchEncoderDistance() == WINCH_ENCODER_DOWN){
                    NewState(PIVOT_HOOK_DOWN,"reached full winch distance");
                }
                break;
            case PIVOT_HOOK_DOWN:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                hookPivotDown();
                if (stopClimbStateMachine) {
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (climbTimer.get()>=HOOK_TIME){
                    climbTimer.stop();
                    NewState(LATCH_HIGH_BAR,"hook timer finished");
                }
                break;
            case LATCH_HIGH_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchHighBar();
                if (stopClimbStateMachine) {
                    NewState(UNLATCH_HIGH_BAR,"stopping sequence so need to unlatch");
                }
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(UNLATCH_MID_BAR,"high bar latch timer finished");
                }
                break;
            case UNLATCH_HIGH_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchHighBar();
                if (climbTimer.get()>=LATCH_TIME) {
                    NewState(DO_NOTHING,"stopping sequence - done unlatching");
                } 
            case UNLATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (stopClimbStateMachine) {
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WINCH_FINAL,"mid bar unlatch timer finished");
                }
                break;
            case WINCH_FINAL:
                if (onStateEntered){
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                }
                raiseWinchString();
                if (stopClimbStateMachine) {
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getAvgWinchEncoderDistance()>=WINCH_FINAL_DISTANCE){
                    climbTimer.stop();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                }
                break;
            case DO_NOTHING:
                break;

        }
    }
    
}
