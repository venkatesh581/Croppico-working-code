#include"Arduino.h"
#include"RelayInit.h"

#define chillerPump PA8  
#define waterPump PD2
#define pHdownPump PB15
#define pHupPump PA12

#define ECPump_A PB12  //EC A
#define ECPump_B PB13  //EC B

#define Lamp1 PB0
#define Lamp2 PB1
#define Lamp3 PB2
#define Lamp4 PB10                
#define Lamp5 PB11

#define ECpwr PB4
#define ECADCpwr PB3
#define PHpwr PD1
#define PHADCpwr PD0
#define INlet PA4
#define outlet PC_4
#define Sensor3v PC_1
#define Sensor5v PC_0


/**
 * @brief Initial Relay Setup function
 * 
 */
void RelayInit::TrigOutput()
{
    pinMode(waterPump, OUTPUT); // water pump
    pinMode(chillerPump, OUTPUT); // chiller
//    pinMode(chillerMotor, OUTPUT);
    pinMode(pHdownPump, OUTPUT); // ph low
    pinMode(pHupPump, OUTPUT); // ph high
    pinMode(ECPump_A, OUTPUT); // ec_A
    pinMode(ECPump_B, OUTPUT); // ec_B
    pinMode(Lamp1, OUTPUT); //lamp -1
    pinMode(Lamp2, OUTPUT); //lamp -2
    pinMode(Lamp3, OUTPUT); //lamp -3
    pinMode(Lamp4, OUTPUT); //lamp -4
    pinMode(Lamp5, OUTPUT); //lamp -5
    pinMode(PHpwr, OUTPUT); // pH power
    pinMode(PHADCpwr, OUTPUT); // pH power

    pinMode(ECpwr, OUTPUT); // EC power
    pinMode(ECADCpwr, OUTPUT); // EC power
    pinMode(INlet, OUTPUT); // EC power 
     pinMode(outlet, OUTPUT);
     pinMode(Sensor3v, OUTPUT);
     pinMode(Sensor5v, OUTPUT);


    

    digitalWrite(waterPump,LOW);
    digitalWrite(chillerPump,LOW);

    digitalWrite(pHdownPump,LOW);
    digitalWrite(pHupPump,LOW);
    digitalWrite(ECPump_A,LOW);
    digitalWrite(ECPump_B,LOW);

}

/**
 * @brief Water pump ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: WaterPump(int state)
{
    digitalWrite(waterPump,state);
}

/**
 * @brief Chiller pump ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Chiller(int state)
{
    digitalWrite(chillerPump,state);
//    digitalWrite(chillerMotor,state);
}

/**
 * @brief pH down solution ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: pHDown(int state)
{
    digitalWrite(pHdownPump,state);
}

/**
 * @brief pH up solution ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: pHUp(int state)
{
    digitalWrite(pHupPump,state);
}

/**
 * @brief EC supplement pump ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: ecPump_A(int state)
{
    digitalWrite(ECPump_A, state);
}

/**
 * @brief EC supplement pump ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: ecPump_B(int state)
{
    digitalWrite(ECPump_B, state);
}

/**
 * @brief Light 1 ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Light1(int state)
{
    digitalWrite(Lamp1, state);
}

/**
 * @brief Light 2 ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Light2(int state)
{
    digitalWrite(Lamp2, state);
}

/**
 * @brief Light 3 ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Light3(int state)
{
    digitalWrite(Lamp3, state);
}

/**
 * @brief Light 4 ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Light4(int state)
{
    digitalWrite(Lamp4, state);
}

/**
 * @brief Light 5 ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: Light5(int state)
{
    digitalWrite(Lamp5, state);
}

/**
 * @brief pH sensor power ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: pHPower(int state)
{
    digitalWrite(PHpwr,state);
}


void RelayInit :: pHADCPower(int state)
{
    digitalWrite(PHADCpwr,state);
}

/**
 * @brief EC sensor power ON/OFF function
 * 
 * @param state 1/0
 */
void RelayInit :: EcPower(int state)
{
    digitalWrite(ECpwr,state);
}


void RelayInit :: EcADCPower(int state)
{
    digitalWrite(ECADCpwr,state);
}

void RelayInit :: solenoidIN(int state)
{
    digitalWrite(INlet,state);
}

void RelayInit :: solenoidOut(int state)
{
    digitalWrite(outlet,state);
}

void RelayInit :: sensor3v(int state)
{
    digitalWrite(Sensor3v,state);
}

void RelayInit :: sensor5v(int state)
{
    digitalWrite(Sensor5v,state);
}
