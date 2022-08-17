//Zwyciestwo!!

#include <Servo.h>

//#define wlacznik 12
//#define tranzystor 11
#define czujnikObecnosci A0
#define czujnikObecnosci1 A1
#define czujnikObecnosci2 A2
#define iloscProbek 20

Servo serwo1, serwo2;
int pozycjaNieAktywna = 0, pozycjaAktywna = 70;
int wykrycie = 0, wykrycie1 = 0, wykrycie2 = 0, prog = 880;
int s0 = 13, s1 = 4, s2 = 7, s3 = 8; //piny modułu połączone z Arduino
int out = 2;                      //pin 2 Arduino i OUT od czujnika
int flag = 0, flaga=0;
int counter = 0;
int countR = 0, countG = 0, countB = 0;
float suma =0, srednia = 0, progCzerwony = 1050, sumaB =0, sredniaB = 0;
float progNiebieski = 1000;
int i = 0, iB = 0;
void setup()
{
  Serial.begin(9600);             //inicjalizacja monitora szeregowego
  pinMode(s0, OUTPUT);            //ustawienie pinów jako wyjścia
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  serwo1.attach(9);
  serwo2.attach(10);
  serwo1.write(pozycjaNieAktywna);
  serwo2.write(pozycjaNieAktywna);
  Serial.println("\t :)");
}
void TCS()
{
  flag = 0;
  digitalWrite(s1, HIGH); //100% 500 - 600 kHz typowo 600
  digitalWrite(s0, HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  attachInterrupt(0, ISR_INTO, LOW); // Zliczanie counter++; digitalPinToInterrupt(2) zamiast 0
  timer0_init();     //odpalanie reszty fubkcji :)
 
}
void ISR_INTO()
{
  counter++;
}
void timer0_init(void)
{
  TCCR2A = 0x00;
  TCCR2B = 0x07; //częstotliwość zegara - 1024
  TCNT2 = 100;   //przepełnienie 10 ms
  TIMSK2 = 0x01; //pozwolenie na przerwanie
}

ISR(TIMER2_OVF_vect)//timer 2, 10ms przepełnienie 10ms. funkcja przerwanie wewnętrzengo przepełnienia
{
  TCNT2 = 100; // te silniki co trzeszcza to chyba przez to :/
  flag++;
  wykrycie = analogRead(czujnikObecnosci);
  wykrycie1 = analogRead(czujnikObecnosci1);
  wykrycie2 = analogRead(czujnikObecnosci2);
  if (flag == 1 && wykrycie2 > 875 && i < iloscProbek)                 //pobranie i wyświetlenie na ekranie składowych RGB
  {
    i++;
    countR = counter; //czerwony
    suma += countR;
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
 
  }
  else if (flag == 2 && wykrycie2 > 875 && iB < iloscProbek)
  {
     iB++;
    countB = counter;
    sumaB += countB;
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
 
  }
  else if (flag == 3) // To jest potężne narzedzie, ktore sprawia,
  {                   //ze czujnik koloru przestaje mierzyc
    flag = 0;
  }
  else if(i == iloscProbek)
  {
    srednia = suma/iloscProbek;
    Serial.print("sr czerwony=");
    Serial.println(srednia, DEC);
    suma = 0;
    i = 0;
  }
  else if(iB == iloscProbek)
  {
    sredniaB = sumaB/iloscProbek;
    Serial.print("sr niebieski=");
    Serial.println(sredniaB, DEC);
    sumaB = 0;
    iB = 0;
  }
  if (srednia > progCzerwony && wykrycie > prog)// mozna by tu wykozystac flage
    {
      serwo1.write(pozycjaAktywna);
      Serial.println("\t serwo1");
      flaga = 1;
    }
  else  if (sredniaB > progNiebieski && wykrycie > prog  )// mozna by tu wykozystac flage
    {
      serwo2.write(pozycjaAktywna);
      Serial.println("\t serwo2");
      flaga = 1;
    }
  if (wykrycie1 > prog && flaga > 0)// mozna by tu wykozystac flage
    {
      serwo1.write(pozycjaNieAktywna);
      serwo2.write(pozycjaNieAktywna);
      Serial.println("\t dezaktywacja!!! :D");
      flaga = 0;
    }
    
  counter = 0;
  
}
void loop()                    //główna pętla programu
{
 /* if(digitalRead(wlacznik) == HIGH)
  {
    analogWrite(LED_BUILTIN, 255);
    analogWrite(tranzystor, 255);
  }
  else
  {
    analogWrite(LED_BUILTIN, 0);
    analogWrite(tranzystor, 0);
  }*/
  TCS();
  while(1);
}
