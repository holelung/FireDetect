#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <pitches.h>

int lm35_pin = A10; // 온도

int light_pin = A13;

int infrared_pin = 49; // 적외선 인체 감지

int r_pin=24; // RGB-LED
int g_pin=26;
int b_pin=28;

int flame=39;

LiquidCrystal_I2C lcd(0x27,16,2); // LCD

#define PIEZO_PIN 6 // 부저

#define B1 1000
#define B2 750
#define B3 500
#define B4 250

int melody[] =
{
NOTE_G4, NOTE_G4
};

int bits[] =
{ 
B1, B3, B3, B3, B3, B1,
B1, B3, B3, B3, B3, B1,
B2, B4, B3, B3, B3, B3, B1,
B1, B3, B3, B3, B3, B1, 
};

void sensor1_init(void)
{
  lcd.init(); // LCD
  lcd.backlight();

  pinMode(infrared_pin, INPUT); // 적외선 인체 감지

  pinMode(r_pin, OUTPUT); // RGB-LED
  pinMode(g_pin, OUTPUT);
  pinMode(b_pin, OUTPUT);
}

void serial_init()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

//#define _TEST_

class MJC_MQTT{
  private:
    typedef void (*callback)(char * topic,int topic_len, char * payload, int payload_len);
    callback mycb;
    HardwareSerial * ESP01;
    int MQTT_LENGTH(char * input){
      int mycount = 0;
    
      while(true){
        if(input[mycount] == '\0'){
          break;
        }
        mycount++;
      }
      
      return mycount;
    }
    void MQTT_MSG_PROGRESS(const char * data){
      int total_length = data[9];
      int topic_length = data[11];
      int payload_length = (total_length -2) - topic_length;
      
      char topic[topic_length+1];
      char payload[payload_length+1];
      for(int i = 0;i<topic_length;i++){
        topic[i] = data[12+i];
      }
      topic[topic_length] = '\0';
      for(int i = 0;i<payload_length;i++){
        payload[i] = data[12+topic_length+i];
      }
      payload[payload_length] = '\0';

      #ifdef _TEST_
      Serial.print("TOPIC=");
      Serial.print(topic);
      Serial.print(", PAYLOAD=");
      Serial.println(payload);
      #endif
      if(mycb != NULL){
        mycb(topic,topic_length,payload,payload_length);
      }
    }
  public:
    bool is_MQTT_CONNECTED = false;
    MJC_MQTT(HardwareSerial * input){
      ESP01 = input;
      ESP01->begin(115200); //아두이노-ESP01
      ESP01->setTimeout(100);

      while (!ESP01) {
        ; // wait for serial port to connect.
      }
    }
    void set_callback(callback yourcb){
      mycb = yourcb;
    }
    bool CONNECT_WIFI(char * id, char * pw){
      //인터넷공유기와 연결
      ESP01->write("AT+CWJAP=\"");
      ESP01->write(id);
      ESP01->write("\",\"");
      ESP01->write(pw);
      ESP01->write("\"");
      ESP01->println();
      
      //WIFI CONNECTED
      //WIFI GOT IP
      //OK
      int mystep = 0;
      bool result = true;
      //아두이노 내부의 시간을 밀리초단위로 반환하는 함수고 반환타입은 unsigned long이다!
      unsigned long while_t = millis();
      while(true){
        //무한루프 진입전의 시간과 현재시간의 차이?
        if(millis() - while_t > 8000){
          result = false;
          break;
        }
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          //Serial.println(data.c_str());
          if(data.indexOf("WIFI CONNECTED") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("인터넷 공유기 접속완료!");
            #endif
            mystep++;
          }else if(data.indexOf("WIFI GOT IP") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("IP주소 획득!");
            #endif
            mystep++;
          }else if(data.indexOf("OK") != -1 && mystep == 2){
            #ifdef _TEST_
            Serial.println("WIFI접속완료!");
            #endif
            break;
          } 
        }
        //delay(50);
      }
      return result;
    }
    void MQTT_CONNECT(){
      //MQTT브로커와 TCP연결
      ESP01->println("AT+CIPSTART=\"TCP\",\"192.168.45.150\",1883");
      //CONNECT
      //OK
      int mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("CONNECT") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("CONNECT 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("OK") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("OK 응답 수신");
            #endif
            break;
          }
        }
      }
      #ifdef _TEST_
      Serial.println("MQTT브로커와 TCP연결완료!");
      #endif
      ESP01->println("AT+CIPSEND=22");
      //OK
      //>
      mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("OK") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("OK 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf(">") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("> 응답 수신");
            #endif
            break;
          }
        }
      }
      #ifdef _TEST_
      Serial.println("데이터 업로드 준비완료!");
      #endif
      byte num1 = random(0,10); 
      byte num2 = random(0,10);
      byte num3 = random(0,10);
      byte num4 = random(0,10);
      byte MQTT_CONNECT[] = {0x10,0x14,0x00,0x04,'M','Q','T','T',0x04,0x02,0x00,0x3C,0x00,0x08,'M','J','C','-','0'+num1,'0'+num2,'0'+num3,'0'+num4};
      //MQTT브로커와 커넥션(MQTT CONNECT DATA FRAME)(1)
      ESP01->write(MQTT_CONNECT,22);
      //Recv 22 bytes
      //SEND OK
      //+IPD,4
      mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("Recv") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("RECV 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("SEND") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("SEND 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("+IPD") != -1 && mystep == 2){
            #ifdef _TEST_
            Serial.println("+IPD 응답 수신");
            #endif
            break;
          }
        }
      }
      Serial.println("MQTT브로커와 연결이 완료되었습니다!");
      is_MQTT_CONNECTED = true;
    }
    
    void MQTT_PUBLISH(char * topic, char * payload){
      int topic_length = MQTT_LENGTH(topic);
      int payload_length = MQTT_LENGTH(payload);
      #ifdef _TEST_
      Serial.println("PUBLISH 시작!!");
      Serial.println(payload_length);
      #endif
      ESP01->println("AT+CIPSEND="+String(4+topic_length+payload_length));
      //OK
      //> 
      int mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("+IPD") != -1){
            //발행하는 와중에 메시지가 수신된 독특한경우
            MQTT_MSG_PROGRESS(data.c_str());
          }
          if(data.indexOf("OK") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("1.OK 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf(">") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("1.> 응답 수신");
            #endif
            break;
          }
        }
      }
      //Serial.println("TCP업로드 준비 완료!!");
      byte header1[] = {0x30,2+topic_length+payload_length,0x00,topic_length};
      //header1
      //topic
      //payload
      ESP01->write(header1,4);
      ESP01->write(topic,topic_length);
      ESP01->write(payload,payload_length);
      //Recv 27 bytes
      //SEND OK
      mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("+IPD") != -1){
            //발행하는 와중에 메시지가 수신된 독특한경우
            MQTT_MSG_PROGRESS(data.c_str());
          }
          if(data.indexOf("Recv") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("Recv 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("SEND") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("SEND OK 응답 수신");
            #endif
            break;
          } 
        }
      }
      Serial.println("MQTT PUBLISH완료!!");
    }
    
    void MQTT_SUBSCRIBE(char * topic){
      int topic_length = MQTT_LENGTH(topic);
      #ifdef _TEST_
      Serial.println("MQTT SUBSCRIBE시작!!");
      #endif
      ESP01->println("AT+CIPSEND="+String(7+topic_length));
      //OK
      //>
      int mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("OK") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("OK 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf(">") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("> 응답 수신");
            #endif
            break;
          }
        }
      }
      #ifdef _TEST_
      Serial.println("데이터 업로드 준비완료!!");
      #endif
      byte header[] = {0x82,5+topic_length,0x00,0x01,0x00,topic_length};
      ESP01->write(header,6);
      ESP01->write(topic,topic_length);
      ESP01->write(0x00);
      //Recv 16 bytes
      //SEND OK
      //+IPD,5
      mystep = 0;
      while(true){
        if(ESP01->available()){
          String data = ESP01->readStringUntil('\n');//readline 
          if(data.indexOf("Recv") != -1 && mystep == 0){
            #ifdef _TEST_
            Serial.println("Recv 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("SEND") != -1 && mystep == 1){
            #ifdef _TEST_
            Serial.println("SEND OK 응답 수신");
            #endif
            mystep++;
          }else if(data.indexOf("+IPD") != -1 && mystep == 2){
            #ifdef _TEST_
            Serial.println("+IPD 응답 수신");
            #endif
            break;
          }
        }
      }
      #ifdef _TEST_
      Serial.println("MQTT SUBSCRIBE완료!!");
      #endif
    }

    void loop(){
      //ESP01에서 수신된 데이터를 시리얼모니터로 전달~
      if(ESP01->available()){
        String rcv_data = ESP01->readStringUntil('\n');
        if(rcv_data.indexOf("CLOSED") != -1){
          //MQTT브로커와 연결이 끊어짐
          is_MQTT_CONNECTED = false;
        }else if(rcv_data.indexOf("+IPD") != -1){
          //16  9 5
          MQTT_MSG_PROGRESS(rcv_data.c_str());          
        }
      }
    }
};

void mjc_callback(char * topic,int topic_len, char * payload, int payload_len){
  Serial.print("토픽=");
  Serial.print(topic);
  Serial.print(", 페이로드=");
  Serial.println(payload);
  String mypayload = payload;
  /*
  int degree = mypayload.toInt();
  if(degree >= 0 && degree <= 180){
    Serial.print(degree);
    Serial.println("서보모터 각도 조절!");
    //myservo.write(degree);
    delay(100);
  }
  */
}

MJC_MQTT mymqtt(&Serial3);

//MQTT브로커와 연결이 끊어지면 재접속
void reconnect(){
  mymqtt.MQTT_CONNECT();
  //구독할 토픽을 지정
  mymqtt.MQTT_SUBSCRIBE("MJC/temp");
  mymqtt.MQTT_SUBSCRIBE("MJC/infrared");
  mymqtt.MQTT_SUBSCRIBE("MJC/light");
  mymqtt.MQTT_SUBSCRIBE("MJC/buzzer1");
  mymqtt.MQTT_SUBSCRIBE("MJC/fire");
}

void serialEvent3() 
{
  mymqtt.loop();
}

//////////////////////////////////////////////
#define p_topic1  "MJC/temp"
#define p_topic2  "MJC/infrared"
#define p_topic3  "MJC/light"
#define p_topic4  "MJC/buzzer1"
#define p_topic5  "MJC/fire"

void setup() {
  Serial.begin(9600); //PC-아두이노
  Serial.println("----- Start Arduino -----");
  
  while(!mymqtt.CONNECT_WIFI("SK_WiFiGIGA5CC4_2.4G","EQV39@0516")){
    Serial.print(".");
  }
  Serial.println("WIFI 접속완료!");

  mymqtt.set_callback(mjc_callback);

  sensor1_init();
  serial_init();
}

void loop() {
  if(!mymqtt.is_MQTT_CONNECTED){
    reconnect();
  }

  
  // 이부분 변경 //////////////////////////
  char tempDetect[128]={0,};
  char infraredDetect[128]={0,};
  char lightDetect[128] ={0,};
  char buzzerResult[128]={0,};
  char fireResult[128]={0,};

  int infraredVal, lightVal,  temp; // 센서별 측정값
  float lm35Val;

  String infraredSt, lm35St, buzSt, lightSt; // 센서별 상태

  String fireSt; // 화재 상태

  infraredVal = digitalRead(infrared_pin); // 적외선 값 입력
  lightVal = analogRead(light_pin);

  temp = analogRead(lm35_pin); // lm35 값 입력
  lm35Val = temp * ((5.0 * 100.0) / 1024.0);

  if (infraredVal == HIGH) { // 적외선 인체 감지
    infraredSt = "Exist";
    Serial.print("Infrared Value = ");
    Serial.println(infraredVal);
    Serial.print("Infrared Status = ");
    Serial.println(infraredSt);
  } else if (infraredVal == LOW) {
    infraredSt = "Not Exist";
    Serial.print("Infrared Value = ");
    Serial.println(infraredVal);
    Serial.print("Infrared Status = ");
    Serial.println(infraredSt);
  }

  if (lm35Val >=0 && lm35Val <30) {
    lm35St = "Normal";
    Serial.print("lm35 Value = ");
    Serial.println(lm35Val);
    Serial.print("lm35 Status = ");
    Serial.println(lm35St);
  } else if (lm35Val >=30) {
    lm35St = "Abnormal";
    Serial.print("lm35 Value = ");
    Serial.println(lm35Val);
    Serial.print("lm35 Status = ");
    Serial.println(lm35St);
  }

  if(lightVal>=0 && lightVal<=499){                           
      lightSt = "Normal";
      Serial.print("Light Value = ");
      Serial.println(lightVal);
      Serial.print("Light Status : ");
      Serial.println(lightSt);
  }
  else if(lightVal>=500 && lightVal<=900){
      lightSt = "Bright";
      Serial.print("Light Value = ");
      Serial.println(lightVal);
      Serial.print("Light Status : ");
      Serial.println(lightSt);
  }


  if(lm35St == "Abnormal"){  //화재 조건 판단 
      fireSt="Fire!!!!";
  } else if(lm35St =="Abnormal" && lightSt=="Bright"){
    fireSt = "Fire!!!!";
  } else {
    fireSt="Safe";
  }
  
  
  if (fireSt== "Fire!!!!") { // 화재가 발생했을 때
    if (infraredSt == "Exist") { // 화재가 발생했을 때 적외선 인체 감지를 한 경우
      digitalWrite(r_pin, HIGH); // (1) led를 적색을 띄우고
      
      for (int i = 0; i < (sizeof(melody)/sizeof(melody[0])); i++) { // (2) 부저를 울리고
        tone(PIEZO_PIN, melody[i], bits[i]);
        delay(bits[i]);
        noTone(3);
      }
      
      buzSt="On";
      lcd.clear(); // (3) LCD로 화재 여부 표시
      lcd.setCursor(0, 0);
      lcd.print("Warning");
      lcd.setCursor(0, 1);
      lcd.print("Exist");
    } else if (infraredSt == "Not Exist") { // 화재가 발생했을 때 적외선 인체 감지를 못 한 경우
      digitalWrite(g_pin, HIGH);
      digitalWrite(r_pin, HIGH); // (1) led를 녹색을 띄우고
      
      for (int i = 0; i < (sizeof(melody)/sizeof(melody[0])); i++) { // (2) 부저를 울리고
        tone(PIEZO_PIN, melody[i], bits[i]);
        delay(bits[i]);
        noTone(3);
      }
      
      buzSt="On";
      lcd.clear(); // (3) LCD로 화재 여부 표시
      lcd.setCursor(0, 0);
      lcd.print("Warning");
      lcd.setCursor(0, 1);
      lcd.print("Not Exist");
    }
  } else { // 화재가 발생하지 않았을 때
    noTone(PIEZO_PIN);
    digitalWrite(r_pin ,HIGH);
    digitalWrite(g_pin, HIGH);
    digitalWrite(b_pin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Safe");
    buzSt="Off";
  }

  ////////////////////////////////////////
 digitalWrite(r_pin, LOW);
 digitalWrite(g_pin, LOW);
 digitalWrite(b_pin, LOW);

infraredSt="Exist";
 infraredVal=1;
 lm35Val=20;
 lm35St="Normal";
 lightSt="Normal";
 lightVal=441;
 buzSt="Off";
 fireSt="Safe";


  char test1[20]={0,}; // 타입 변환
  lm35St.toCharArray(test1, lm35St.length()+1);

  char test2[20]={0,}; // 타입 변환
  infraredSt.toCharArray(test2, infraredSt.length()+1);

  char test3[20]={0,}; // 타입 변환
  lightSt.toCharArray(test3, lightSt.length()+1);

  char test4[20]={0,}; // 타입 변환
  buzSt.toCharArray(test4, buzSt.length()+1);

  char test5[20]={0,}; // 타입 변환
  fireSt.toCharArray(test5, fireSt.length()+1);

  

  
  sprintf(tempDetect, "{\"lm35Status\":\"%s\", \"lm35Value\":\"%d\"}", test1, (int)lm35Val);

  sprintf(infraredDetect, "{\"infraredStatus\":\"%s\", \"infraredValue\":\"%d\"}", test2, infraredVal);

  sprintf(lightDetect, "{\"lightStatus\":\"%s\", \"lightValue\":\"%d\"}", test3, lightVal);

  sprintf(buzzerResult, "{\"buzzer\":\"%s\"}", test4);

  sprintf(fireResult, "{\"fire\":\"%s\"}", test5);

  Serial.println(infraredDetect);
  Serial.println(tempDetect);
  Serial.println(lightDetect);
  Serial.println(buzzerResult);
  Serial.println(fireResult);

  Serial.println("----토픽 publish----");
  mymqtt.MQTT_PUBLISH(p_topic1, tempDetect);
  mymqtt.MQTT_PUBLISH(p_topic2, infraredDetect);
  mymqtt.MQTT_PUBLISH(p_topic3, lightDetect);
  mymqtt.MQTT_PUBLISH(p_topic4, buzzerResult);
  mymqtt.MQTT_PUBLISH(p_topic5, fireResult);



  mymqtt.loop();
  delay(1000); //1초
}