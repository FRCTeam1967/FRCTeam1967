package frc.robot;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class Limelight {
    private NetworkTable limelightTable;
    private double xOffset, yOffset;
    
    private double leftCommand, rightCommand;

    private boolean goingLeft;

    public Limelight(){
        limelightTable = NetworkTableInstance.getDefault().getTable("limelight");
        updateValues();
    }

    public void updateValues() {
        //get x & y offset values from network table
        xOffset = limelightTable.getEntry("tx").getDouble(0.0);
        yOffset = limelightTable.getEntry("ty").getDouble(0.0);
    }

    public void configDashboard(ShuffleboardTab tab){
        //add camera view to shuffleboard
        tab.addCamera("Limelight Camera", "m_limelight", "http://10.19.67.11:5800/");

        tab.addDouble("xOffset", () -> limelightTable.getEntry("tx").getDouble(0.0));
        tab.addDouble("yOffset", () -> limelightTable.getEntry("ty").getDouble(0.0));

        tab.addDouble("kP_AIMING", () -> Constants.Vision.kP_AIMING);
        tab.addDouble("Degree Error", () -> Constants.Vision.DEGREE_ERROR);

        tab.addDouble("left command", () -> leftCommand);
        tab.addDouble("right command", () -> rightCommand);

        tab.addBoolean("limelight going left?", () -> goingLeft);
    }

    public void setVisionMode(boolean isVision){
        if (isVision){
            limelightTable.getEntry("pipeline").setNumber(0);
            //limelightTable.getEntry("camMode").setNumber(0); //CAM MODE
        } else {
            limelightTable.getEntry("pipeline").setNumber(1);
            //limelightTable.getEntry("camMode").setNumber(1); //CAM MODE
        }
    }

    //TODO possible solutions for oscillation
    //decreasing MIN_COMMAND, increasing DEGREE_ERROR 
    //keep kP small, adjust min_command first
    public void alignAngle(){
        updateValues();
        
        double steeringAdjust = Constants.Vision.kP_AIMING * xOffset;
        leftCommand = 0.0;
        rightCommand = 0.0; 
        
        if (xOffset < -Constants.Vision.DEGREE_ERROR){ //if heading error is larger than allowed and is negative
            //need to turn right: left goes forward and right goes backward
            goingLeft = false;

            steeringAdjust -= Constants.Vision.MIN_COMMAND;
            leftCommand -= steeringAdjust;
            rightCommand += steeringAdjust;
        
        } else if (xOffset > Constants.Vision.DEGREE_ERROR){  //if heading error is larger than allowed and is positive
            //need to turn left: left goes backward and right goes forward
            goingLeft = true;

            steeringAdjust += Constants.Vision.MIN_COMMAND;
            leftCommand -= steeringAdjust;
            rightCommand += steeringAdjust;
        }
    }

    public void adjustDistance(){ //not in use
        updateValues();
        
        double distanceAdjust = Constants.Vision.kP_AIMING * yOffset;
        leftCommand = 0.0;
        rightCommand = 0.0; 

        leftCommand -= distanceAdjust;
        rightCommand -= distanceAdjust; 
    }
    
    public double getLeftCommand(){
        return leftCommand;
    }

    public double getRightCommand(){
        return rightCommand;
    }
}
