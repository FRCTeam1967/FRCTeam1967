
package org.janksters.robotcode;
import edu.wpi.first.wpilibj.Joystick;
//import edu.wpi.first.wpilibj.GenericHID;
 
 
public class jankyXboxJoystick extends Joystick {
   final static int BUTTON_A_PORT =1;
   final static int BUTTON_B_PORT =2;
   final static int BUTTON_X_PORT =3;
   final static int BUTTON_Y_PORT =4;
   final static int BUTTON_LB_PORT =5;
   final static int BUTTON_RB_PORT =6;
   final static int BUTTON_BACK_PORT =7;
   final static int BUTTON_START_PORT =8;
   final static int LEFT_STICK_BUTTON_PORT =9;
   final static int RIGHT_STICK_BUTTON_PORT =10;
   final static int LEFT_X_AXIS_CHANNEL =0;
   final static int LEFT_Y_AXIS_CHANNEL =1;
   final static int RIGHT_X_AXIS_CHANNEL=4;
   final static int RIGHT_Y_AXIS_CHANNEL =5;
 
   //public jankyXboxJoystick(int portNumber);
   public jankyXboxJoystick (int portNumber){
       super (portNumber);
 
   }
 
   //jankyXboxJoystick XboxController = new jankyXboxJoystick(2);
  
 
   public boolean GetButtonA(){
       return getRawButton(BUTTON_A_PORT);
   }
  
   public boolean GetButtonB(){
       return getRawButton(BUTTON_B_PORT);
   }
 
   public boolean GetButtonX(){
       return getRawButton(BUTTON_X_PORT);
   }
   public boolean GetButtonY(){
       if (getRawButton(BUTTON_Y_PORT)) return true;
       else return false;
   }
   public boolean GetButtonLB(){
       return getRawButton(BUTTON_LB_PORT);
   }
   public boolean GetButtonRB(){
       return getRawButton(BUTTON_RB_PORT);
   }
   public boolean GetButtonBack(){
       return getRawButton(BUTTON_BACK_PORT);
   }
   public boolean GetButtonStart(){
       return getRawButton(BUTTON_START_PORT);
   }
   public boolean GetLeftStickButton(){
       return getRawButton(LEFT_STICK_BUTTON_PORT);
   }
   public boolean GetRightStickButton(){
       return getRawButton(RIGHT_STICK_BUTTON_PORT);
   }
 
   public float GetLeftThrottle(){
       float leftThrottle = GetLeftThrottle();
       if (leftThrottle > 0.2){
           return leftThrottle;
       }
       else return 0;
   }
   public float GetRightThrottle(){
       float rightThrottle = GetRightThrottle();
       if (rightThrottle>0){
           return rightThrottle;
       }
       else return 0;
   }
   //C++ has an argument but java seems to hate that
   public float GetLeftXAxis(){
       float x = GetLeftXAxis();
       return x;
   }
   public float GetLeftYAxis(){
       float y = (GetLeftYAxis());
       return y;
   }
   public double GetRightXAxis(){
       double x= getRawAxis(RIGHT_X_AXIS_CHANNEL);
       return x;
   }
 
   public double GetRightYAxis(){
       double y=(getRawAxis(RIGHT_Y_AXIS_CHANNEL));
       return y;
   }
   public float GetLeftTwist(){
       float leftTwist = GetLeftTwist();
       return leftTwist;
   }
   public float GetRightTwist(){
       float rightTwist = GetRightTwist();
       return rightTwist;
   }
 
}
