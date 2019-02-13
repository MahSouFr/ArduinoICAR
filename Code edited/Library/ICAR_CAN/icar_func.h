#ifndef test   //envelope du fichier .h
#define test
#include <Arduino.h> //for arduino version >100
#include <SPI.h>
#include <mcp_can.h>
class I_car {
	public:
	    I_car(long pin); // constructeur
        I_car();//constructeur par defaut
        void analog_write(unsigned char tab[8],int b);
		void analog_can_writer(unsigned char tab[8],int fsval,long analog,unsigned char e);  //read the analog value from analog pin and write it to can
		void can_reader1(unsigned char tab[8],unsigned char len);
		bool digital_read(); //lire le valeur digital
		void begin();
		void in();        //set input
		void out();       //set output
		void dw_hight();   //set digital pin to hight
		void dw_low();     //set digital pin to low
		void can_init();   //initialisation du bus can
		void in_pullup();  //pin relier  un resistance
	private:    //conservation des valeur donner comme argment des fonctions
	long _pin;  
	long _analog; //le pin analogique
	int _baud=9600; //set baud rate to 9600
	unsigned char* _tab; //le buffer
	unsigned char* _tab1;
	unsigned char _len; 
	unsigned char _AR; 
};
#endif	