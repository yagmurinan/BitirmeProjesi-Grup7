#include <SoftwareSerial.h>     //Bluetooth haberlesme icin kutuphane tanimlanmistir.
#include <LiquidCrystal_I2C.h>  //LCD icin kutuphane tanimlanmistir.
LiquidCrystal_I2C LCD(0x27,16,2); //LCD kutuphanesine mainde kullanmak icin isim atanmis ve init fonksiyon olarak 
SoftwareSerial BluetoothSw(10, 11);  //Bluetooth kutuphanesine mainde kullanmak icin isim atanmis ve init fonksiyon olarak Arduino UNO üzerindeki pinleri sirasiyla RX ve TX olarak tanimladik
int coordinate; //Bluetooth ile alinan koordinat bilgilerini kaydetmek icin global degisken tanimlanmistir

void setup() 
{
  BluetoothSw.begin(38400); //haberlesme 38400 baudrate ile baslamistir
  LCD.init();               //LCD aktif hale getirilmistir
  LCD.backlight();          //LCD arka isigi aktive hale getirilmistir
  LCD.setCursor(0, 0);      
  LCD.print("X: ");         //LCD uzerindeki 1. satir 1. sutun konumundan itibaren <X: > yazilacak
  LCD.setCursor(0, 1);
  LCD.print("Y: ");         //LCD uzerindeki 2. satir 1. sutun konumundan itibaren <Y: > yazilacak
  //Son 4 LCD komutunun setup() icerisinde yapilmis olmasi yazilan yazilarin operasyon boyunca hic degismeyecek olmasindan kaynaklanmaktadir.
}

void loop() 
{
  if(BluetoothSw.available()>0) //Bluetootha veri gelip gelmedigini kontrol eder
  {
    coordinate = BluetoothSw.read();  //Bluetoothtan gelen veriyi coordinate degiskenine kaydeder
    if (coordinate < 64)              //gonderilen veriler haritalandirmaya gore [0,64] araligindadir. Gelen veriler 0010_0000 degerinden kucukse x coordinatina aittir. Bu if statementta bu ayristirma yapilmaktadir.
    {
      LCD.setCursor(3, 0);
      LCD.print(coordinate);          //alinan coordinate degeri 1. satir 3. sutun konumundan itibaren yazilir
    }
    else if (coordinate > 128)        //gonderilen veriler haritalandirmaya gore [128,192] araligindadir. Gelen veriler 1000_0000 degerinden buyukse x coordinatina aittir. Bu if statementta bu ayristirma yapilmaktadir.
    {
      coordinate = coordinate - 128;  //gonderilen y ekseni verileri coordinate+128 olarak gonderilmistir. Boylece en anlamlı bit x/y biti olarak degerlendirilir ve 1xxx_xxxx seklinde bir gonderim geldiginde bu veri y eksenine aittir. Veriyi anlamlandirmak icin veriden 128 cikarilir.
      LCD.setCursor(3, 1);
      LCD.print(coordinate);          //alinan coordinate degeri 2. satir 3. sutun konumundan itibaren yazilir
    }
  }
  else                                //Bluetootha veri gelmediginde
  {
    LCD.setCursor(3, 0);              //alinan coordinate degeri 1. satir 3. sutun konumundan itibaren yazilir
    LCD.print("-   ");                //veri gelmemesi durumunda ekrana <-> yazilir
    LCD.setCursor(3, 1);              //alinan coordinate degeri 2. satir 3. sutun konumundan itibaren yazilir
    LCD.print("-   ");                //veri gelmemesi durumunda ekrana <-> yazilir
  }
    LCD.display();                    //LCD icin belirlenen kisitlar kapsaminda LCD ekran basilir
    delayMicroseconds(20);            //LCD ekranin yenilenmesi ve Bluetooth verilerinin daha saglikli alinmasi icin 20 mikrosaniye gecikme atanmistir.
}
