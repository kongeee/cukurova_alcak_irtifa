#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MMA8451.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/


#define MAX 5 //kontrolcü sayısı
#define HATA -444 // hata kodu

#define SEALEVELPRESSURE_HPA (1013.25)
#define Adafruit_MMA8451_SCL 19;
#define Adafruit_MMA8451_SDA 18;
#define Adafruit_BME280_SCL 19;
#define Adafruit_BME280_SDA 18;

Adafruit_BME280 bme1;
Adafruit_BME280 bme2;
Adafruit_MMA8451 mma = Adafruit_MMA8451();

float alt[MAX]; //5 altimetre değeri tutan array

bool ucus = true; //roketin ucusta mı düşüşte mi olduğunu anlar false->düşüşte veya durutor  true->uçuşta
bool tek_parasut_acilma = true; // parasut acildi yazısını bir kere yazmak için tanımlandı
float bme1_deger; //1. altimetreden gelen yükseklik değerini tutar
float bme2_deger; //2. altimetreden gelen yükseklik değerini tutar

void alt_init();// array elemanlarının hepsini -1 yaparak boş kalmasını sağlayan fonksiyon
float ortalama(float,float); // iki yükseklik değerinin ortalamasını alan fonksiyon
void altilere_deger_ata(); // altimetrelere gelen yükseklik değerlerini gerekli işlemlerden geçerek ve gerekli kontrolleri yaparak aktaran fonksiyon
bool parasut(); // paraşütün açılıp açılmayacağını anlamak için gerekli kontrolleri yapan fonksiyon
float bme_deger_ata(Adafruit_BME280); // bme değerlerini veren fonksiyon
bool ucusta_mi(); // ucusta mı yoksa düşüşte mi oldğunu kontrol eder
int calb1 ; //jiroskop verilerini tutar
int calb2 ;
int calb3 ;

static void smartDelay(unsigned long ms);
static void printFloat(float val, bool valid, int len, int prec);
static void printInt(unsigned long val, bool valid, int len);

static void printStr(const char *str, int len);
void displayInfo();


static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
SoftwareSerial mySerial(0, 1); // RX, TX

  

void setup() {
  Serial.begin(9600);
   mySerial.begin(9600);
  

  if (!bme1.begin(0x76)) {
    Serial.println("Could not find a valid BME2801 sensor, check wiring!");
    while (1);
  }
   if (!bme2.begin(0x77)) {
    Serial.println("Could not find a valid BME2802 sensor, check wiring!");
    while (1);
  }
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(7,OUTPUT);

   
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  mySerial.println(F("DeviceExample.ino"));
  mySerial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  mySerial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  mySerial.println(F("by Mikal Hart"));
  mySerial.println();



  Serial.println("Adafruit MMA8451 test!");
  mySerial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  mySerial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");

  mySerial.print("Range = "); mySerial.print(2 << mma.getRange());  
  mySerial.println("G");
}



void loop() {  

  
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());

    mma.read();
  calb1 = map(mma.x,-4096,4096,-180,+180);
  calb2 = map(mma.y,-4096,4096,0,360);
  calb3 = map(mma.z,-4096,4096,0,360); 
   
  Serial.print("X:\t"); Serial.print(calb1); 
  Serial.print("\tY:\t"); Serial.print(calb2); 
  Serial.print("\tZ:\t"); Serial.print(calb3); 
  Serial.print("  -----  ");

  mySerial.print("X:\t"); mySerial.print(calb1); 
  mySerial.print("\tY:\t"); mySerial.print(calb2); 
  mySerial.print("\tZ:\t"); mySerial.print(calb3); 
  mySerial.print("  -----  ");

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.print("m/s^2 ");
  Serial.print("-----");

  mySerial.print("X: \t"); mySerial.print(event.acceleration.x); mySerial.print("\t");
  mySerial.print("Y: \t"); mySerial.print(event.acceleration.y); mySerial.print("\t");
  mySerial.print("Z: \t"); mySerial.print(event.acceleration.z); mySerial.print("\t");
  mySerial.print("m/s^2 ");
  mySerial.print("-----");
   /*mySerial.println("Hello, world?");
    mySerial.println("Hello, world?");
     mySerial.println("Hello, world?");
      mySerial.println("Hello, world?");*/
  
  
  /* Get the orientation of the sensor */
  uint8_t o = mma.getOrientation();
  
  switch (o) {
    case MMA8451_PL_PUF: 
      Serial.print("Portrait Up Front");
      mySerial.print("Portrait Up Front");
      break;
    case MMA8451_PL_PUB: 
      Serial.print("Portrait Up Back");
      mySerial.print("Portrait Up Back");
      break;    
    case MMA8451_PL_PDF: 
      Serial.print("Portrait Down Front");
      mySerial.print("Portrait Down Front");
      break;
    case MMA8451_PL_PDB: 
      Serial.print("Portrait Down Back");
      mySerial.print("Portrait Down Back");
      break;
    case MMA8451_PL_LRF: 
      Serial.print("Landscape Right Front");
      mySerial.print("Landscape Right Front");
      break;
    case MMA8451_PL_LRB: 
      Serial.print("Landscape Right Back");
      mySerial.print("Landscape Right Back");
      break;
    case MMA8451_PL_LLF: 
      Serial.print("Landscape Left Front");
      mySerial.print("Landscape Left Front");
      break;
    case MMA8451_PL_LLB: 
      Serial.print("Landscape Left Back");
      mySerial.print("Landscape Left Back");
      break;
    }
  Serial.println();
  Serial.println();
  
  mySerial.println();
  mySerial.println();

  

  static const double LAB_LAT = 37.032263, LAB_LON = 35.352589;//Lab konumu
    /*while (ss.available() > 0)
    if (gps.encode(ss.read()))*/
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  
  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LAB_LAT, 
      LAB_LON) / 1000;
  Serial.print("Laba uzaklik = ");
  
  mySerial.print("Laba uzaklik = ");
  printInt(distanceKmToLondon, gps.location.isValid(), 9);


  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LAB_LAT, 
      LAB_LON);
  Serial.print("Hangi derecede  = ");
  mySerial.print("Hangi derecede  = ");
  printFloat(courseToLondon, gps.location.isValid(), 7, 2);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
  Serial.print("Hangi yonde = ");
  mySerial.print("Hangi yonde = ");
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

 /* printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);*/
  Serial.println();
  mySerial.println();
  smartDelay(200);

  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".

  Serial.println("******************************************************");
  mySerial.println("******************************************************");
  
  alt_init();
  altilere_deger_ata(); 
  delay(100);
  
 
  if(parasut() && !ucus && tek_parasut_acilma){//parasut doğruysa ve inişteyse
    Serial.println("PARASUT AC//////////////////");
    mySerial.println("PARASUT AC//////////////////");
   tek_parasut_acilma = false;//parasut aç komutu bir kere geldiğinde birdaha parasutu açmaması için
   digitalWrite(10,HIGH);
   digitalWrite(9,LOW);
   digitalWrite(7,LOW);
   delay(5000);
   
    
    
   }
   if(!parasut()&& !ucus){Serial.println("PARASUT ACMAAAAAAAA");
   mySerial.println("PARASUT ACMAAAAAAAA");
   digitalWrite(9,HIGH);
   digitalWrite(10,LOW);
   digitalWrite(7,LOW);
   
   }
   if(!parasut() && ucus){
    digitalWrite(9,HIGH);
   digitalWrite(10,LOW);
   
    }
    if(!tek_parasut_acilma){
    ucus = false;//eğer parasüt açılmışsa inişte demektir ve ucus değişkeninin hep false olması gerekir
    }
    if(ucus){
      digitalWrite(7,HIGH);
      }
    if(!ucus){
      digitalWrite(7,LOW);
      }
  delay(100);
  Serial.println();
  mySerial.println();
  
  
}
float bme_deger_ata(Adafruit_BME280 bme){
  
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  
}//BME DEĞER ATA FONKSİYONUNUN SONU

void alt_init(){ //altimetre arrayinin içini boşaltır
  int i;
  for(i=0;i<MAX;i++){ // 5 kere döner
    alt[i] = -1;
  }
  
}//İNİT FONKSİYONUNUN SONU

float ortalama(float bme1,float bme2){//iki altimetreden gelen değerin ortlamasını alan fonksiyon
  if(abs(bme1 - bme2)>100.0){ // eğer iki altimetre arasındaki fark(yukseklik) 100mt den büyükse hata oluştuğunu bildir
    return HATA; // hata olduğunu anlamak için -444 dönder
    
  }
  
  return (bme1 + bme2)/2.0; // eğer hata yoksa iki değerin ortalmasını dön
  
} // ORTALAMA FONKSİYONUNUN SONU

void altilere_deger_ata(){ // altimetre değerlerini atamak için fonksiyon
  int i = 0; 
  float hatali_mi; // ortalama fonksiyonundan dönen değerin hatalı olup olmadığına bakar
  
  for(i=0;i<MAX;i++){ //tüm altimetre kontrol değişkenlerine değer ata
    bme1_deger = bme_deger_ata(bme1);//1.bme yi yazdir fonksiyonuna yolla ve bme1_deger e yüksekliği aktar
    
    bme2_deger = bme_deger_ata(bme2);//2.bme yi yazdir fonksiyonuna yolla ve bme2_deger e yüksekliği aktar
    
    hatali_mi = ortalama(bme1_deger,bme2_deger); //hatalı olup olmadığını anla,ortalamayı tutar
    
    if(hatali_mi == HATA && i =0){ // eğer ortlama fonk hata dönerse
      
      Serial.println("HATA!!!"); 
      mySerial.println("HATA!!!"); 
      Serial.print("1-)");
      Serial.println(bme1.readAltitude(SEALEVELPRESSURE_HPA));
      Serial.print("2-)");
      Serial.println(bme2.readAltitude(SEALEVELPRESSURE_HPA));
      i = i-1; //hatali bir durumda döngü hatalı duruma tekrar döner

    }
    
    else{ // hata yoksa 
      
      Serial.println(hatali_mi);
      mySerial.println(hatali_mi);
      alt[i] = hatali_mi; // alt ın i inci elemanına dönen değer yani ortalama değerini yaz
    }
    delay(100);
  }
  Serial.println();
  mySerial.println();
  
}//DEĞER ATAMA FONKSİYONUNUN SONU

bool ucusta_mi(){
  int i;
  int ucus_counter = 0; // karşılaştıran değerleri saymak için sayaç
  for(i=0 ; i<MAX-1 ; i++){//max-1 kere dön
    if(alt[i]<alt[1+i]+5){ //mevcut değer bir sonraki değerden küçükse(5 mt kadar dururken iniş durumuna geçmemesi için) yükselişte demektir
      continue; // devam et
    }
    else{// mevcut değer bir sonraki değerden büyükse iniştedir
      ucus_counter++;// sayacı artır
      }
    }
    if(ucus_counter == MAX-1){// eğer sürekli yukarıdaki else girerse ve counter MAX-1 sayısına ulaşırsa düşüşte demektir
      return false;// yanlış yani inişte dönder
      
      }
      return true;// doğru yani ucuşta dönder
  
  }// UCUSTA MI FONKSİYONUNUN SONU

bool parasut(){//parasut acilma durumunu kontrol eder
  int i;
 // unsigned int parasut_counter = 0; // inişteyken değerler arasında belirlenen farktan az bir fark varsa bu kontrolcü ile kontrol edilir
  unsigned int counter = 0; //arrayin elemanlarının büyüklük küçüklük ilişkisini anlamak için oluşturulan değişken
  if(alt[0]<alt[1]){//yukarı çıkıyor demektir diğer değerlere bakmaya gerek yok (hızlı olması için)
    ucus = ucusta_mi();
    return false;
  }
  for(i=0 ; i<MAX-1 ; i++){
    if(!ucusta_mi() && (alt[i] - alt[1+i]>50.0)){//eğer önceki değer sonraki değerden büyükse düşüyor demektir tüm altiler için kontrol edilir(and den sonrası aradaki fark)
      counter++; //bu değişken eğer tüm bir önceki değerler bi sonrakinden büyükse aşağıdaki kontrol için var
      
    }
  }
  if(counter >= MAX-1){// max -1 dedik çünkü 5 sayı olursa alt[5] 4 karşılaştırma olur 
    
    ucus = ucusta_mi();
    return true; //eğer tüm karşılaştırmalar doğruysa parasut acilir düşüş anı
  }
  
  ucus = ucusta_mi();
  return false;
  
  
}//Fonksiyon sonu
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
    mySerial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    mySerial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
      mySerial.print(' ');
  }
  smartDelay(0);
}
static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  mySerial.print(sz);
  smartDelay(0);
}


static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i){
    Serial.print(i<slen ? str[i] : ' ');
    mySerial.print(i<slen ? str[i] : ' ');}
  smartDelay(0);}

  void displayInfo()
{
  Serial.print(F("Konum: "));
  mySerial.print(F("Konum: ")); 
  if (gps.location.isValid())
  {
    Serial.print("1.koordi =");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("2.koordi =");
    Serial.print(gps.location.lng(), 6);

    mySerial.print("1.koordi =");
    mySerial.print(gps.location.lat(), 6);
    mySerial.print(F(","));
    mySerial.print("2.koordi =");
    mySerial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
    mySerial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  mySerial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());

    mySerial.print(gps.date.month());
    mySerial.print(F("/"));
    mySerial.print(gps.date.day());
    mySerial.print(F("/"));
    mySerial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
    mySerial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  mySerial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) {Serial.print(F("0"));mySerial.print(F("0"));}
    Serial.print(gps.time.hour());
    Serial.print(F(":"));

    mySerial.print(gps.time.hour());
    mySerial.print(F(":"));
    if (gps.time.minute() < 10) {Serial.print(F("0"));mySerial.print(F("0"));}
    Serial.print(gps.time.minute());
    Serial.print(F(":"));

    mySerial.print(gps.time.minute());
    mySerial.print(F(":"));
    if (gps.time.second() < 10) {Serial.print(F("0"));mySerial.print(F("0"));}
    Serial.print(gps.time.second());
    Serial.print(F("."));

    mySerial.print(gps.time.second());
    mySerial.print(F("."));
    if (gps.time.centisecond() < 10) {Serial.print(F("0"));mySerial.print(F("0"));}
    Serial.print(gps.time.centisecond());
    mySerial.print(gps.time.centisecond());

    
    /*if (gps.time.hour() < 10) mySerial.print(F("0"));
    mySerial.print(gps.time.hour());
    mySerial.print(F(":"));
    if (gps.time.minute() < 10) mySerial.print(F("0"));
    mySerial.print(gps.time.minute());
    mySerial.print(F(":"));
    if (gps.time.second() < 10) mySerial.print(F("0"));
    mySerial.print(gps.time.second());
    mySerial.print(F("."));
    if (gps.time.centisecond() < 10) mySerial.print(F("0"));
    mySerial.print(gps.time.centisecond());*/
  }
  else
  {
    Serial.print(F("INVALID"));
    mySerial.print(F("INVALID"));
  }

  Serial.println();
  mySerial.println();
}

  
