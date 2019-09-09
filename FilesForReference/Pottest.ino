#define battVolt B2 //battery voltage
#define UCPVolt B3 //uncleaned panel voltage
#define CPVolt B4 // cleaned panel voltage
#define UCPcurr A1 //current uncleaned panel
#define CPcurr A0 // current cleaned panel
#define tempUCP D1 // temperature of uncleaned panel
#define tempCP D3 // temperature of cleaned panel
#define tempBatt D5 //temperature of battery
#define irr B5 //irr


float vBat = 0;   //voltage of battery
float vUcp = 0;   //voltage of uncleaned panel
float vCp = 0;    //voltage of cleaned panel
float curUcp = 0; //current of uncleaned panel
float curCp = 0; //current of cleaned panel
float tempBat = 0; //temperature of battery
float tempUcp = 0; //temperature of uncleaned panel
float tempCp = 0; //temperature of cleaned panel
float irradiation = 0;
