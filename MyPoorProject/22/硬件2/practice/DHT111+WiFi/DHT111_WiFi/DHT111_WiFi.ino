//软串口
#include <SoftwareSerial.h>
// RX, TX 配置2、3为软串口
SoftwareSerial mySerial(2, 3);
#include <dht11.h>                                               //引用dht11库文件，使得下面可以调用相关参数
#define DHT11PIN 4                                             //定义温湿度针脚号为4号引脚
dht11 DHT11;                                                       //实例化一个对象
//定时器函数
void onTimer();
//WIFI上传函数，s为上传到服务器的值
void sendmessage(String s);
void setup()
{
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
  pinMode(DHT11PIN,OUTPUT);                       //定义输出口
 // DHT11.begin();
}
void loop() {
   //与下一句清空串口缓存
    if((mySerial.available() > 0)){
    mySerial.println(Serial.read());
  }
  int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk

int tem=(float)DHT11.temperature;               //将温度值赋值给tem
int hum=(float)DHT11.humidity; //将湿度值赋给hum

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

delay(6000);                                          //延时一段时间

}
