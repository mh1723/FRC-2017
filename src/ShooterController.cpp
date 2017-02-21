#include "WPILib.h"
#include "Params.h"
#include "ShooterController.h"
#include "RobotModel.h"

ShooterController::ShooterController(RobotModel *myRobot, RemoteControl *myHumanControl) {
  robot = myRobot;
  humanControl = myHumanControl;

  shooterP = 0.005;
  shooterI = 0.0;
  shooterD = 0.05;

  pidOutput = new ShooterMotorsPIDOutput(robot->shooterMotorA, robot->shooterMotorB);
  shooterPID = new PIDController(shooterP, shooterI, shooterD, robot->shooterEncoder, pidOutput);

  shooterPID->SetPIDSourceType(PIDSourceType::kRate);

  shooterPID->SetSetpoint(2000.0 / 60.0);
  shooterPID->SetAbsoluteTolerance(100.0 / 60.0);
  shooterPID->Disable();
  m_stateVal = kInitialize;
  nextState = kInitialize;
}

ShooterController::~ShooterController() {

}

void ShooterController::Reset() {
  m_stateVal = kInitialize;
}

void ShooterController::Update(double currTimeSec, double deltaTimeSec) {
  switch (m_stateVal) {
    case (kInitialize):
      nextState = kTeleop;
      break;
    case (kTeleop):
      //Shooter Behaviour
      if(humanControl->GetShooterRunDesired()){
        if(SHOOTER_USE_PID){
            shooterPID->Enable();
        }
        else {
          robot->SetShooterMotorsSpeed(SHOOTER_HARDSET_MOTOR_SPEED);
        }
      }
      else { 
        if(SHOOTER_USE_PID){
          shooterPID->Disable();
        }
        else {
          robot->SetShooterMotorsSpeed(0.0);
        }
      }
      nextState = kTeleop;
      break;
  } 

  m_stateVal = nextState;
}