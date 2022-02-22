package org.janksters.CommonClassesThisYear;

import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
//import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
//import com.ctre.phoenix.motorcontrol.TalonFXSensorCollection;
//import com.ctre.phoenix.motorcontrol.TalonSRXFeedbackDevice;
//import com.ctre.phoenix.motorcontrol.can.BaseTalon;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

public class Flywheel {
    private WPI_TalonFX flywheelMotor;
    private int kSlotIdx;
    private int kPIDLoopIdx;
    private int kTimeoutMs;
    private double desiredRPM;
    private double runningRPM;
    private double reverseRPM;
    private double targetVelocity_UnitsPer100ms;
    private double distanceToVisionTarget;

    private String visionDistance = "Distance to Tape";
    private int NoDataDefault = -1000;
    
    private int shootingZoneOne = 1;
    private int shootingZoneTwo = 2;
    private int shootingZoneThree = 3;
    private int shootingZoneFour = 4;


    public Flywheel(int flywheelChannelNumber){
        kSlotIdx = 0;
        kPIDLoopIdx = 0;
        kTimeoutMs = 30;
        flywheelMotor = new WPI_TalonFX(flywheelChannelNumber);
        flywheelMotor.configFactoryDefault();
        flywheelMotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Relative, 0, kTimeoutMs);
        //flywheelMotor.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, 0, kTimeoutMs);
        flywheelMotor.setSensorPhase(true);
        flywheelMotor.configNominalOutputForward(0, kTimeoutMs);
        flywheelMotor.configNominalOutputReverse(0, kTimeoutMs); //0 is the minimum percent so it does turn off completely - it is the percent of motor output
        flywheelMotor.configPeakOutputForward(1, kTimeoutMs);
        flywheelMotor.configPeakOutputReverse(-1, kTimeoutMs);
        flywheelMotor.config_kF(kPIDLoopIdx, 0.045, kTimeoutMs); //what's an f constant??, it measures how much motor output to change to get velocity
        flywheelMotor.config_kP(kPIDLoopIdx, 0.0012, kTimeoutMs);
        flywheelMotor.config_kI(kPIDLoopIdx, 0.0, kTimeoutMs);
        flywheelMotor.config_kD(kPIDLoopIdx, 0.0001, kTimeoutMs);

        SmartDashboard.putNumber(visionDistance, NoDataDefault);
    }

    public void CalculateDistanceToVisionTarget() {
        distanceToVisionTarget = SmartDashboard.getNumber(visionDistance, NoDataDefault); //was commented out before? we never call so it doesn't get data
    }

    public double GetDistanceToVisionTarget(){
        return distanceToVisionTarget;
    }

    public void SetRPM(){
        //desiredRPM = (5.4545 * distanceToVisionTarget) + 4000;
        desiredRPM = (5.4545 * distanceToVisionTarget) + 1000; //for testing range
        // desiredRPM = (3.7272 * distanceToVisionTarget) + 5500; //when using vision
        targetVelocity_UnitsPer100ms = desiredRPM * 2048 / 600;

        SmartDashboard.putNumber("Java - desiredRPM", desiredRPM);
        flywheelMotor.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
        //flywheelMotor.set(0.2);
        SmartDashboard.putBoolean("Flywheel is Alive", flywheelMotor.isAlive());
        

        SmartDashboard.putNumber("Java - shooting zone number final", distanceToVisionTarget);
        SmartDashboard.putNumber("Java - target velocity final", targetVelocity_UnitsPer100ms);
    }

    public void SetRPMAuto(){
        desiredRPM = 5000; //note: before turret angle changed for 2021 was 6000, was 53000 for first 5 calgame matches
        targetVelocity_UnitsPer100ms = desiredRPM * 2048/600;
        flywheelMotor.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
    }

    public double ReturnDesiredRPM(){
        return desiredRPM;
    }

    public double GetRunningRPM(){
        runningRPM = (flywheelMotor.getSelectedSensorVelocity(kPIDLoopIdx)) / 2048 * 600;
        SmartDashboard.putNumber("RPM flywheel is running at", runningRPM);
        return runningRPM;
    }
    
    public void StopFlywheel(){
        flywheelMotor.set(ControlMode.Velocity, 0);
    }

    public void FlywheelOut(){
        reverseRPM = -3000; //test
        targetVelocity_UnitsPer100ms = reverseRPM * 2048 / 600;
        flywheelMotor.set(ControlMode.Velocity, reverseRPM);
    }

    public void SetSelectorVisionDistance(int selectorZone){
        if (selectorZone == shootingZoneOne) {
            distanceToVisionTarget = 0; //in inches //10 for range, 265 for normal
        } else if (selectorZone == shootingZoneTwo) {
            distanceToVisionTarget = 10; //250 for range(good speed for 2022), 100 for normal
        } else if (selectorZone == shootingZoneThree) {
            distanceToVisionTarget = 50; //500 for range, 150 for normal
        } else if (selectorZone == shootingZoneFour) {
            distanceToVisionTarget = 200; //1000 for range, 250 for normal
        } else {
            System.out.println("No distance to vision target!");
        }

        SmartDashboard.putNumber("Java-Manual Distance To Vision Target: ", distanceToVisionTarget);
    }

    public double getFlywheelEncoder(){ //?? Tiffany's group including Tiffany isn't sure?
        return flywheelMotor.getSensorCollection().getIntegratedSensorPosition();
    }

}
