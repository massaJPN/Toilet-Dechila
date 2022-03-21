// Arduino 1.8.15
// Board M5STACK 1.0.9
// Library M5ATOM 0.0.5
#include <M5Atom.h>
#include <esp_now.h>
#include <WiFi.h>

bool ledState = false;

void led_clear(){
  for(int i=0; i<25; i++){
    M5.dis.drawpix(i, 0x000000);

  }
}

void led_center(){
  M5.dis.drawpix(1, 0xffffff); // ここで表示する文字を作成してます。
  M5.dis.drawpix(2, 0xffffff);　//LED Matrixの位置番
  M5.dis.drawpix(7, 0xffffff); //  　1  2  3  4  5
  M5.dis.drawpix(12, 0xffffff); //  6  7  8  9 10
  M5.dis.drawpix(16, 0xffffff); // 11 12 13 14 15
  M5.dis.drawpix(18, 0xffffff); // 16 17 18 19 20
  M5.dis.drawpix(20, 0xffffff); // 21 22 23 24 25 
  M5.dis.drawpix(24, 0xffffff); //USB Conector side
}

void OnDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println();
    Serial.printf("Last Packet Recv from: %s\n", macStr);
    Serial.printf("Last Packet Recv Data(%d): ", data_len);
    Serial.print(data[0]);
    Serial.print(" ");
        if ((data[0] == 222) && (ledState == false)) {
        Serial.printf("OK");
        led_center();
        delay(1000);
        ledState = true;
        }
        else if((data[0] != 222) && (ledState == true)) {
        Serial.printf("OFF");
        led_clear(); 
        delay(1000);
        ledState = false;
        }

}

void setup() {
    M5.begin(true, false, true);
    delay(50);
    Serial.begin(115200);
    Serial.print("STA MAC: ");Serial.println(WiFi.macAddress()); // このアドレスを送信側へ登録します
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESP-Now Init Success");
    }
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
