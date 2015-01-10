///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Shooter for Rebound Rumble 2012

#include "jankyShooter.h"

JankyShooter::JankyShooter(int JagPort, int EncoderAPort, int EncoderBPort):
	ShooterMotor(JagPort),
	ShooterEncoder(EncoderAPort,EncoderBPort,false,ShooterEncoder.k1X),
	PID(0.02,0.03,0.01,&ShooterEncoder,&ShooterMotor)
{
	RPMerrorOld = 0;
	ShooterMotor.Set(0.0);
	TargetRPMx4 = 0;
	CurrentRPMx4 = 0;
	MotorSpeed = 0;
	PID.SetInputRange(0.0,20000.0);
	PID.SetOutputRange(0.0,1.0);
	PID.SetSetpoint(0.0);
//	PID.Enable();
	EncoderTimer.Reset();
	EncoderTimer.Start();
	ShooterTimer.Reset();
	ShooterTimer.Start();
	PreviousCount=0;
	PreviousTime=0;
	PreviousRPM=0;
	SmartDashboard::PutNumber("Delta Time", -1.0);
	SmartDashboard::PutNumber("Delta Count", -1);
	GetCurrentRPM();
	SmartDashboard::PutString("P-shooter", "0.000006");
	SmartDashboard::PutString("D-shooter", "0.0001");
}

JankyShooter::~JankyShooter(void)
{
}

int JankyShooter::GetCurrentRPM(void)
{
	
	int CurrentCount= ShooterEncoder.Get();
	
	double CurrentTime= EncoderTimer.Get();
	
	long CurrentRPM= ((long)((float)(CurrentCount-PreviousCount)/(CurrentTime-PreviousTime)))/6;
	
	//current count (tick/sec)-need to convert-RPM-x60 sec, /360 ticks = (1/6)
	
	CurrentRPM = (PreviousRPM * 15 + CurrentRPM) / 16;
	
	double DeltaTime=CurrentTime-PreviousTime;
	
	int DeltaCount=CurrentCount-PreviousCount;
	
	if (CurrentCount<PreviousCount || DeltaCount>10000)
	{
		PreviousCount=CurrentCount;
		PreviousTime=CurrentTime;
		
		return PreviousRPM;
	}
	if (CurrentRPM>10000 || CurrentRPM<=0)
	{
		if (CurrentRPM!=0)
		{	
		SmartDashboard::PutNumber("Delta Time", DeltaTime);
		SmartDashboard::PutNumber("Delta Count", DeltaCount);
		
		}
		return PreviousRPM;
	}
	else
	{	
		PreviousCount=CurrentCount;
	
		PreviousTime=CurrentTime;
		
		PreviousRPM=(int)CurrentRPM;
	
		SmartDashboard::PutNumber("Current Encoder Value", CurrentCount);
		if (CurrentRPM > 10000 || CurrentRPM < 0)
			SmartDashboard::PutString("Test", "Help!");
		
		SmartDashboard::PutNumber("Current RPM",(int)CurrentRPM);
		
	
		return (int)CurrentRPM;
	}	
}
void JankyShooter::setTargetRPM(int desiredrpm)
{
	
	TargetRPMx4 = desiredrpm;
//	PID.SetSetpoint(TargetRPMx4);
//	SmartDashboard::GetInstance()->PutDouble("PIDOutput",PID.m_pidOutput);
	
	if (ShooterTimer.Get()>MINSHOOT_WAIT)
	{	
		DoCalculations();
		ShooterTimer.Reset();
	}
	
}

void JankyShooter::DoCalculations(void) //adjust RPM
{
	/*
	float proportion;
	std::string tempstring;

	tempstring=SmartDashboard::GetInstance()->GetString("P-shooter");
	sscanf(tempstring.c_str(), "%f", &proportion);
	
	int suspectRPM = (int)GetCurrentRPM();
	
	int deadband= (int)0.1*TargetRPMx4;
	
	//filtering out noise from the encoder 
	if (suspectRPM < -10000 || suspectRPM > 10000)
		return;
	
	CurrentRPMx4 = suspectRPM;
		
	double RPMerror = (CurrentRPMx4 - TargetRPMx4);
	double ChangeSpeed = proportion*RPMerror;  

	
	
	if (TargetRPMx4 < 1300 && TargetRPMx4 !=0)
	{
		MotorSpeed = 0.15;
	}
	else if (TargetRPMx4 == 0)
		MotorSpeed = 0;
	else if (TargetRPMx4 > CurrentRPMx4 + deadband || TargetRPMx4 < CurrentRPMx4 - deadband)
	{
		MotorSpeed = MotorSpeed - ChangeSpeed;
	}
	if (MotorSpeed > 1.0)
		MotorSpeed = 1.0;
	
	if (MotorSpeed < 0.0)
		MotorSpeed = 0.0;
	
	SmartDashboard::GetInstance()->PutInt("CurrentRPM",CurrentRPMx4);
	SmartDashboard::GetInstance()->PutDouble("Motor Speed",MotorSpeed);
	//SmartDashboard::GetInstance()->PutDouble("RPMError",RPMerror);
	//SmartDashboard::GetInstance()->PutDouble("Change in Speed",ChangeSpeed);
				
	ShooterMotor.Set(MotorSpeed);
	

*/
	float proportion;
	std::string tempstring;

	tempstring=SmartDashboard::GetString("P-shooter");
	sscanf(tempstring.c_str(), "%f", &proportion);
	
	//derivative variable-added start-4.6.12 SUPERAWESOME

	float derivative;
	std::string tempstring2;

	tempstring2=SmartDashboard::GetString("D-shooter");
	sscanf(tempstring2.c_str(), "%f", &derivative);

	//derivative-added done
	
	
						
	int suspectRPM = (int)GetCurrentRPM();
	
	int deadband= (int)0.1*TargetRPMx4;
	
	//filtering out noise from the encoder 
	if (suspectRPM < -10000 || suspectRPM > 10000)
		return;
	
	CurrentRPMx4 = suspectRPM;
		
	double RPMerror = (CurrentRPMx4 - TargetRPMx4);
	double RPMrate = (RPMerror-RPMerrorOld); //added 4.6.12 SUPERAWESOME
  	double ChangeSpeed=proportion*RPMerror + derivative*RPMrate; // -added 4.6.12 SUPERAWESOME
	//TODO RPMerrorOld-instantiate, deltatime
	
	
	if (TargetRPMx4 < 1300 && TargetRPMx4 !=0)
	{
		MotorSpeed = 0.15;
	}
	else if (TargetRPMx4 == 0)
		MotorSpeed = 0;
	else if (TargetRPMx4 > CurrentRPMx4 + deadband || TargetRPMx4 < CurrentRPMx4 - deadband)
	{
		MotorSpeed = MotorSpeed - ChangeSpeed;
	}
	if (MotorSpeed > 1.0)
		MotorSpeed = 1.0;
	
	if (MotorSpeed < 0.0)
		MotorSpeed = 0.0;
	
	SmartDashboard::PutNumber("CurrentRPM",CurrentRPMx4);
	SmartDashboard::PutNumber("Motor Speed",MotorSpeed);
	//SmartDashboard::GetInstance()->PutDouble("RPMError",RPMerror);
	//SmartDashboard::GetInstance()->PutDouble("Change in Speed",ChangeSpeed);
				
	ShooterMotor.Set(MotorSpeed);
	
	RPMerrorOld=RPMerror; //added 4.6.12 SUPERAWESOME

}




JankyShooter2::JankyShooter2(int JagPort, int EncoderAPort, int EncoderBPort):
	Victor(JagPort),
	ShooterEncoder(EncoderAPort,EncoderBPort,false,ShooterEncoder.k1X),
	//PID(0.02,0.03,0.01,this,this)
	PID(0.02,0.03,0.01,this,this)
{
	Victor::Set(0.0);
	TargetRPMx4 = 0;
	CurrentRPMx4 = 0;
	MotorSpeed = 0;
	PID.SetInputRange(0.0,3000.0);
	PID.SetOutputRange(0.0,1.0);
	PID.SetSetpoint(0.0);
	PID.Enable();
	EncoderTimer.Reset();
	EncoderTimer.Start();
	PreviousCount=0;
	PreviousTime=0;
	PreviousRPM=0;
	SmartDashboard::PutNumber("Delta Time", -1.0);
	SmartDashboard::PutNumber("Delta Count", -1);
	GetCurrentRPM();
	char initstr[20];
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("P-shooter", SHOOTER_P));
	SmartDashboard::PutString("P-shooter", initstr);
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("I-shooter", SHOOTER_I));
	SmartDashboard::PutString("I-shooter", initstr);
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("D-shooter", SHOOTER_D));
	SmartDashboard::PutString("D-shooter", initstr);	
	SmartDashboard::PutString("Set Called","No");
}

JankyShooter2::~JankyShooter2(void)
{
	PID.Disable();
}

int JankyShooter2::GetCurrentRPM(void)
{
	
	int CurrentCount= ShooterEncoder.Get();
	
	double CurrentTime= EncoderTimer.Get();
	
	long CurrentRPM= ((long)((float)(CurrentCount-PreviousCount)/(CurrentTime-PreviousTime)))/6;
	
	//current count (tick/sec)-need to convert-RPM-x60 sec, /360 ticks = (1/6)
	
	CurrentRPM = (PreviousRPM * 15 + CurrentRPM) / 16;
	
	double DeltaTime=CurrentTime-PreviousTime;
	
	int DeltaCount=CurrentCount-PreviousCount;
	
	if (CurrentCount<PreviousCount || DeltaCount>10000)
	{
		PreviousCount=CurrentCount;
		PreviousTime=CurrentTime;
		
		return PreviousRPM;
	}
	if (CurrentRPM>10000 || CurrentRPM<=0)
	{
		if (CurrentRPM!=0)
		{	
		SmartDashboard::PutNumber("Delta Time", DeltaTime);
		SmartDashboard::PutNumber("Delta Count", DeltaCount);
		
		}
		return PreviousRPM;
	}
	else
	{	
		PreviousCount=CurrentCount;
	
		PreviousTime=CurrentTime;
		
		PreviousRPM=(int)CurrentRPM;
	
		SmartDashboard::PutNumber("Current Encoder Value", CurrentCount);
		if (CurrentRPM > 10000 || CurrentRPM < 0)
			SmartDashboard::PutString("Test", "Help!");
		
		SmartDashboard::PutNumber("Current RPM",(int)CurrentRPM);
		
	
		return (int)CurrentRPM;
	}	
}
void JankyShooter2::setTargetRPM(int desiredrpm)
{
	
	TargetRPMx4 = desiredrpm;
//	PID.SetSetpoint(TargetRPMx4);
//	SmartDashboard::GetInstance()->PutDouble("PIDOutput",PID.m_pidOutput);
	
	PID.SetSetpoint(TargetRPMx4);
	
	InteractivePIDSetup();
	
}

void JankyShooter2::Set(double Motor)
{
	if (Motor < 0.0)
		Victor::Set(0.0);
	if (Motor > 1.0)
		Victor::Set(1.0);
	else
		Victor::Set(Motor);
	SmartDashboard::PutString("Set Called","Yes");
	SmartDashboard::PutNumber("VictorGet",Victor::Get());
}

void JankyShooter2::InteractivePIDSetup(void)
{
	float p, i, d;
	std::string tempstring;

	//
	// Pull values from the smart dashboard and then put them into preferences.
	//
	
	tempstring=SmartDashboard::GetString("P-shooter");
	sscanf(tempstring.c_str(), "%f", &p);
	Preferences::GetInstance()->PutDouble("P-shooter", p);
	
	tempstring=SmartDashboard::GetString("I-shooter");
	sscanf(tempstring.c_str(), "%f", &i);
	Preferences::GetInstance()->PutDouble("I-shooter", i);

	tempstring=SmartDashboard::GetString("D-shooter");
	sscanf(tempstring.c_str(), "%f", &d);
	Preferences::GetInstance()->PutDouble("D-shooter", d);
	
	PID.SetPID(p,i,d);
}



