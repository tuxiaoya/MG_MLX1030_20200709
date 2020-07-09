#include <Arduino.h>
#include "Adafruit_MLX90614.h"
#include "WiFi.h"
#include "WiFiClient.h"
#include <BlynkSimpleEsp32.h>

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED_BUILTIN 2 // 蓝色LED

// 武玉柱老师 60.213.28.10  QQ登录 密码hongminote2
char auth[] = "_XhOI4mpO_sA6kUAZMKZwAAiO-fIq5BK";

char ssid[] = "敏光科技-3";  //
char pass[] = "mgkj8190688"; //3楼wifi
IPAddress WYZ_IP(60, 213, 28, 10); //  武玉柱老师 60.213.28.10 QQ登录 密码hongminote2
uint16_t Blynk_PORT = 8080;

// char *dtostrf(double _val, signed char _width, unsigned char prec, char *_s)
//     参数说明：
//     _val : 要转换的float或者double值。

//            _width : 转换后整数部分长度。

//                     _prec：转换后小数部分长度。

//                     _s : 保存到该char数组中

char msg[50];

WidgetLCD lcd(V20);

Adafruit_MLX90614 MLX0 = Adafruit_MLX90614();      // 硅片
Adafruit_MLX90614 MLX1 = Adafruit_MLX90614(0x5B);   // 裸窗
double MLX_emissivity0 = 0.9;     //  0.1-1
double MLX_emissivity1 = 0.9;
double MLX0_OBJ = 0;
double MLX0_AMB = 0;
double MLX1_OBJ = 0;
double MLX1_AMB = 0;

BlynkTimer timer;

void myTimerEvent()
{  
  MLX0_OBJ = MLX0.readObjectTempC();
  MLX0_AMB = MLX0.readAmbientTempC();
  MLX1_OBJ = MLX1.readObjectTempC();
  MLX1_AMB = MLX1.readAmbientTempC();

  Serial.print("OBJ0 is: ");  Serial.println(MLX0_OBJ);
  Serial.print("AMB0 is: ");  Serial.println(MLX0_AMB);
  Serial.print("OBJ1 is: ");  Serial.println(MLX1_OBJ);
  Serial.print("AMB1 is: ");  Serial.println(MLX1_AMB);

  Blynk.virtualWrite(V5, MLX0_OBJ);
  Blynk.virtualWrite(V6, MLX0_AMB);
  Blynk.virtualWrite(V7, MLX1_OBJ);
  Blynk.virtualWrite(V8, MLX1_AMB);
}
 

BLYNK_WRITE(V1)
{
  double pinValue = param.asDouble();
  MLX_emissivity0 = pinValue;
  MLX0.writeEmissivity(MLX_emissivity0);
  Serial.print("MLX_emissivity0  is: ");
  Serial.println(MLX_emissivity0);
  Blynk.virtualWrite(V3, MLX_emissivity0);
  
} 
BLYNK_WRITE(V2)
{
  double pinValue = param.asDouble();
  MLX_emissivity1 = pinValue;
  MLX1.writeEmissivity(MLX_emissivity1);
  Serial.print("MLX_emissivity1  is: ");
  Serial.println(MLX_emissivity1);
  Blynk.virtualWrite(V4, MLX_emissivity1);
}

BLYNK_WRITE(V10)
{
  int16_t pinValue = param.asInt();
  lcd.clear();              //Use it to clear the LCD Widget
  lcd.print(0, 0, "MLX_E0:");
  dtostrf(MLX_emissivity0, 1, 2, msg);
  lcd.print(10, 0, msg);                                          //
  lcd.print(0, 1, "MLX_E1:");
  dtostrf(MLX_emissivity1, 1, 2, msg);
  lcd.print(10, 1, msg); //
}

void set_led(byte status)
{
  digitalWrite(LED_BUILTIN, status);
}

boolean SETUP_OK = true; // 允许在线读取温度

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);  
  MLX0.begin();
  MLX1.begin();
  // MLX0.writeEmissivity(MLX_emissivity0);
  // MLX1.writeEmissivity(MLX_emissivity1);
  MLX_emissivity0 = MLX0.readEmissivity();
  Serial.print("MLX_emissivity0  is: ");
  Serial.println(MLX_emissivity0);
  MLX_emissivity1 = MLX1.readEmissivity();
  Serial.print("MLX_emissivity1  is: ");
  Serial.println(MLX_emissivity1);
 
  Serial.println("NLX90614 Initial OK!");

  Blynk.begin(auth, ssid, pass, WYZ_IP, Blynk_PORT);  
  timer.setInterval(1000L, myTimerEvent);

  Blynk.virtualWrite(V3, MLX_emissivity0); 
  Blynk.virtualWrite(V4, MLX_emissivity1);
  set_led(SETUP_OK);

}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
 