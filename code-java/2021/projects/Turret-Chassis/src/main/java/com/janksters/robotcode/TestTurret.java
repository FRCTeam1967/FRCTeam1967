package com.janksters.robotcode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX; 
public class TestTurret {

    WPI_TalonSRX turretMotor;

    public TestTurret(int turretMotorChannel){
        turretMotor = new WPI_TalonSRX(turretMotorChannel); 
    }

    public void turretLeft(double turretSpeed){
        turretMotor.set(-1 * turretSpeed);
    }

    public void turretRight(double turretSpeed){
        turretMotor.set(turretSpeed);
    }

    public void stopTurret(){
        turretMotor.set(0.0);
    }

}