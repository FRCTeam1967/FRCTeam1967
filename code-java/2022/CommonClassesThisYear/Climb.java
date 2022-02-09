package org.janksters.CommonClassesThisYear;
//import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
//import edu.wpi.first.wpilibj.motorcontrol.PWMTalonFX; //doesn't have encoder methods?
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import edu.wpi.first.wpilibj.Timer;

import org.janksters.jankyLib.JankyStateMachine;

public class Climb extends JankyStateMachine{
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

    //states
    private final int Idle = 0;
    private final int LatchMidBar = 1;
    private final int WinchUp = 2;
    private final int PivotHookDown = 3;
    private final int LatchHighBar = 4;
    private final int UnlatchMidBar = 5;
    private final int WinchFinal = 6;
    private final int DoNothing = 7;

    boolean startClimbStateMachine = false;

    public Climb(int winchMotorChannelL, int winchMotorChannelR,
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
        SetName(Idle,"Idle");
        SetName(LatchMidBar,"LatchMidBar");
        SetName(WinchUp,"WinchUp");
        SetName(PivotHookDown,"PivotHookDown");
        SetName(LatchHighBar,"LatchHighBar");
        SetName(UnlatchMidBar,"UnlatchMidBar");
        SetName(WinchFinal,"WinchFinal");
        SetName(DoNothing,"DoNothing");
        
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
        //add codition for when one is wrong
        return (getWinchEncoderCountL() + getWinchEncoderCountR()) / 2;
    }

    public double getAvgWinchEncoderDistance(){ //in inches
        return getAvgWinchEncoderCount() * (PULSES_PER_REVOLUTION); //to do: figure out distance per revolution
    }

    public void startClimbStateMachine() {
        startClimbStateMachine = true;
    }

    public void StateEngine(int curState, boolean onStateEntered){
        switch (curState){
            case Idle:
                if (startClimbStateMachine){
                    NewState(LatchMidBar,"x button pressed to start sequence");
                }
                break;
            case LatchMidBar:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchMidBar();
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WinchUp,"mid bar latch timer finished");
                }
                break;
            case WinchUp:
                if (onStateEntered){
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                }
                raiseWinchString();
                if (getAvgWinchEncoderDistance()>=WINCH_UP_DISTANCE){
                    NewState(PivotHookDown,"reached full winch distance");
                }
                break;
            case PivotHookDown:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                hookPivotDown();
                if (climbTimer.get()>=HOOK_TIME){
                    climbTimer.stop();
                    NewState(LatchHighBar,"hook timer finished");
                }
                break;
            case LatchHighBar:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchHighBar();
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(UnlatchMidBar,"high bar latch timer finished");
                }
                break;
            case UnlatchMidBar:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WinchFinal,"mid bar unlatch timer finished");
                }
                break;
            case WinchFinal:
                if (onStateEntered){
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                }
                raiseWinchString();
                if (getAvgWinchEncoderDistance()>=WINCH_FINAL_DISTANCE){
                    climbTimer.stop();
                    NewState(PivotHookDown,"slightly pulling up winch finished");
                }
                break;
            case DoNothing:
                
                break;

        }
    }

    /*
    public void Run(){
        switch (caseNum){
            case 0:
                climbTimer.reset();
                climbTimer.start();
                caseNum=1;
                break;
            case 1: 
                latchMidBar(); 
                if (climbTimer.get() >= LATCH_TIME) {
                    climbTimer.stop();
                    caseNum = 2;
                }
                break;
            case 2:
                //reset winches encoder
                resetWinchEncoders();
                caseNum=3;
                break;
            case 3:
                raiseWinchString();
                if(getAvgWinchEncoderDistance()>=WINCH_UP_DISTANCE){ //inches, make 28 costat
                    caseNum=4;
                }
                break;
            case 4:
                climbTimer.reset();
                climbTimer.start();
                caseNum = 5;
                break;
            case 5:
                hookPivotDown();
                if (climbTimer.get() >= HOOK_TIME) {
                    climbTimer.stop();
                    caseNum = 6;
                }
                break;
            case 6:
                latchHighBar();
                if (climbTimer.get() >= LATCH_TIME) {
                    climbTimer.stop();
                    caseNum = 7;
                }
                break;
            case 7:
                climbTimer.reset();
                climbTimer.start();
                caseNum = 8;
                break;
            case 8:
                unlatchMidBar();
                if (climbTimer.get() >= LATCH_TIME) {
                    climbTimer.stop();
                    caseNum = 9;
                }
                break;
            case 9:
                resetWinchEncoders();
                caseNum=10;
                break;
            case 10:
                raiseWinchString();
                if(getAvgWinchEncoderDistance()>=2){ //inches
                    caseNum = 11;
                }    
                break;
            case 11:
                //do nothing - end of sequece
                break;
        }
    }
    */
    
}
