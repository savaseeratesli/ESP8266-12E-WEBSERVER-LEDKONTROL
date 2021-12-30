#include <Arduino.h>
#include<ESP8266WiFi.h>

const char* ssid="Eratesli";
const char* password="2663.bond"; 

int ledPin=D2;

WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin,OUTPUT);

  //Led sönük durumda başlasın
  digitalWrite(ledPin,LOW);

  Serial.println("");
  Serial.println("Wifi Bağlaniyor...");
  Serial.println(ssid);//Wifi adı

  WiFi.begin(ssid,password);
  
  //Wifi bağlantısı yoksa olasıya kadar dönsün
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(100);
    Serial.println("...");
  }

  Serial.println("");
  Serial.println("Wifi Baglandi.");

  server.begin();

  Serial.println("Server Basladi.");

  Serial.print("IP Adres= ");
  Serial.print("http://");

  //Tıklanabilir IP adresi
  Serial.print(WiFi.localIP());

  Serial.print("/");

}

void loop() 
{
  //WEBten bir işlem geldi mi? Client
  WiFiClient client=server.available();

  //WEBtwn bir şey geldi mi?
  //Sürekli dön
  if(!client)
  {
    return;
  }

  //WEBten işlem yoksa bekle 
  while(!client.available())
  {
    delay(1);
  }

  //WEBten işlem varsa
  //Local IPden sonraki değeri oku
  //Local IP yanına işlem atayacağız
  String request=client.readStringUntil('\r');

  //İşlem
  Serial.println(request);

  //client tamamlandı
  client.flush();

  //Led kontrol
  int value=LOW;

  //WEBten ON gelirse
  if(request.indexOf("/LED=ON")!=-1)
  {
    digitalWrite(ledPin,HIGH);
    value=HIGH;
  }

  if(request.indexOf("/LED=OFF")!=-1)
  {
    digitalWrite(ledPin,LOW);
    value=LOW;
  }


//HTML KODLARI//
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta charset=\"UTF-8\"/>");
  client.print("<span style='font-size:50px;font-weight:bold;'>Led: </span>");
 
  if(value == HIGH) 
  {
    client.print("<span style='font-size:70px;font-weight:bold;color:green;'>ACIK</span>");
  } 
  else 
  {
    client.print("<span style='font-size:70px;font-weight:bold;color:red;'>KAPALI</span>");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button style='font-size:350%;color:white;font-weight:bold;width:45%;height:200px;border-radius: 40px;background-color:green;box-shadow: 3px 3px 3px #000;'>On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button style='font-size:350%;color:white;width:45%;height:200px;border-radius: 25px;margin-left:50px;background-color:red;box-shadow: 3px 3px 3px #000;'>Off </button></a><br />");  
  client.println("</html>");
  delay(1);
  Serial.println("Client Tamamlandi.");
  Serial.println("");

}

