#include <SoftwareSerial.h>

long Previous = 0; 
int Step = 0;
char x [50];

// открываем сессию
byte CmdOpenSesion[] = {0xaF,0x3F,0x21,0x8D,0x0A};
// читаем тип счетчика
byte CmdReadType[] = {0x06,0x30,0x35,0xb1,0x8d,0x0a};
// снимаем показания
byte CmdEtope[] = {0x81,0xd2,0xb1,0x82,0xc5,0xd4,0x30,0x50,0xc5,0x28,0xa9,0x03,0xb7};
//напряжение на фазах
byte CmdVolta[] = {0x81,0xd2,0xb1,0x82,0x56,0xcf,0xcc,0xd4,0x41,0x28,0xa9,0x03,0x5f};
// мощность
byte CmdPower[] = {0x81,0xd2,0xb1,0x82,0x50,0xcf,0xd7,0xc5,0x50,0x28,0xa9,0x03,0xe4};
// частота  
byte CmdFrequ[] = {0x81,0xd2,0xb1,0x82,0xc6,0xd2,0xc5,0xd1,0x55,0x28,0xa9,0x03,0x5c};
// ток  
byte CmdCurre[] = {0x81,0xd2,0xb1,0x82,0xc3,0x55,0xd2,0xd2,0xc5,0x28,0xa9,0x03,0x5a};
// cos f  
byte CmdCos_f[] = {0x81,0xd2,0xb1,0x82,0xc3,0x55,0xd2,0xd2,0xc5,0x28,0xa9,0x03,0x5a};


#define CYCLE_TIME 15
#define DIR 8 
 
SoftwareSerial UART (14, 12); // RX, TX

struct Energomera{
  String NameParam;
  String StrValue;
  float  FloatValue;
};


String ReadStr;
Energomera Etope;
Energomera Volta;
Energomera Power;
Energomera Curre;
Energomera Frequ;
Energomera Cos_f;


Energomera ValueParser(String inString, String Param)
{
  String inString2;
  Energomera Buffer;

  Buffer.NameParam = Param;
  Buffer.StrValue = "0";
  Buffer.FloatValue = 0;

  if (inString.lastIndexOf(Param)>0) 
      {
          int first = inString.indexOf(Param);
          first = inString.indexOf('(', first + 1 );
          first = first +1;
          //Serial.println(first);
          int first_2 = inString.indexOf(')', first + 1 );
                   
          inString2 = inString.substring(first,first_2);
          inString2.toCharArray(x, sizeof(x));
          float f = atof(x);
          
          Buffer.StrValue = inString2;
          Buffer.FloatValue = f;
          
          inString = "";
      }
      return Buffer;
}

void PrintVolume(Energomera Buffer)
{
   Serial.print(Buffer.NameParam); 
   Serial.print("   ");
   Serial.print(Buffer.StrValue);   //формат string
   Serial.print("------>");
   Serial.println(Buffer.FloatValue);           //формат float
}

void EnergomeraRead()
{
  switch (Step) {
        case 1:
              
        break;
        case 2:
              
        break;
        case 3:
              Etope = ValueParser(ReadStr, "ET0PE");
              PrintVolume(Etope);
        break;
        case 4:
              Volta = ValueParser(ReadStr, "VOLTA");
              PrintVolume(Volta);
        break;
        case 5:
              Power = ValueParser(ReadStr, "POWEP");
              PrintVolume(Power);
        break;
        case 6:
              Frequ = ValueParser(ReadStr, "FREQU");
              PrintVolume(Frequ);
        break;
        case 7:
              Curre = ValueParser(ReadStr, "CURRE");
              PrintVolume(Curre);
        break;
        case 8:
              Cos_f = ValueParser(ReadStr, "COS_f");
              PrintVolume(Cos_f);
              Serial.println();
        break;
        default:
  
        break;
      }
      ReadStr = "";
}


void SendCommand(byte* cmd, int size)
{
    digitalWrite(DIR, HIGH);
    UART.write (cmd,size);
    digitalWrite(DIR, LOW);
}

void EmergomeraWrite()
{
    switch (Step) {
        case 1:
             SendCommand(CmdOpenSesion,5);
        break;
        case 2:
             SendCommand(CmdReadType,6);
        break;
        case 3:
             SendCommand(CmdEtope,13);
        break;
        case 4:
             SendCommand(CmdVolta,13);
        break;
        case 5:
             SendCommand(CmdPower,13);
        break;
        case 6:
             SendCommand(CmdFrequ,13);
        break;
        case 7:
             SendCommand(CmdCurre,13);
        break;
        case 8:
             SendCommand(CmdCos_f,13);
        break;
        case CYCLE_TIME:
               Step = 0;
        break;
          default:
  
        break;
      }
     
}

void setup()
{
  Serial.begin(9600);
  UART.begin(9600);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, HIGH);
}

void EnergomeraCycle()
{ 
  if (UART.available())
    {
      char response = UART.read();
      response &= 0x7F;// convert 8N1 to 7E1
      
      //Serial.print(response);
      char inChar = response;

      ReadStr += inChar;
    }
    
    //ReadStr = "FREQU(50.00)";
      
    if (millis() - Previous > 1000)
    {
     Previous = millis();      
     EnergomeraRead();
     Step ++;
     EmergomeraWrite();          
     }
}

void loop()
{
    EnergomeraCycle();
}
