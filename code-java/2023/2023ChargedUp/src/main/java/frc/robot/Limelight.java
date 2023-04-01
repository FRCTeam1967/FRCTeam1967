package frc.robot;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class Limelight {
    private NetworkTable limelightTable;
    private double xOffset, yOffset, leftCommand, rightCommand;

    public Limelight(){
        limelightTable = NetworkTableInstance.getDefault().getTable("limelight");
        updateValues();
    }

    /**
     * Updates X and Y offset values from network table
     */
    public void updateValues() {
        xOffset = limelightTable.getEntry("tx").getDouble(0.0);
        yOffset = limelightTable.getEntry("ty").getDouble(0.0);
    }

    /**
     * Adds camera stream and offset values to Shuffleboard
     * @param tab - Shuffleboard tab to add values to
     */
    public void configDashboard(ShuffleboardTab tab){
        tab.addCamera("Limelight Camera", "m_limelight", "http://10.19.67.11:5800/");

        tab.addDouble("Limelight xOffset", () -> limelightTable.getEntry("tx").getDouble(0.0));
        tab.addDouble("Limelight yOffset", () -> limelightTable.getEntry("ty").getDouble(0.0));
    }

    /**
     * Change pipeline from vision to driver and vice versa
     * @param isVision - whether pipeline is vision or not, boolean
     */
    public void setVisionMode(boolean isVision){
        if (isVision){
            limelightTable.getEntry("pipeline").setNumber(0);
        } else {
            limelightTable.getEntry("pipeline").setNumber(1);
        }
    }

    /**
     * If outside of error range, offset values to angle
     */
    public void alignAngle(){
        updateValues();
        
        double steeringAdjust = Constants.Vision.kP_AIMING * xOffset;
        leftCommand = 0.0;
        rightCommand = 0.0; 
        
        if (xOffset < -Constants.Vision.DEGREE_ERROR){ //if heading error is larger than allowed and is negative
            //need to turn right: left goes forward and right goes backward
            steeringAdjust -= Constants.Vision.MIN_COMMAND;
            leftCommand -= steeringAdjust;
            rightCommand += steeringAdjust;
        
        } else if (xOffset > Constants.Vision.DEGREE_ERROR){  //if heading error is larger than allowed and is positive
            //need to turn left: left goes backward and right goes forward
            steeringAdjust += Constants.Vision.MIN_COMMAND;
            leftCommand -= steeringAdjust;
            rightCommand += steeringAdjust;
        }
    }

    /**
     * NOT IN USE - update distance commands
     */
    public void adjustDistance(){
        updateValues();
        
        double distanceAdjust = Constants.Vision.kP_AIMING * yOffset;
        leftCommand = 0.0;
        rightCommand = 0.0; 

        leftCommand -= distanceAdjust;
        rightCommand -= distanceAdjust; 
    }
    
    /**
     * Get value of left command
     * @return value of leftCommand, double
     */
    public double getLeftCommand(){
        return leftCommand;
    }

     /**
     * Get value of right command
     * @return value of rightCommand, double
     */
    public double getRightCommand(){
        return rightCommand;
    }
}
