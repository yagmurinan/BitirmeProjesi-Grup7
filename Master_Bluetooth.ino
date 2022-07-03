#include<SoftwareSerial.h>

int x_c, y_c;
int center = 0;

#include <Wire.h>
SoftwareSerial BTSerial(10,11); //Bluetooth modulü girişleri seçilir.
void setup() {
  Wire.begin(8);                // I2C kanalına 8 adresi ile katıl.
  Wire.onReceive(receiveEvent); // Veri alındığında çalıştırılacak fonkisyon: receiveEvent.
  Serial.begin(38400);           // 
  BTSerial.begin(38400);      //Bluetooth haberleşmesi Baud rate: 38400
}

void loop() {
  //Loop'a yazılacak bir şey bulunmamaktadır. Receive event her veri alındığında çalışır.
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // Master Arduino veri transferina hazır olana kadar bekle
    int c = Wire.read();        // receive byte as a character
    Serial.println(c);         // print the character (Kontrol amaçlı)
  }
  int x = Wire.read();    // Gönderilen veriyi tamsayı x değişkenine kaydet
  BTSerial.write(x);      //Veriyi (x) bluetooth modulü ile gönder.

}
