
#include <dht11.h>                                               //引用dht11库文件，使得下面可以调用相关参数
#define DHT11PIN 2                                             //定义温湿度针脚号为2号引脚
dht11 DHT11;                                                       //实例化一个对象

void setup() {                                                       //设置

Serial.begin(115200);                                            //设置波特率参数
pinMode(DHT11PIN,OUTPUT);                       //定义输出口
}

void loop() {                                                    //循环
int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk

int tem=(float)DHT11.temperature;               //将温度值赋值给tem
int hum=(float)DHT11.humidity;                   //将湿度值赋给hum
//Serial.print("Tempeature: Humidity:",tem hum);  //打印出Tempeature:
Serial.print("{\"Tempeature\":");
Serial.print("\"");
Serial.print(tem);  
Serial.print("\"");
//Serial.print("\"}");                                   //打印温度结果

Serial.print(",");

Serial.print("\"Humidity");     //打印出Humidity:
Serial.print("\":");
Serial.print("\"");
Serial.print(hum);                     //打印出湿度结果
Serial.print("\"");
Serial.println("}");                //打印出%

delay(1000);                                          //延时一段时间

}
