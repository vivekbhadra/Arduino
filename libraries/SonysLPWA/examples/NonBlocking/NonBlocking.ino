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
#include "SonysLPWA.h"

const int LOG_LINE_SIZE = 512;

SonysLPWA lpwa = SonysLPWA();
PAYLOAD_BUF payload;

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
  Serial.begin(9600);

  //  printLog("%s\r\n", SonysLPWA::getRevision());

  lpwa.begin();
  lpwa.setDebugLogFunction(logputs);

  payload.bit_info.b40 = 255;   /* 実際はLSB 1bitのみ有効 */
  payload.bit_info.b39b32 = 0x23;
  payload.bit_info.b31b24 = 0x45;
  payload.bit_info.b23b16 = 0x67;
  payload.bit_info.b15b8 = 0x89;
  payload.bit_info.b7b0 = 0xab;
}

void loop() {
  PAYLOAD_RESULT payloadResult = STILL_PL;
  TX_RESULT result;

  printLog("SET %02X%02X%02X%02X%02X%02X\r\n",
           payload.byte_info[0], payload.byte_info[1],
           payload.byte_info[2], payload.byte_info[3],
           payload.byte_info[4], payload.byte_info[5]);

  lpwa.setPayload(payload);

  while (STILL_PL == payloadResult) {
    printLog("Trying to read serial line\r\n");
    payloadResult = lpwa.readPayloadResult();
  }

  printLog("PlayLoad Result = %02X\r\n", payloadResult);
  if (OK != payloadResult) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    result = STILL_TX;
    while (STILL_TX == result) {
      result = lpwa.readTxResult();
    }
    printLog("Tx Result = %02X\r\n", result);
  }

  payload.bit_info.b40++;
  payload.bit_info.b39b32++;
  payload.bit_info.b31b24++;
  payload.bit_info.b23b16++;
  payload.bit_info.b15b8++;
  payload.bit_info.b7b0++;
}

/*
 * Local Variables:
 * mode: Arduino
 * coding: utf-8
 * End:
 */
