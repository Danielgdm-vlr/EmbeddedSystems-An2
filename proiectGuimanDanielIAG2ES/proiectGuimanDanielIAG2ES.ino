#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Rtc_Pcf8563.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//init the real time clock
Rtc_Pcf8563 rtc;
int hours = 0, minutes = 0, seconds = 0, timeUnit = 0;
int day = 0, month = 0, year = 0, dateUnit = 0;
bool ok = false, okDate = false;

//button variables
const int buttonPin1 = 1; 
const int buttonPin2 = 2; 
int buttonState1 = 0;
int buttonState2 = 0;


//encoder variables
#define outputA 7
#define outputB 6
#define outputC 5
int encoderButtonState = 0;
int counterMenuZwei = 0; 
int counterMenuThree = 0;
int aState;
int aLastState;  

//menu stuff
int menuItem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;
String menuZweiOptions[4] = {"Alarm", "Timer", "Reverse Timer", "Temperature"};
String menuThreeOptions[2] = {"Set TIME", "Set DATE"};
int scroll, pages = 3;

//int i = 0;
  
void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  
  //initializare pini butoane
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  scroll = 1;

  //initializare pini encoder
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode(outputC, INPUT_PULLUP);
  aLastState = digitalRead(outputA);
  
  display.display();
  delay(2000);
  display.clearDisplay();

  loadingSchema();
  delay(1500);
  display.clearDisplay();
  
  loadingManevra();
  delay(250);
  display.clearDisplay();
  display.clearDisplay();
  
  mainMenu();
}

void loop() {
  scrollThroughPages();
  scrollThroughMenuOptions();
  clickEncoderMenuOptions();
  
  /*readRotaryEncoder();
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
   switch (b) {
      case ClickEncoder::Clicked:
         middle=true;
        break;
    }
  } */   

  /*display.clearDisplay();
  aState = digitalRead(outputA);
  if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        i++;
      } else {
        delay(300);
        i--;
      }
  }

  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(0, 0);
  if(abs(i)>=0 && abs(i) <= 23)
    display.print(abs(i));
  display.display();*/
}

void loadingSchema(void){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5, 25);
  display.println("Loading...");
  display.display();
}

void loadingManevra(void){
   display.setTextSize(2);
   display.setTextColor(WHITE);
   display.setCursor(45, 25);
   for(int i = 1; i <= 100; i++){
     display.print(i);
     display.print(" %");  
     if(i < 50){
      delay(50);
      display.display();
     }
     else if(i >= 50 && i <= 98){
      delay(100);
      display.display();
     }
     else if(i > 98 && i < 100){
      delay(250);
      display.display();
     }
     else if(i == 100){
      delay(500);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(30, 25);
      display.print("Succes");
      display.display();
      delay(1000);
     }
     display.setCursor(45, 25);
     display.clearDisplay();
  }
}

//In functia mainMenu este afisata ora si data;
void mainMenu(){
  //afisare ora hh:mm
  showTime();
  
  //data  dd/mm/yyyy
  showDate();

  display.display();

}

//In functia menuZwei sunt afisate optiunile de Alarma, Timer, si Reverse Timer
void menuZwei(void){
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setCursor(0, 0);
   for(int i = 0; i < 4; i++){
    if(i == counterMenuZwei){
      display.setTextSize(2);
      display.println(menuZweiOptions[i]);
      
    }
    else{
      display.setTextSize(1);
      display.println(menuZweiOptions[i]);
    }
   }
   display.display();
}

//In functia menuThree sunt afisate optiunile din meniul Setting, adica cele de setTime si setDate;
void menuThree(void){
   display.clearDisplay();
   display.setTextSize(2);
   display.setTextColor(WHITE);
   display.setCursor(0, 0);
   for(int i = 0; i < 2; i++){
    if(i == counterMenuThree){
      display.setTextSize(2);
      display.println(menuThreeOptions[i]);
    }
    else{
      display.setTextSize(1);
      display.println(menuThreeOptions[i]);
    };
   }
   display.display();
}
void scrollThroughPages(){
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  if(buttonState1 == HIGH){
    delay(300);
    scroll--;
  }
  if(buttonState2 == HIGH){
    delay(300);
    scroll++;
  }
  if(scroll > pages){
    scroll = 1;
  }
  if(scroll < 1){
    scroll = pages;
  }
  if(scroll == 1){
    display.clearDisplay();
    mainMenu();
  }
  else if(scroll == 2){
    display.clearDisplay();
    menuZwei();
  }
  else if(scroll == 3){
    display.clearDisplay();
    menuThree();
    //setTime();
  }
}

void scrollThroughMenuOptions(void){
  aState = digitalRead(outputA);
  if(scroll == 2){
    counterMenuThree = 0;
    if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        counterMenuZwei++;
      } else {
        delay(300);
        counterMenuZwei--;
      }
      if(counterMenuZwei > 3){
        counterMenuZwei = 0;
      }
      if(counterMenuZwei < 0){
        counterMenuZwei = 3;
      }
    }
    aLastState = aState;
  }else if(scroll == 3){
    counterMenuZwei = 0;
    if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        counterMenuThree++;
      } else {
        delay(300);
        counterMenuThree--;
      }
      if(counterMenuThree > 1){
        counterMenuThree = 0;
      }
      if(counterMenuThree < 0){
        counterMenuThree = 1;
      }
    }
    aLastState = aState;
  }
  else if(scroll == 1){
    counterMenuZwei = 0;
    counterMenuThree = 0;
  }
}


void clickEncoderMenuOptions(void){
  encoderButtonState = digitalRead(outputC);
  if(encoderButtonState == LOW){
   if(scroll == 2){
     if(counterMenuZwei == 0){
        setAlarm();
     }
     if(counterMenuZwei == 1){
        setTimer();
     }
     if(counterMenuZwei == 2){
        setReverseTimer();
     }
     if(counterMenuZwei == 3){
        seeTemperature();
     }
   }
   else if(scroll == 3){
     if(counterMenuThree == 0){
        while(buttonState2 == LOW){
          display.clearDisplay();
          //setTime();
          buttonState2 = digitalRead(buttonPin2);
        }
        scroll--;
     }
     if(counterMenuThree == 1){
        while(buttonState2 == LOW){
          display.clearDisplay();
          //setDate();
          buttonState2 = digitalRead(buttonPin2);
        }
        scroll--;
        //setDate();
     }
   }
  }
}

void showTime(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(35, 11);

  if(rtc.getHour() < 10){
    display.print("0");
    display.print(rtc.getHour());
  }
  else{
    display.print(rtc.getHour());
  }
  display.print(":");

  if(rtc.getMinute() < 10){
    display.print("0");
    display.print(rtc.getMinute());
  }
  else{
    display.print(rtc.getMinute());
  }

}

void showDate(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5, 35);
  
  if(rtc.getDay() < 10){
    display.print("0");
    display.print(rtc.getDay());
  }
  else{
    display.print(rtc.getDay());
  }
  display.print("/");
  
  if(rtc.getMonth() < 10){
    display.print("0");
    display.print(rtc.getMonth());
  }
  else{
    display.print(rtc.getMonth());
  }
  display.print("/");
  
  display.print((2000 + rtc.getYear()));
  display.display();
}

void setAlarm(void){
}

void setTimer(void){

}

void setReverseTimer(void){

}

void seeTemperature(void){

}

void setTime(void){
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Current time is:");
  display.setTextSize(2);
 
  if(rtc.getHour() < 10){
    display.print("0");
    display.print(rtc.getHour());
  }
  else{
    display.print(rtc.getHour());
  }
  display.print(":");
  
  if(rtc.getMinute() < 10){
    display.print("0");
    display.print(rtc.getMinute());
  }
  else{
    display.print(rtc.getMinute());
  }
  display.print(":");
  
  if(rtc.getSecond() < 10){
    display.print("0");
    display.println(rtc.getSecond());
  }
  else{
    display.println(rtc.getSecond());
  }

  display.setTextSize(1);
  display.println("Change time to:");
  display.setTextSize(2);

  buttonState1 = digitalRead(buttonPin1);
  if(buttonState1 == HIGH){
    delay(300);
    timeUnit++;
  }
  
  buttonState2 = digitalRead(buttonPin2);
  if(buttonState2 == HIGH){
    delay(300);
    display.clearDisplay();
    menuThree();
  }
  
  if(timeUnit == 1){
    display.setTextColor(BLACK, WHITE);
    if(abs(hours) >= 0 && abs(hours) <= 23){
      if(abs(hours) < 10){
        display.print("0");
        display.print(abs(hours));
      }
      else{
        display.print(abs(hours));
      }
    }
    else{
      display.print("00");
      hours = 0;
    }
    setHours();
  }
  else{
    display.setTextColor(WHITE);
    if(abs(hours) >= 0 && abs(hours) <= 23){
      if(abs(hours) < 10){
        display.print("0");
        display.print(abs(hours));
      }
      else{
        display.print(abs(hours));
      }
    }
    else{
      display.print("00");
      hours = 0;
    }
  }
  
  display.setTextColor(WHITE);
  display.print(":");
  if(timeUnit == 2){
    display.setTextColor(BLACK, WHITE);
    if(abs(minutes) >= 0 && abs(minutes) <= 59){
      if(abs(minutes) < 10){
        display.print("0");
        display.print(abs(minutes));
      }
      else{
        display.print(abs(minutes));
      }
    }
    else{
      display.print("00");
      minutes = 0;
    }
    setMinutes();
  }
  else{
    display.setTextColor(WHITE);
    if(abs(minutes) >= 0 && abs(minutes) <= 59){
      if(abs(minutes) < 10){
        display.print("0");
        display.print(abs(minutes));
      }
      else{
        display.print(abs(minutes));
      }
    }
    else{
      display.print("00");
      minutes = 0;
    }
  }
  
  display.setTextColor(WHITE);
  display.print(":");
  if(timeUnit == 3){
    display.setTextColor(BLACK, WHITE);
    if(abs(seconds) >= 0 && abs(seconds) <= 59){
      if(abs(seconds) < 10){
        display.print("0");
        display.print(abs(seconds));
      }
      else{
        display.print(abs(seconds));
      }
    }
    else{
      display.print("00");
      seconds = 0;
    }
    setSeconds();
  }
  else{
    display.setTextColor(WHITE);
    if(abs(seconds) >= 0 && abs(seconds) <= 59){
      if(abs(seconds) < 10){
        display.print("0");
        display.print(abs(seconds));
      }
      else{
        display.print(abs(seconds));
      }
    }
    else{
      display.print("00");
      seconds = 0;
    }
  }
  display.println();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("button2 return");

  if(timeUnit >= 4){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("New time is:");
    display.setTextSize(2);
    display.print(abs(hours));
    display.print(":");
    display.print(abs(minutes));
    display.print(":");
    display.println(abs(seconds));
    display.setTextSize(1);
    display.print("Press button 2 to save the new time or button 1 to select a new time.");
    delay(300);
    buttonState1 = digitalRead(buttonPin1);
    if(buttonState1 == HIGH){
      delay(300);
      timeUnit = 0;
      ok = false;
    }
    else{
      buttonState2 = digitalRead(buttonPin2);
      if(buttonState2 == HIGH){
        delay(300);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Time saved");
        display.print(abs(hours));
        display.print(":");
        display.print(abs(minutes));
        display.print(":");
        display.println(abs(seconds));
        display.display();
        saveTime();
        delay(3500);
        display.clearDisplay();
        ok = true;
      }
    }
  }

  if(!ok){
    display.display();
  }
  else{
    display.clearDisplay();
    mainMenu();
  }
}

void setHours(){
  if(timeUnit == 1){
  aState = digitalRead(outputA);
  if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        hours++;
      } else {
        delay(300);
        hours--;
      }
    }
  }
}

void setMinutes(){
  aState = digitalRead(outputA);
  if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        minutes++;
      } else {
        delay(300);
        minutes--;
      }
  }
}

void setSeconds(){
  aState = digitalRead(outputA);
  if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        seconds++;
      } else {
        delay(300);
        seconds--;
      }
  }
}

void saveTime(void){
    rtc.setTime(abs(hours), abs(minutes), abs(seconds));
    hours = 0;
    minutes = 0;
    seconds = 0;
}

void setDate(void){
  //Nu stiu de ce dar "crapa" daca pun mai multe mesaje. Adica nici macar nu porneste display-ul. Porneste dupa ce sterg mesajele, deci ma gandesc ca o avea vreo problema cu memoria.
  //Desi nu da nici o eroare legata de memorie, am folost 814 bytes, si mai am 1234 bytes liberi, adica inca o data si jumatate, in total am 2048 bytes.
  //Nu mai afisez mesaje pe ecran, doar o sa comentez codu.
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  //display.setTextSize(1);
  //display.println("Current date is:");
  display.setTextSize(2);
 
  if(rtc.getDay() < 10){
    display.print("0");
    display.print(rtc.getDay());
  }
  else{
    display.print(rtc.getDay());
  }
  display.print("/");
  
  if(rtc.getMonth() < 10){
    display.print("0");
    display.print(rtc.getMonth());
  }
  else{
    display.print(rtc.getMonth());
  }
  display.print("/");
  display.println((2000 + rtc.getYear()));
  display.println("");

  buttonState1 = digitalRead(buttonPin1);
  if(buttonState1 == HIGH){
    dateUnit++;
    delay(300);
  }

  buttonState2 = digitalRead(buttonPin2);
  if(buttonState2 == HIGH){
    delay(300);
    display.clearDisplay();
    menuThree();
  }

  if(dateUnit == 1){
    display.setTextColor(BLACK, WHITE);
    if(abs(day) >= 1 && abs(day) <= 31){
      if(abs(day) < 10){
        display.print("0");
        display.print(abs(day));
      }
      else{
        display.print(abs(day));
      }
    }
    else{
      display.print("00");
      day = 0;
    }
    setDay();
  }
  else{
    display.setTextColor(WHITE);
    if(abs(day) >= 1 && abs(day) <= 31){
      if(abs(day) < 10){
        display.print("0");
        display.print(abs(day));
      }
      else{
        display.print(abs(day));
      }
    }
    else{
      display.print("00");
      day = 0;
    }
  }
  
  display.setTextColor(WHITE);
  display.print("/");

 if(dateUnit == 2){
    display.setTextColor(BLACK, WHITE);
    if(abs(month) >= 1 && abs(month) <= 12){
      if(abs(month) < 10){
        display.print("0");
        display.print(abs(month));
      }
      else{
        display.print(abs(month));
      }
    }
    else{
      display.print("00");
      month = 0;
    }
    setMonth();
  }
  else{
    display.setTextColor(WHITE);
    if(abs(month) >= 1 && abs(month) <= 12){
      if(abs(month) < 10){
        display.print("0");
        display.print(abs(month));
      }
      else{
        display.print(abs(month));
      }
    }
    else{
      display.print("00");
      month = 0;
    }
  }
  
  display.setTextColor(WHITE);
  display.print("/");

  if(dateUnit == 3){
    display.setTextColor(BLACK, WHITE);
    display.println((2000 + abs(year)));
    setYear();
  }
  else{
    display.setTextColor(WHITE);
    display.println((2000 + abs(year)));
  }
  display.setTextColor(WHITE);
  
  if(dateUnit >= 4){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("New date is:");
    display.setTextSize(2);
    display.print(abs(day));
    display.print(":");
    display.print(abs(month));
    display.print(":");
    display.println(abs(year));
    display.setTextSize(1);
    display.print("Press button 2 to save the new date or button 1 to select a new time.");
    delay(300);
    buttonState1 = digitalRead(buttonPin1);
    if(buttonState1 == HIGH){
      delay(300);
      dateUnit = 0;
      okDate = false;
    }
    else{
      buttonState2 = digitalRead(buttonPin2);
      if(buttonState2 == HIGH){
        delay(300);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Date saved");
        display.print(abs(day));
        display.print(":");
        display.print(abs(month));
        display.print(":");
        display.println(abs(year));
        display.display();
        saveDate();
        delay(3500);
        display.clearDisplay();
        okDate = true;
      }
    }
  }

  if(!okDate){
    display.display();
  }
  else{
    display.clearDisplay();
    mainMenu();
  }
  
  /*
  display.setTextSize(1);
  display.println("Change date to:");
  display.setTextSize(2);
  display.display();*/
}

void setDay(void){
  if(dateUnit == 1){
  aState = digitalRead(outputA);
  if (aState != aLastState){     
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) { 
        delay(300);
        day++;
      } else {
        delay(300);
        day--;
      }
    }
  }
}

void setMonth(void){
  if(dateUnit == 2){
    aState = digitalRead(outputA);
    if (aState != aLastState){     
        // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
        if (digitalRead(outputB) != aState) { 
          delay(300);
          month++;
        } else {
          delay(300);
          month--;
       }
     }
  }
}

void setYear(void){
  if(dateUnit == 3){
    aState = digitalRead(outputA);
    if (aState != aLastState){     
        // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
        if (digitalRead(outputB) != aState) { 
          delay(300);
          year++;
        } else {
          delay(300);
          year--;
        }
    }
  }
}

void saveDate(void){
  rtc.setDate(day, 1, month, 0, year);
  day = 0;
  month = 0;
  year = 0;
}
