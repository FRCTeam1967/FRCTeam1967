package org.janksters.CommonClassesThisYear;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_VictorSPX;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.jankyLib.JankyTask;

public class ShooterController extends JankyTask{
    private double currentEncoderCount; 
    private double desiredEncoderCount;
    private boolean run;
    private boolean manualSConveyor;
    private float offsetFromVisionTarget;

    private WPI_TalonSRX conveyorBeltMotor;
    private Flywheel flywheelmech;
    private IntakeMech intakemech;
    private WPI_VictorSPX bridgeMotor;
    private WPI_TalonSRX turretMotor; 
    
    public ShooterController(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber, int bridgeChannel, int turretChannel, int pCMChannel){
        conveyorBeltMotor = new WPI_TalonSRX(conveyorBeltMotorChannel);
        conveyorBeltMotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
        //currentEncoderCount = conveyorBeltMotor.getSensorCollection().setQuadraturePosition(0,10); 
        conveyorBeltMotor.getSensorCollection().setQuadraturePosition(0,10); 
        currentEncoderCount = conveyorBeltMotor.getSensorCollection().getQuadraturePosition(); 
        flywheelmech = new Flywheel(flywheelChannelNumber);
        intakemech = new IntakeMech(pCMChannel, motorRollChannel, leftPistonChannel, rightPistonChannel);

        bridgeMotor = new WPI_VictorSPX(bridgeChannel);
        turretMotor = new WPI_TalonSRX(turretChannel); 

        ResetEncoderCount();
        SetDesiredCount(0);
        start();
    }
    
    //Flywheel
    public void Target(){
        flywheelmech.SetRPM();
    }

    public void SetRPMAuto(){
        flywheelmech.SetRPMAuto();
    }
    
    public void FlywheelOut(){
        flywheelmech.FlywheelOut();
    }

    public void SetSelectorVisionDistance(int selectorZone){
        flywheelmech.SetSelectorVisionDistance(selectorZone);
    }

    public double GetRunningRPM(){
        return flywheelmech.GetRunningRPM();
    }

    public double GetDesiredRPM(){
        return flywheelmech.ReturnDesiredRPM();
    }
    public void StopTarget(){
        flywheelmech.StopFlywheel();
    }

    //Storage
    public double GetEncoderCount(){ //was int
        currentEncoderCount = conveyorBeltMotor.getSensorCollection().getQuadraturePosition();
        SmartDashboard.putNumber("Current Conveyor Encoder Count", currentEncoderCount);
        return -1.0 * currentEncoderCount;
    }
    public void ResetEncoderCount(){
        //currentEncoderCount = conveyorBeltMotor.getSensorCollection().setQuadraturePosition(0,10);
        conveyorBeltMotor.getSensorCollection().setQuadraturePosition(0,10);
        currentEncoderCount = conveyorBeltMotor.getSensorCollection().getQuadraturePosition();
        desiredEncoderCount = 0;
        run = false;
        manualSConveyor = true;
    }
    public void SetDesiredCount(int count){
        desiredEncoderCount = count;
        run = true;
        manualSConveyor = false;
    }

    public void GetOneBall(){
        if(GetEncoderCount() < desiredEncoderCount)
        { 
            intakemech.RollersIn();
            conveyorBeltMotor.set(1.0);
        }
        else
        {
            conveyorBeltMotor.set(0.0);
            intakemech.RollersStop();
            ResetEncoderCount();
        }
    }

    public void OuttakeOneBall(){
        if(GetEncoderCount() > desiredEncoderCount)
        { 
            intakemech.RollersOut();
            conveyorBeltMotor.set(-1.0);
        }
        else
        {
            intakemech.RollersStop();
            conveyorBeltMotor.set(0.0);
            ResetEncoderCount();
        }
    }

    public void StopConveyorBelt(){
        conveyorBeltMotor.set(0.0);
        manualSConveyor = true;
    }
    
    public void StartConveyorBelt(){
        manualSConveyor = true;
        conveyorBeltMotor.set(-0.6); //was 1.0, then 0.7
    }

    //Intake
    public void IntakeIn(){
        intakemech.RollersIn();
    }

    public void IntakeOut(){
        intakemech.RollersOut();
    }

    public void IntakeStop(){
        intakemech.RollersStop();
    }

    public void IntakePistonsDown(){
        intakemech.MechOutRobot();
    }
    public void IntakePistonsUp(){
        intakemech.MechInRobot();
    }
    public void BridgeForward(){
        bridgeMotor.set(1.0);
    }
    public void StopBridge(){
        bridgeMotor.set(0.0);
    }
    public void BridgeBackward(){
        bridgeMotor.set(-1.0);
    }

    //Turret
    public void TurretLeft(double turretSpeed){ //was float, but xBoxJoyStick returns double
        turretMotor.set(-turretSpeed);
    }
    public void TurretRight(double turretSpeed){
        turretMotor.set(turretSpeed);
    }
    public void StopTurret(){
        turretMotor.set(0.0);
    }

    //Storage
    public void Run(){ //virtual?
        if(!manualSConveyor && (GetEncoderCount() > desiredEncoderCount) && (run) && (desiredEncoderCount < 0))
        { 
            conveyorBeltMotor.set(0.7);
        }
        else if(!manualSConveyor && (GetEncoderCount() < desiredEncoderCount) && (run) && (desiredEncoderCount > 0))
        { 
            conveyorBeltMotor.set(-0.7);
        }
        else if (!manualSConveyor)
        {
            conveyorBeltMotor.set(0.0);
            ResetEncoderCount();
        } 
        SmartDashboard.putNumber("Conveyor Encoder Count", GetEncoderCount());
        SmartDashboard.putNumber("Desired Conveyor Encoder", desiredEncoderCount);
        //System.out.println("conveyor encoder: " + GetEncoderCount());
    }
}