#include <VirtualWire.h>
//#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define TFT_CS     10
//#define TFT_RST    8                      
//#define TFT_DC     9
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define OLED_RESET 4
Adafruit_SSD1306 tft(OLED_RESET);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!

const int receive_pin = 11;
char temperatureChar[10];
char humidityChar[10];


struct package
{
  float temperature = 0.0;
  float humidity = 0.0;
};


typedef struct package Package;
Package data;


void setup()
{
    Serial.begin(9600);   
   // tft.initR(INITR_BLACKTAB);
    //tft.fillScreen(ST7735_BLACK); 
  //  printUI();
    //delay(1000);
tft.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
 tft.display();
  delay(50); tft.display();

  // Clear the buffer.
  tft.clearDisplay();
    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[sizeof(data)];
    uint8_t buflen = sizeof(data);

if (vw_have_message())  // Is there a packet for us? 
  {
    vw_get_message(buf, &buflen);
    memcpy(&data,&buf,buflen);

    Serial.print("T celsius ");
    Serial.print(data.temperature);
    tft.setTextSize(1);
  tft.setTextColor(WHITE);
tft.setCursor(5,0); tft.setTextColor(BLACK, WHITE); 
  tft.println("Remote 433MHz DHT11");tft.display();
  tft.setTextColor(WHITE, BLACK); 
  
    String temperatureString = String(data.temperature,1);
    temperatureString.toCharArray(temperatureChar,10);
  //  tft.fillRect(10,20,80,30,ST7735_BLACK);
   // printText(temperatureChar, ST7735_WHITE,10,20,3);
  tft.setCursor(0,10); tft.println(temperatureChar); tft.setCursor(30,10);tft.print ("T: Celsius");tft.display();

    String humidityString = String(data.humidity,1);
    humidityString.toCharArray(humidityChar,10);
 //   tft.fillRect(10,95,80,100,ST7735_BLACK);
 //   printText(humidityChar, ST7735_WHITE,10,95,3);
tft.setCursor(0,20);tft.println(humidityChar);tft.setCursor(30,20);tft.print("Hum: %");tft.display();
    Serial.print(" Hum %");
    Serial.print(data.humidity);
  }
}

void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}

void printUI()
{
    printText("TEMPERATURE", WHITE,30,5,1);  // Temperature Static Text
    printText("o", WHITE,90,13,2);
    printText("C", WHITE,105,20,3);

    printText("HUMIDITY", WHITE,30,80,1);  // Temperature Static Text
    printText("%", WHITE,90,95,3);
}

