#include <icar_func.h>
#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

/*
    In the master, represented by the carte relay, we have firstly to recover 
    our data, this data is represented by : 
        -   In priority, check the footswitch, If its on or off, to know 
            if we have to take action or no !
        -   This action that we should take in the end is used to turn 
            on or off the "Embryage" in the desired direction (forward 
            or reverse)
        -   To know the direction we have to read the switch (AR) status 

    FootSwitch ON  -> (A:R)? -> take action
               OFF -> Do Nothing 
*/

//status variable
int ST_FS;
int ST_AR;
int ST_EMBR;

bool tmp_stfs, tmp_star;

//For the mode mode : Thermique ou electrique
//I_car modeTorE(1);

//To use function in the library
I_car function;

//To cover values from FS & AR pins
I_car FS_Pin(2);
I_car AR_Pin(8);

//potentiometre qui est a A0 sert pour le calcule du vitesse desirer
long POT_FS_SPEED = A0;

//value to set to the Embryage
//I_car Embr(4);

//message a envoyer
unsigned char msg[8] = {1, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
    //Initialisation des entrees et des sorties
    //modeTorE.in();
    FS_Pin.in();
    AR_Pin.in();
    pinMode(POT_FS_SPEED, INPUT);

    //initialisation du bus can
    function.can_init();
}

void loop()
{
    //read switch status

    tmp_stfs = FS_Pin.digital_read();
    tmp_star = AR_Pin.digital_read();
    ST_FS = (int)tmp_stfs;
    ST_AR = (int)tmp_star;
    
    if (ST_FS == 1) {
        if (ST_AR == 1) {   
            function.analog_can_writer(msg,ST_FS,POT_FS_SPEED,1);
        } else {   
            function.analog_can_writer(msg,ST_FS,POT_FS_SPEED,0);
        }
    }
    else{
        function.analog_can_writer(msg,ST_FS,"A1",0);
    }
    Serial.println(ST_FS);
}
//I am trying to edit
