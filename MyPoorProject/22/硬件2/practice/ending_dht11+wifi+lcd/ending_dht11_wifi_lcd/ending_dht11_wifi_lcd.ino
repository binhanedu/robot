//软串口
#include <SoftwareSerial.h>
// RX, TX 配置9、10为软串口
SoftwareSerial mySerial(9, 10);
#include <dht11.h>    //引用dht11库文件，使得下面可以调用相关参数
#include <LiquidCrystal.h> //加载lcd库
#define DHT11PIN 8 //定义温湿度针脚号为8号引脚
dht11 DHT11;
//定时器函数
void onTimer();
//WIFI上传函数，s为上传到服务器的值
void sendmessage(String s);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(115200);    //波特率设置为115200因为要跟ESP8266的串口波特率保持一致
  mySerial.begin(115200);    //ESP8266的串口波特率为115200
  mySerial.println("AT+CIPMODE=1");//设置信息发送的模式为透传模式
  delay(500);
  mySerial.println("AT+CIPSTART=\"TCP\",\"192.168.43.8\",8883");//连接服务器，
  delay(5000);
  /***********发送数据*************/
  mySerial.println("AT+CIPSTATUS");
  delay(500);
  mySerial.println("AT+CIPSEND");
  delay(500);
  pinMode(DHT11PIN,OUTPUT);//定义输出口
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  lcd.print("Wifi connects...");//开始连接WiFi
  delay(5000);//延时
  lcd.clear();//清屏
  delay(1000);//延时
  lcd.print("successful");//连接成功
  delay(5000);//延时
  lcd.clear();//清屏
  delay(3000);
  lcd.print("display");//开始显示
  delay(3000);
  lcd.clear();//清屏
  delay(3000);
  

}

void loop() {
  //与下一句清空串口缓存
  if((mySerial.available() > 0)){
  mySerial.println(Serial.read());
  }
  float chk = DHT11.read(DHT11PIN);
  float tem=DHT11.temperature;               //将温度值赋值给tem
  float hum=DHT11.humidity; //将湿度值赋给hum
  Serial.println("数据发送成功！");//数据查看
  if(tem!=0){
  mySerial.print("{\"temperature\":");                 //打印出tempeature:
  mySerial.print("\"");
  mySerial.print(tem);  
  mySerial.print("\"");
  //Serial.print("\"}");                                   //打印温度结果

  mySerial.print(",");

  mySerial.print("\"humidity");     //打印出humidity:
  mySerial.print("\":");
  mySerial.print("\"");
  mySerial.print(hum);                     //打印出湿度结果
  mySerial.print("\"");
  mySerial.println("}");                //打印出%
  lcd.setCursor(0, 0);
  lcd.print("temp: ");
  lcd.print((float)DHT11.temperature, 2);//lcd第一行显示湿度
  lcd.print("C");
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("humidity: ");
  lcd.print((float)DHT11.humidity, 2);//lcd第二行显示湿度
  lcd.print("%");
  delay(6000);//延时一段时间
}

}
