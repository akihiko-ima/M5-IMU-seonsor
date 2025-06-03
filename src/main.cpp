#include <M5Stack.h>
#include <Arduino.h>
#include <Wire.h>

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
bool isRecording = false;

void initMPU6050()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void showWelcomeMessage()
{
  M5.Lcd.setTextSize(3);
  M5.Lcd.println("Hello,");
  M5.Lcd.println("Work Monitoring System!");
}

void showStatusMessage(const char *msg)
{
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println(msg);
}

void readMPU6050()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

void printSensorData()
{
  Serial.print("AcX = ");
  Serial.print(AcX);
  Serial.print(" | AcY = ");
  Serial.print(AcY);
  Serial.print(" | AcZ = ");
  Serial.print(AcZ);
  Serial.print(" | Tmp = ");
  Serial.print(Tmp / 340.00 + 36.53); // equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = ");
  Serial.print(GyX);
  Serial.print(" | GyY = ");
  Serial.print(GyY);
  Serial.print(" | GyZ = ");
  Serial.println(GyZ);
}

void handleButtons()
{
  if (M5.BtnA.wasPressed())
  {
    isRecording = true; // データ取得開始
    showStatusMessage("Start Recording");
  }
  else if (M5.BtnB.wasPressed())
  {
    isRecording = false; // データ取得停止
    showStatusMessage("Stop Recording");
  }
  else if (M5.BtnC.wasPressed())
  {
    showStatusMessage("C pressed");
  }
}

void setup()
{
  M5.begin();
  initMPU6050();
  showWelcomeMessage();

  pinMode(21, INPUT_PULLUP); // SDAピン21
  pinMode(22, INPUT_PULLUP); // SCLピン22

  Serial.begin(115200); // シリアル通信の初期化
}

void loop()
{
  M5.update();
  handleButtons();

  if (isRecording)
  {
    readMPU6050();
    printSensorData();
  }

  delay(333); // データ取得間隔
}
