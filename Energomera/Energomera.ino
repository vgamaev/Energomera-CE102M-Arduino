
#include <SoftwareSerial.h>
int i = 0;
long Previous = 0; 
int val = 0;
int Time_1 = 1;         
int Time_2 = 2;          
int Time_3 = 3;
int Time_4 = 4;
int Time_5 = 5;
int Time_6 = 6;
int Time_7 = 7;
int Time_8 = 8;
int Time_9 = 15; 

char x [50];
// открываем сессию
byte cmd1[] = {0xaF,0x3F,0x21,0x8D,0x0A};
// читаем тип счетчика
byte cmd2[] = {0x06,0x30,0x35,0xb1,0x8d,0x0a};
// снимаем показания
byte cmd3[] = {0x81,0xd2,0xb1,0x82,0xc5,0xd4,0x30,0x50,0xc5,0x28,0xa9,0x03,0xb7};
//напряжение на фазах
byte cmd4[] = {0x81,0xd2,0xb1,0x82,0x56,0xcf,0xcc,0xd4,0x41,0x28,0xa9,0x03,0x5f};
// мощность
byte cmd5[] = {0x81,0xd2,0xb1,0x82,0x50,0xcf,0xd7,0xc5,0x50,0x28,0xa9,0x03,0xe4};
// частота  
byte cmd6[] = {0x81,0xd2,0xb1,0x82,0xc6,0xd2,0xc5,0xd1,0x55,0x28,0xa9,0x03,0x5c};
// ток  
byte cmd7[] = {0x81,0xd2,0xb1,0x82,0xc3,0x55,0xd2,0xd2,0xc5,0x28,0xa9,0x03,0x5a};
// ток  
byte cmd8[] = {0x81,0xd2,0xb1,0x82,0xc3,0x55,0xd2,0xd2,0xc5,0x28,0xa9,0x03,0x5a};



#define DIR 8 
 
SoftwareSerial RS485 (10, 11); // RX, TX

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
  switch (val) {
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
        break;
        default:
  
        break;
      }
}


void EmergomeraWrite()
{
     if (val == Time_1)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd1,5);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_2)
       {
        digitalWrite(DIR, HIGH);
         RS485.write (cmd2,6);
        digitalWrite(DIR, LOW); 
       }
       if (val == Time_3)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd3,13);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_4)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd4,13);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_5)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd5,13);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_6)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd6,13);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_7)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd7,13);
         digitalWrite(DIR, LOW);
       }
       if (val == Time_8)
       {
         digitalWrite(DIR, HIGH);
         RS485.write (cmd8,13);
         digitalWrite(DIR, LOW);
       }
       
       if (val == Time_9)
       {
          val = 0;
       }
}

void setup()
{
  Serial.begin(9600);
  RS485.begin(9600);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, HIGH);
}

void loop()
{ 
  if (RS485.available())
    {
      char response = RS485.read();
      response &= 0x7F;// convert 8N1 to 7E1
      
      //Serial.print(response);
      char inChar = response;

      ReadStr += inChar;
    }
    
    ReadStr = "FREQU(50.00)";
      
    if (millis() - Previous > 1000)
    {
     Previous = millis();      
     EnergomeraRead();
     val ++;
     EmergomeraWrite();          
     }
}
   
