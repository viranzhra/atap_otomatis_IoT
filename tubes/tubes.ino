#include <Servo.h>

Servo myServo;  // Objek untuk mengendalikan servo motor
int servoPin = 4;  // Pin digital untuk servo motor

int ldrPin = 5;  // Pin digital untuk sensor LDR
int ldrValue = 0;  // Variabel untuk menyimpan nilai sensor LDR

int rainPin = 2;  // Pin digital untuk sensor hujan
int rainValue = 0;  // Variabel untuk menyimpan nilai sensor hujan

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);  // Menyambungkan servo motor ke pin digital
  pinMode(ldrPin, INPUT);     // Menyambungkan sensor LDR ke pin digital
  pinMode(rainPin, INPUT);    // Menyambungkan sensor hujan ke pin digital
}

void loop() {
  // Membaca nilai sensor LDR
  ldrValue = analogRead(ldrPin);
  
  // Membaca nilai sensor hujan
  rainValue = digitalRead(rainPin);

  // Menampilkan nilai sensor cahaya dan hujan pada monitor Serial
  Serial.print("Nilai LDR: ");
  Serial.print(ldrValue);
  Serial.print(", Nilai Hujan: ");
  Serial.println(rainValue);

  // Menggerakkan servo motor jika nilai LDR lebih rendah dari ambang tertentu dan tidak hujan
  if (ldrValue < 500 && rainValue == LOW) {
    myServo.write(90);  // Menggerakkan servo motor ke posisi 90 derajat
    delay(1000);  // Tunggu 1 detik
  } else {
    myServo.write(0);  // Menggerakkan servo motor ke posisi 0 derajat
  }

  // Memeriksa kondisi hujan
  if (rainValue == HIGH) {
    Serial.println("Hujan!");
    
    // Tambahan: Menutup atap jika cahaya cerah dan hujan
    if (ldrValue > 800) {
      Serial.println("Cahaya cerah, menutup atap.");
      // Tambahkan kode untuk menutup atap di sini
    }
  } else {
    Serial.println("Tidak hujan.");

    // Tambahan: Menutup atap jika gelap dan tidak hujan
    if (ldrValue < 200) {
      Serial.println("Gelap, menutup atap.");
      // Tambahkan kode untuk menutup atap di sini
    }
  }

  delay(1000);  // Tunggu 1 detik sebelum membaca sensor lagi
}
