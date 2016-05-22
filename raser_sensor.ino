#include <Wire.h>

// I2Cで読み取った電圧から距離を算出する．
// 読み取る電圧の単位はmV
// 算出する距離の単位はμm

// 定数
// V = 0のときの距離
const float BASE_DIST = 30000;
// 距離[μm] / 電圧[mV] の値を計算
const float DPV_def = 0.5;
const float DPV_mag = 1.2;
const float DPV = DPV_def / DPV_mag;

// is_posのピン
//const int pin_is_pos = 9;
//uint8_t bit_is_pos, port_is_pos;

float dist;



// ----- I2Cの各種設定 -----
const byte INA226_ADDR = B1000000;
const byte INA226_CONFIG = 0x00;
const byte INA226_BUSV   = 0x02;
const byte INA226_POWER  = 0x03;
const byte INA226_CALIB  = 0x05;
const byte INA226_MASK   = 0x06;
const byte INA226_ALERTL = 0x07;
const byte INA226_DIE_ID = 0xff;
void INA226_write(byte reg, unsigned short val)
{
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(reg);
  Wire.write(val >> 8);
  Wire.write(val & 0xff);
  Wire.endTransmission();  
}

short INA226_read()
{
  short ret = 0;
  // request the registor
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(INA226_BUSV);
  Wire.endTransmission();  
  // read
  Wire.requestFrom((int)INA226_ADDR, 2);
  while(Wire.available()) {
    ret = (ret << 8) | Wire.read();
  }
  return ret;
}

float read_distance()
{
//  int is_pos = (*portInputRegister(port_is_pos) & bit_is_pos) ? 1 : -1;
//  float in_v = INA226_read() * is_pos * 1.25;
  float in_v = INA226_read() * 1.25;

  return (float)(in_v * DPV) + BASE_DIST;
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // I2Cの初期設定
  Wire.begin();
  // average: 16 times, conversion time: 8.244ms/8.244ms 
  INA226_write(INA226_CONFIG, 0x45ff);
  // current conversion
  INA226_write(INA226_CALIB,  2560);

  // DigitalReadの初期設定
//  bit_is_pos = digitalPinToBitMask(pin_is_pos);
//  port_is_pos = digitalPinToPort(pin_is_pos);
  

  // 最初の読み取り
  dist = read_distance();
  
}

void loop() {
  
  dist = read_distance();
  Serial.println(dist);
  
  delay(100);
}
