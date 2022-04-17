//Arduino Stopwatch Project

#include <LiquidCrystal.h>;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

bool isStopwatch = false, isTimer = false;

const int pushbtn1 = 9; // pentru ceas
const int pushbtn2 = 8; // pentru navigare meniu
const int pushbtn3 = 7; // pentru timer
const int pushbtn4 = 6; // pentru cronometru
const int led = 13; //pentru LED

int hour, minute, second, mls = 0, lastTime = 0, now;
int mode = 0; //modurile de functionare
int k = 0; //pentru citire ora 
int var; // variabila navigare meniu

//state-urile butoanelor
int btnstate1; 
int btnstate2;
int btnstate3;
int btnstate4;


void setup() {
  lcd.begin(16, 2);
  
  pinMode(pushbtn1, INPUT);
  pinMode(pushbtn2, INPUT);
  pinMode(pushbtn3, INPUT);
  pinMode(pushbtn4, INPUT);
  pinMode(led, OUTPUT);
  
  Serial.begin(9600);
}

void loop(){
  if(mode==0){
    if(k<=0)
      readClock(); // citim ora din serial monitor
    clock(); // afisam ora + meniul
  }
  else if(mode==2){
    timer(); // intram in modul timer
  }
  else if(mode==1){
    stopwatch(); // intram in modul cronometru
  }
}


//FUNCTII APASARE BUTOANE
void checkState(){
  btnstate1=digitalRead(pushbtn1);
  btnstate2=digitalRead(pushbtn2);
  btnstate3=digitalRead(pushbtn3);
  btnstate4=digitalRead(pushbtn4);
  
  if(btnstate1|btnstate2|btnstate3|btnstate4) { // verificam daca s-a apasat un buton
     if(btnstate1==1){
       mode=0; //MENIU
     }
     if(btnstate2==1){
       var = var + 1; // FUNCTIONALITATI
     }
     if(btnstate3==1){
       mode=2; //TIMER
     }
     if(btnstate4==1){
       mode=1; //CRONOMETRU
     }
    }
  else {
        mode=0; //MENIU
    }
}


//FUNCTII PENTRU CEAS
void line() { //scrie o linie noua in serial monitor
  Serial.println("");
}

void readClock(){ // citim ora introdusa de la serial monitor
  Serial.println("Enter current hour:");
  while(Serial.available()==0);
    hour=Serial.parseInt();
    Serial.println(hour);
  line();
  
  Serial.println("Enter current minute:");
  while(Serial.available()==0);
    minute=Serial.parseInt();
    Serial.println(minute);
  line();

  Serial.println("Enter current second:");
  while(Serial.available()==0);
    second=Serial.parseInt();
    Serial.println(second);
  line();

  k=k+1;
}

void clock(){ // afisare ora
  lcd.setCursor(0,0);
  lcd.print("Ora: ");
  
  if(hour<10)
    lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  
  if(minute<10)
    lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  
  if(second<10)
    lcd.print("0");
  lcd.print(second);
  
  // afisare meniu
  if (var == 0){
      lcd.setCursor(0,1);
        lcd.print("MENIU           ");  
    } else if (var == 1) {
      lcd.setCursor(0,1);
      lcd.print(var);
      lcd.print(" - Stopwatch    ");
    } else if (var == 2){
      lcd.setCursor(0,1);
      lcd.print(var);
      lcd.print(" - Timer        ");
    } else {
        lcd.setCursor(0,1);
        lcd.print("MENIU           ");
      var = 0;
    } 
 
 checkState(); // verificam la fiecare 200ms daca s-a apasat un buton
 delay(200);
 runningClock(&hour, &minute, &second, &now, &lastTime, &mls); // apelam functia de actualizare ceas
}

void runningClock(int* _hour,int* _minute, int* _second, int* _now, int* _lastTime, int* _mls){
 int increHour = 0; // increment pt ora
 *_now = (millis()); // calculam cat timp a trecut de la pornire
 *_mls += (*_now-*_lastTime);
 *_lastTime = *_now; // actualizam last_time

 if(*_mls >= 1000){
  *_second += (*_mls/1000); //cresc secundele cu 1
  *_mls %= 1000; // se reseteaza milisecundele
  }
  
 if(*_second >= 60){
   *_minute += *_second/60; // cresc minutele cu 1
   *_second %= 60; // se reseteaza secundele
  }
  
 if(*_minute >= 60){
   increHour = *_minute/60; // cresc orele cu 1
    *_minute %= 60; // se reseteaza minutele
  }
  
  if(increHour % 24==0){ // se reseteaza incrementul pt ora
    increHour = 0;
  } 
  else { // se actualizeaza ora
    increHour %= 24;
    if(*_hour == 24){
      *_hour = 0;
    } else if(*_hour + increHour >= 24){
      *_hour = 0;
    } else{
      *_hour=((*_hour + increHour) % 24);
    }
 }
}

//FUNCTII PENTRU TIMER
void timer() {
  int minuteTimer=0, secondTimer=0, mlsTimer=0, lastTimeTimer;
  int *main_var; // variabila pentru a schimba minutele si secundele pe timer
  lcd.clear();
  int LedFlag=0; //variabila pt LED
  int flag2=0; // variabila pt LED
  int currentChangeIn = 2; //variabila pt minute/secunde (2-minute, 3-secunde)
  
  while(true) 
  {
    lcd.setCursor(0,0);
    lcd.print("Timer");
    
    lcd.setCursor(0,1);
    if(minuteTimer<10)
      lcd.print("0");
    lcd.print(minuteTimer);
    lcd.print(":");
    
    if(secondTimer<10)
      lcd.print("0");
    lcd.print(secondTimer);
    
    btnstate1=digitalRead(pushbtn1);
    btnstate2=digitalRead(pushbtn2);
    btnstate3=digitalRead(pushbtn3);
    btnstate4=digitalRead(pushbtn4);
    
    if(btnstate1==1){ // Inapoi in MENIU
      mode=0;
      delay(200);
      break;
    }
    if(btnstate3==1){ // Schimbam intre minute si secunde
      currentChangeIn+=1; // se trece in secunde
      if(currentChangeIn==4)
        currentChangeIn=2; // ne intoarcem la minute
      delay(200);
    }
    if(currentChangeIn==2){ // setam minutele
      main_var=&minuteTimer;
     
      if(digitalRead(pushbtn2)==1){
        minuteTimer+=1;
        if(minuteTimer==60)
          minuteTimer=0;
        delay(200);
      }
    }
    if(currentChangeIn==3){ // setam secundele
      main_var=&secondTimer;
     
      if(digitalRead(pushbtn2)==1){
        secondTimer+=1;
        if(secondTimer==60)
          secondTimer=0;
        delay(200);
      }
    }
    if(btnstate4==1){ // buton pentru start timer
      if(!isTimer)
        isTimer=true;
      else{
        isTimer=false;
      }
      LedFlag=0;
      flag2 = 1;
      lastTimeTimer=millis();
      delay(200);
    }
    if(isTimer)
      runningTimer(&minuteTimer, &secondTimer, &mlsTimer, &lastTimeTimer);
    if(minuteTimer==0&secondTimer==0){ // daca timerul a ajuns la 0:0
        LedFlag=1; // se aprinde LED-ul
    }
    if(LedFlag==1 && flag2 == 1){
      digitalWrite(13, HIGH);
        delay(300);
        digitalWrite(13, LOW);
        delay(300);
        digitalWrite(13, HIGH);
        delay(300);
        digitalWrite(13, LOW);
        delay(300);
        digitalWrite(13, HIGH);
        delay(300);
        digitalWrite(13, LOW);
        delay(300);
        flag2 = 0;
    }
    
    delay(200);
    runningClock(&hour, &minute, &second, &now, &lastTime, &mls); // calculam in continuare ora, ca sa ni se actualizeze in background
  }
}

void runningTimer(int* _minute, int* _second, int* _mls, int* _lastTime){
 int _now = millis();
 *_mls += (_now - *_lastTime);
 *_lastTime = _now;
  
 if(*_mls >= 1000){ // la fiecare 1000 ms scadem o secunda din timer
  *_second -= (*_mls/1000); 
  *_mls %= 1000;
  }
  
 if(*_second <= 0){
   if(*_minute == 0 & *_second < 0){
    isTimer=false; // oprim timerul la 00:00
    
    delay(3000);
    *_second=0;
  }
   else if(*_minute>0){ // actualizam minutele de pe timer cand secundele ajung la 0
    *_minute-=1;
    *_second=59;
   }
 }
}



//FUNCTII PENTRU CRONOMETRU
void stopwatch(){
  int hourStopwatch=0, minuteStopwatch=0, secondStopwatch=0, mlsStopwatch=0, lastTimeStopwatch=0;
  int cnt=0;
  lcd.clear();
  
  while(true) 
  {
    lcd.setCursor(0,0);
    lcd.print("Stopwatch");
    
    lcd.setCursor(0,1);
    if(hourStopwatch<10)
      lcd.print("0");
    lcd.print(hourStopwatch);
    lcd.print(":");
    
    if(minuteStopwatch<10)
      lcd.print("0");
    lcd.print(minuteStopwatch);
    lcd.print(":");
    
    if(secondStopwatch<10)
      lcd.print("0");
    lcd.print(secondStopwatch);
    
    btnstate1=digitalRead(pushbtn1);
    btnstate2=digitalRead(pushbtn2);
    btnstate3=digitalRead(pushbtn3);
    btnstate4=digitalRead(pushbtn4);
    
    if(btnstate1==1){ //Inapoi in MENIU
      mode=0;
      delay(200);
      break;
    }
    
    if(btnstate2==1){ // butonul de Start cronometru
      isStopwatch=true;
      lastTimeStopwatch=millis();
      delay(200);
    }
    
    if(btnstate3==1){ // butonul de Stop cronometru
      //cand apasam Stop, afisam pe Serial monitor timpul de pe stopwatch
      cnt++;
      isStopwatch=false;
      Serial.print(cnt);
      Serial.print(" - ");
      Serial.print(hourStopwatch);
      Serial.print(":");
      Serial.print(minuteStopwatch);
      Serial.print(":");
      Serial.print(secondStopwatch);
      line();
      delay(200);
    }
    
    if(btnstate4==1){ // butonul de Reset cronometru
      isStopwatch=false;
      hourStopwatch=0;
      minuteStopwatch=0;
      secondStopwatch=0;
      cnt = 0;
      delay(200);
    }
    
    if(isStopwatch)
      runningStopwatch(&hourStopwatch, &minuteStopwatch, &secondStopwatch, &lastTimeStopwatch, &mlsStopwatch);
  
    runningClock(&hour, &minute, &second, &now, &lastTime, &mls); // calculam ora curenta in contnuare
  }
}

void runningStopwatch(int* _hour,int* _minute, int* _second, int* _lastTime, int* _mls){
 int increHour=0, _now;
  
 _now=(millis());
 *_mls+=(_now-*_lastTime);
 *_lastTime=_now;

 if(*_mls >= 1000){
  *_second+=(*_mls/1000); 
  *_mls%=1000;
  }
  
 if(*_second >= 60){
   *_minute+=*_second/60;
   *_second%=60;
  }
  
 if(*_minute >= 60){
   *_hour=*_minute/60;
    *_minute%=60;
  }
  
  if(*_hour>=24){ // cronometreaza maxim 24 ore
    isStopwatch=false;
  }
}
