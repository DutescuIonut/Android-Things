#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>



/* PIN LAYOUT 
 *  RC522 sensor
 *  SDA ---->53
 *  SCK----->52
 *  MOSI---->51
 *  MISO---->50
 *  RST----->5
 *  IRQ not connected
 *  
 *  LEDS
 *  rosu=38
 *  verde=36
 *  
 *  KEYPAD
 *  
 *  1--->33
 *  2--->35
 *  3--->37
 *  4--->39
 *  5--->41
 *  6--->43
 *  7--->45
 *  8--->47
 *  
 *  LCD
 *  GND---->GND
 *  VCC--->5V
 *  SDA---->SDA
 *  SCL--->SCL
 *  
 */
 


LiquidCrystal_I2C lcd(0x27,16,2);


  char acces[50][4];
  int j=0;
 const int rosu=38;
 const int verde=36;
#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);  


const byte ROWS=4;
const byte COLS=4;
char hexaKeys[ROWS][COLS]={
  {'1','2','3','A' },
  {'4','5','6','B' },
  {'7','8','9','C' },
  {'*','0','#','D' },
};

byte rowPins[ROWS]={33,35,37,39};
byte colPins[COLS]={41,43,45,47};
Keypad customKeypad= Keypad(makeKeymap(hexaKeys),rowPins,colPins,ROWS,COLS);

void add(char cartela[50])
{
  
  j++;
  if(gasesc(cartela))
      return 0;
  else
      { lcd.setCursor(0,1);
        lcd.print("Adaugare cartela");
        j++;
        for (byte i = 0; i <strlen(cartela); i++)
            acces[j][i]=cartela[i];
        delay(2000);
        lcd.clear();
        }
      
}
void del(char cartela[50])
{
  lcd.setCursor(0,1);
  lcd.print("Stergere cartela");
  for(int i=1;i<=j;i++)
    if(strcmp(acces[i],cartela)==0)
        acces[i][0]=0;
   delay(2000);
   lcd.clear();
  
 
}

int gasesc(char cartela[50])
{
  for(int i=1;i<=j;i++)
    if(strcmp(acces[i],cartela)==0)
      return 1;
  return 0;
}

void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);  
  SPI.begin();      
  mfrc522.PCD_Init();  
  lcd.print("Apropiati cartela...");
  pinMode(rosu,OUTPUT);
  pinMode(verde,OUTPUT);

}
void loop() 
{ 
  
    char customKey;
    bool ok=false;
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
   
    return;
  }
 
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    
    return;
  }
 
  while(ok==false)
    {customKey=customKeypad.getKey();
    if(customKey=='1' || customKey=='2' || customKey=='3')
          ok=true;
      }
  
  Serial.print("UID tag :");
  char content[50];
  String text="";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     text.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     text.concat(String(mfrc522.uid.uidByte[i], HEX));
    
  }
  Serial.println();
  for(char i=0;i<text.length();i++)
    content[i]=30+text[i]-30;
    content[text.length()]=0;
  j++;
  if(customKey=='1')
    add(content);
   if(customKey=='2')
    del(content);
  if(customKey=='3') 
    {if (gasesc(content)) 
      { lcd.setCursor(0,1);
        lcd.print("Authorized access");
        digitalWrite(verde,HIGH);
        Serial.println();
        delay(3000);
        lcd.clear();
        digitalWrite(verde,LOW);
       }
 
      else   
      {
    lcd.setCursor(0,1);
    lcd.print(" Access denied");
    digitalWrite(rosu,HIGH);
    delay(3000);
    lcd.clear();
    digitalWrite(rosu,LOW);
      }
   }
}
