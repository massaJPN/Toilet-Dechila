// Arduino 1.8.15
// Board M5STACK 1.0.9
// Library M5ATOM 0.0.5
#include <M5Atom.h>
#include <esp_now.h>
#include <WiFi.h>

const int CDS_PIN = 33; // CdSセルの接続端子番号です。
const int CDS_VAL = 0; // 明暗の閾値を設定します。実使用環境でのCdSセルの出力値より判断して設定します。
bool CDS_FLAG = false;

uint8_t slaveAddress[] = { 0x**, 0x**, 0x**, 0x**, 0x**, 0x** }; // 受信機のMACアドレスに書き換えます。

void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void setup() {
    M5.begin();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESPNow Init Success");
    }

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
}

void loop() {
    M5.update();

    int CDS_VAL = analogRead(CDS_PIN);
    Serial.println(CDS_VAL);
    delay(50);

    if (CDS_VAL>100){
        CDS_FLAG == true;
        }
    else{
        CDS_FLAG == false;    
        }   
    if (CDS_VAL>100 && CDS_FLAG == false) {
        uint8_t data[1] = {222}; // 送信データ
        esp_err_t result = esp_now_send(slaveAddress, data, sizeof(data));
        
        Serial.print("Send Status: 222");
        CDS_FLAG = true;
    }
    else if(CDS_VAL<100 && CDS_FLAG == true){
        uint8_t data[1] = {333}; // 送信データ
        esp_err_t result = esp_now_send(slaveAddress, data, sizeof(data));

        Serial.print("Send Status: 333");
        CDS_FLAG = false;
    }
    delay(1);
}
