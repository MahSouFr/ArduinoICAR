#include <icar_func.h>
#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>
int _SPI_CS_PIN=9; //Bus can
MCP_CAN CAN(_SPI_CS_PIN); //set CS pin
I_car::I_car(long pin)  //reservation des pin
{
	_pin=pin;
}
I_car::I_car()  
{
	//default constructor
}
void I_car::analog_can_writer(unsigned char tab[8], int fsval,long analog,int AR) //ecrire le valeur analogique dans le buf( e:pour ordre d\avance ou recule)
{
	/*
		functionnality steps :
		1) In parameters we have : 
									- the table to fill and send : unsigned char tab[8]
									- the foot switch value will be on 0 or in 1, it shoul be added to tab[2] : int fsval
									- analog value to be sent to the variateur to tab[3] : long analog
									- avant ou arriere to tab[4]
		
		2) Write paraeter value to _tab, to be written on the CAN circuit
	*/
	_tab=tab; //le buf (message)

	int analog_value=analogRead(_analog); //convertir le valeur analogique en entier
	analog_value=map(analog_value, 0, 1023, 0,255);

	_analog=analog; //analog: le pin analogique (en decimal)

	_AR = AR; //avance/recule
	
	//remplir le buf
	_tab[2] = fsval;
	_tab[3] = analog_value;
	_tab[4] = AR;
	
	CAN.sendMsgBuf(0x00,0,8,(byte *)_tab); //format:(id en hexa, id_type:0 for 11 bit can(entre 0x000 et 0x7FF), dlc: numero du bit in thedata buf, data buf)
									       //le (byte *) transform le tab (char *) en byte *
								       
	delay(100);									   
}
void I_car::can_init() //initialisation du bus can
{
	Serial.begin(_baud);
	while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(10); //f=1/t=1/100ms
    }
    Serial.println("CAN BUS Shield init ok!");
}
void I_car::in_pullup() //car le pin est relier a un resistance
{
	pinMode(_pin,INPUT_PULLUP);
}
void I_car::in() //set input pin
{
	pinMode(_pin,INPUT);
}
void I_car::out()//et output pin
{
	pinMode(_pin,OUTPUT);
}
void I_car::dw_hight() //digital hight
{
	digitalWrite(_pin,HIGH);
}

void I_car::dw_low() //digital low
{
	digitalWrite(_pin,LOW);
}

void I_car::can_reader1(unsigned char tab[8],unsigned char len) //read buf
{
	_tab1=tab;
	_len=len;
	if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data comming
    {
        CAN.readMsgBuf(&_len,_tab1);    // read data,  len: data length, buf: data buf
        unsigned long canId = CAN.getCanId();
        //Serial.println("-----------------------------");
        //Serial.print("Get data from ID: 0x");
        //Serial.println(canId, HEX);
        //Serial.println(_tab1[1]);
    }
} 

void I_car::analog_write(unsigned char tab[8],int b) //b:indice du valeur analogique dans le buf
{
    _tab1=tab;
	analogWrite(_pin,((int)_tab1[b]));
	//Serial.println(_tab1[1]);
}

bool I_car::digital_read() 
{
	return digitalRead(_pin);
}