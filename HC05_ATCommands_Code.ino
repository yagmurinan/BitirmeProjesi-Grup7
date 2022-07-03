/*  Bluetooth AT Commands
 *  Uğur ALTINIŞIK
 *  Nazif Orhun TEKCİ
 *  Bu program Bluetooth Modulu ayar modunun yuklenmesini ve degistirilmesine olanak saglanmasini icerir.
 */

#include <SoftwareSerial.h> //haberlesme kutuphanesi tanimlanmistir.
SoftwareSerial BluetoothSw(10, 11); //Arduino UNO'nun 10 ve 11. pinleri sirasi ile Arduino UNO'nun RX TX pini olarak tanimlanmistir

void setup() 
{
  Serial.begin(9600); //Serial Port Baud Rate degeri tanimlanmistir.
  Serial.println("Enter AT Commands:"); //Serial Port Yazi yazdirilmistir
  BluetoothSw.begin(38400); //Bluetooth modul default Baud Rate degeri atanmistir
}
void loop() 
{
  if(BluetoothSw.available()) //Bluetoothtan veri geliyorsa
  {
    Serial.write(BluetoothSw.read()); //veriyi okur ve Serial Port'a yazar
  }
  if(Serial.available()) //Serial Port'tan veri geliyorsa
  {
    BluetoothSw.write(Serial.read()); //veriyi okur ve Bluetooth ile gonderir
  }
}

//Ayar modu icin Serial Porta AT+<KOMUT>? ile mevcut bilgi ogrenilir, mevcut bilgi degistirilecekse AT+<KOMUT>=<DEGER> ile durum degistirilir
//ORNEK: AT+NAME=BitirmeGrup7AA -> Bluetooth modulunun adi BitirmeGrup7AA olur
