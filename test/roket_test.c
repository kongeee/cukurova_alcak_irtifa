/*ÇUKUROVA ÜNİVERSİTESİ ALÇAK İRTİFA ROKET TAKIMI MERKÜT ROKETİ TEST YAZILIMIDIR/
/*04.05.2020*/
/*Yazan: Furkan Ekici*/
#include <stdio.h> // standart kütüphane
#include <math.h>  // matematik kütüphanesi
#define MAX 5 // kaç adet veri alınacağı
#define HATA -444 //Belirlenmiş hata kodu.
#define UCUS_MIN 3.0 //uçuş fonksiyonunda durma toleransı
#define PARASUT_MAX 50.0 // en az bu kadar mesafe varsa her veri arasında paraşüt açılır
#define ORTALAMA_FARK 100.0 //ortalama fonksiyonunda farkın kaçtan büyük olursa hata döneceğini söyler

float veri[MAX]; //Değerlerin tutulacağı dizi.
float alti_1;//birinci altimetreden gelen değer
float alti_2;//ikinci altimetreden gelen değer

void bme_degeri_al(int); //Birinci ve ikinci altimetreden veri almayı sağlar.
void init_alti();//Altimetre dizisini ilk duruma getirir.(içini boşaltır)
float ortalama();//Ortalama hesaplayan fonksiyon.
void alti_deger_ata();//Gerekli kontrolleri yaparak altimetre dizisine eleman ekler.
int parasut();//Paraşütün açılıp açılmadığını kontrol eder.
int ucus();//Roketin uçuşta mı yoksa düşüte mi oldugunu söyler.
void verileri_goster();//veri dizisinin içeriği gösterir

//MAIN
int main(){
	int secenek;//döngü koşulu
	int ucus_durumu; //1 ise ucuyor veya duruyor , 0 ise düşüşte
	int parasut_durumu; // 0 ise parasutu acma , 1 ise ac 
	while(1){
		printf("Teste devam etmek istiyor musunuz? (e(1)/h(2)): "); scanf("%d",&secenek);
		if(secenek == 2){//seçenek hayırsa
			break;// döngüden çık
		}
		
		init_alti(); //Altimetre dizisini ilk haline çeviriyor.(önceki değerleri siliyor yani)
		alti_deger_ata();//altimetre değerlerini al ve veriler dizisine yaz
		
		parasut_durumu = parasut(); // paraşüt durumunu al
		ucus_durumu = ucus();       // uçuş durumunu al
		
		if(parasut_durumu && !ucus_durumu){ //parasüt açıksa ve düşüteyse
			puts("Parasut = Acik\nUcus Durumu = Dusuyor");
		}
		
		
		else if(!parasut_durumu && !ucus_durumu){ //paraşüt kapalı ve düşüyor
			puts("Parasut = Kapali\nUcus Durumu = Dusuyor");
		}
		else{//paraşüt açık yükselemeyeceği için
			puts("Parasut = Kapali\nUcus Durumu = Ucuyor veya duruyor");
		}
	}	
	puts("Program sonlandirildi.");
		
	

}
//MAIN SONU

//BME_DEGERI_AL FONKSIYONU BASLANGIC
void bme_degeri_al(int i){
	printf("%d-)Birinci altimetreden gelen deger :",i+1); scanf("%f",&alti_1); //Birinci altimetreden gelen değer alındı.
	printf("%d-)Ikinci altimetreden gelen deger  :",i+1); scanf("%f",&alti_2); //İkinci altimetreden gelen değer alındı.
}
//BME_DEGERİ_AL FONKSIYONU SONU

//INIT_ALTI FONKSIYONU BASLANGIC
void init_alti(){ //veri dizisini ilk duruma getirir.
	unsigned int i;//sayaç
	for(i=0;i<MAX;i++){//i, MAX'tan küçük olduğu sürece 
		veri[i] = -1;//veri dizisinin i. elemanına -1 değeri aktar.
	
	}
}
//INIT_ALTI FONKSIYONU SONU

//ORTALAMA FONKSIYONU BASLANGIC
float ortalama(){
	if(fabs(alti_1 - alti_2) > ORTALAMA_FARK){//eğer altimetreler arasındaki değer 100mt'den fazla ise
		return HATA;//Hata döndür
	}
	return (alti_1 + alti_2)/2.0;//değilse ortalamalarını döndür.
}
//ORTALAMA FONKSIYONU SONU

//VERILAERI_GOSTER FONKSIYONU BASLANGIC
void verileri_goster(){
	unsigned int i;
	printf("[ ");
	for(i=0;i<MAX;i++){
		printf("%f ",veri[i]);
	}
	puts("]\n");
}
//VERILER GOSTER FONKSIYONU SONU

//ALTI_DEGER_ATA FONKSIYONU BASLANGIC
void alti_deger_ata(){
	float deger;// ortalama fonksiyonundan dönen değeri tutar
	int i;//sayaç
	for(i=0;i<MAX;i++){
		bme_degeri_al(i); //Sensörden gelen değerler.
		deger = ortalama(); // ortalamayi degere aktarır
		
		if(deger == HATA){ // deger hatalı ise
			puts("HATA!!\n");
			i = i-1; 
		}
		else{ // hiçbir hata yoksa
			veri[i] = deger; //ortalama fonksiyonundan dönen değeri verilerin içine kaydet
		}
		
	}
	verileri_goster(); //döngü bittiği zaman veriler dizisi dolmuş demektir
			   //bu diziyi yazdır	
	
}
//ALTI_DEGER_ATA FONKSIYONU SONU

//PARASUT_FONKSIYONU BASLANGIC
int parasut(){
	unsigned int parasut_sayac = 0;
	unsigned int i;
	for(i=0;i<MAX-1;i++){//max-1 lik bir döngü kur
		if(veri[i] - veri[1+i] > PARASUT_MAX){//eğer ilk gelen veri ikinci gelen veriden en az 40mt daha fazla ise
			parasut_sayac++;//parasut acilma sayacısını artır
		}
	}
	if(parasut_sayac == MAX-1){//parasüt sayac max-1 ise yani tüm degerler arasındaki fark 40 tan fazla ise
		return 1; //1(parasüt aç) dönder
	}
	return 0; //değilse 0(paraşüt açma) dönder
	
}
//PARASUT_FONKSIYONU SONU

//UCUS FONKSIYONU BASLANGIC
int ucus(){//parasut fonksiyonuna benziyor
	unsigned int ucus_sayac = 0;
	unsigned int i;
	
	for(i=0;i<MAX-1;i++){
		if(veri[i] - veri[1+i] > UCUS_MIN){ //bir önceki değer bir sonrakinden en az 3 fazla ise düşüşte demektir
					     //Böyle olmasının sebebi sensörlerin durma anında ufak değişikliklerinin olması
			ucus_sayac++;//uçma sayacını artır
		}
		
	}
	if(ucus_sayac == MAX-1){
		return 0;//düşüyor demektir
	}
	return 1;       //uçuyor veya duruyor
	
}
//UCUS FONKSYONU SONU	












