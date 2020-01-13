/*
  Copyright 2017 Sony Semiconductor Solutions Corporation

  $Rev: 1059 $
  $Date:: 2017-08-29 15:39:13 +0900#$

  Sony's LPWA トラッカープラスライブラリの実装サンプル。

  (ノンブロッキングモード)

  Sony's LPWA トラッカーライブラリにて、2、3番ピンを使用しています。本
  サンプルプログラムでは、ペイロードを8ビット単位で分割し、毎回、インク
  リメントして送信しています。

  極力、Arduino style guide

  https://www.arduino.cc/en/Reference/StyleGuide

  に準拠していますが、一部、ポインタを使用して実装している部分がありま
  す。

 */
#include <SoftwareSerial.h>
#include "SonysLPWA.h"

const int LOG_LINE_SIZE = 512;

SonysLPWA lpwa = SonysLPWA();
PAYLOAD_BUF payload;

SoftwareSerial mySerial(6, 7);

/*
 * ログ出力用関数
 */
size_t logputs(const char szMsg[]) {
  int ret = Serial.print(szMsg);
  Serial.flush();
  return ret;
}

/*
 * ログ出力用関数(書式付き)
 */
void printLog(const char szFmt[], ...) {
  va_list arg;
  char buf[LOG_LINE_SIZE];

  va_start(arg, szFmt);
  vsnprintf(buf, LOG_LINE_SIZE - 1, szFmt, arg);
  va_end(arg);

  logputs("[Sample App] ");
  logputs(buf);
}

void setup() {

  // ハードシリアルをデバッグログ用として使用
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);

  //  printLog("%s\r\n", SonysLPWA::getRevision());

  lpwa.begin();
  lpwa.setDebugLogFunction(logputs);

  payload.bit_info.b40 = 0;   /* 実際はLSB 1bitのみ有効 */
  payload.bit_info.b39b32 = 0;
  payload.bit_info.b31b24 = 0;
  payload.bit_info.b23b16 = 0;
  payload.bit_info.b15b8 = 0;
  payload.bit_info.b7b0 = 0;
}

void loop() {
  PAYLOAD_RESULT payloadResult = STILL_PL;
  TX_RESULT result;

  printLog("SET %02X%02X%02X%02X%02X%02X\r\n",
           payload.byte_info[0], payload.byte_info[1],
           payload.byte_info[2], payload.byte_info[3],
           payload.byte_info[4], payload.byte_info[5]);

  mySerial.flush();
  /* unblock the master so that data flow starts */
  /* write synch byte on the serial for the master to start transmission */
  mySerial.write(0xde);
  mySerial.write(0xad);
  mySerial.write(0xbe);
  mySerial.write(0xef);

  // put your main code here, to run repeatedly:
  Serial.println("Waiting for payload bytes\n");
  while(!mySerial.available())
          ;
  /* just wait */
  Serial.println("Reading Payload bytes\n");
  while(mySerial.available())
  {
    int val = mySerial.read();
    Serial.println(val);
  }
  Serial.println("Flushing SW Serail\n");
  mySerial.flush();
  Serial.println("Writing the stop byte\n");
  mySerial.write(0x30);

//  if(Serial.available() > 0)
//  {
//    int incomingByte = Serial.read();
//    byte b = lowByte(incomingByte); 
//    Serial.println( b, HEX );  
//    payload.bit_info.b40 = b;
//    payload.bit_info.b39b32 = b;
//    payload.bit_info.b31b24 = b;
//    payload.bit_info.b23b16 = b;
//    payload.bit_info.b15b8 = b;
//    payload.bit_info.b7b0 = b;
//  } else {
//    printLog("Nothing on the Rx line yet\n");
//  }
//
//  lpwa.setPayload(payload);
//
//  while (STILL_PL == payloadResult) {
//    payloadResult = lpwa.readPayloadResult();
//  }
//
//  printLog("PlayLoad Result = %02X\r\n", payloadResult);
//  if (OK != payloadResult) {
//    return;
//  }
//
//  for (int i = 0; i < 4; i++) {
//    result = STILL_TX;
//    while (STILL_TX == result) {
//      result = lpwa.readTxResult();
//    }
//    printLog("Tx Result = %02X\r\n", result);
//  }

  //payload.bit_info.b40++;
  //payload.bit_info.b39b32++;
  //payload.bit_info.b31b24++;
  //payload.bit_info.b23b16++;
  //payload.bit_info.b15b8++;
  //payload.bit_info.b7b0++;
}

/*
 * Local Variables:
 * mode: Arduino
 * coding: utf-8
 * End:
 */
