/*
 * Copyright 2017 Sony Semiconductor Solutions Corporation
 *
 * $Rev: 1055 $
 * $Date:: 2017-08-29 13:44:38 +0900#$
 */
#include "SonysLPWA.h"
#include "StrUtil.h"

#define PIN_UART_RX 2
#define PIN_UART_TX 3

#define NUM_OF_TX 4

#define SPEED 9600L

#define SND_BUF_SIZE 64 // "Set Payload " + payload buffer + CRLF + buffer
#define RCV_BUF_SUZE SND_BUF_SIZE

static const long DEFAULT_TX_TIMEOUT = 180;
static const long DEFAULT_PAYLOAD_TIMEOUT = 20;

// PAYLOADのマスク値
static const unsigned char PAYLOAD_MASK[] = {
    0x01, 0xff, 0xff, 0xff, 0xff, 0xff
};

/*
 * コンストラクタ
 */
SonysLPWA::SonysLPWA() {
  this->m_pfPrint = NULL;

  strcpy(this->m_szSendBuf, "Set payload ");
  this->m_pszArg1 = this->m_szSendBuf + strlen(this->m_szSendBuf);

  this->m_pSwSerial = new SoftwareSerial(PIN_UART_RX, PIN_UART_TX);

  // set up a new serial port
  pinMode(PIN_UART_RX, INPUT);
  pinMode(PIN_UART_TX, OUTPUT);

  // set the data rate for the softwareSerial port
  this->m_pSwSerial->begin(SPEED);
}

/*
 *
 */
void SonysLPWA::begin(int payloadTimeout, int txTimeout)
{
  this->m_payloadTimeout = payloadTimeout * 100;
  this->m_txTimeout = txTimeout * 100;
}

const char *
SonysLPWA::getRevision(void)
{

  static char rev[] = REVISION;
  return rev;
}

void
SonysLPWA::begin()
{
  this->begin(DEFAULT_PAYLOAD_TIMEOUT, DEFAULT_TX_TIMEOUT);
}

void SonysLPWA::end()
{
}

void
SonysLPWA::printLog(const char *pszFmt, ...) {
    va_list arg;
    //char buf[1024];
    char buf[256];

    if (!this->m_pfPrint) {
        return;
    }

    va_start(arg, pszFmt);
    vsnprintf(buf, 256 - 1, pszFmt, arg);
    va_end(arg);

    this->m_pfPrint("[Sony's LPWA] ");
    this->m_pfPrint(buf);
}

void
SonysLPWA::setDebugLogFunction(size_t (*pf)(const char *)) {
    this->m_pfPrint = pf;
    if (this->m_pfPrint) {
      printLog("enabled print function. (%s)\r\n",
               SonysLPWA::getRevision());
    }
}

char *
SonysLPWA::readLine(char *lpszBuf, // 受信バッファ
                    size_t cbMax,  // 最大受信サイズ
                    long timeout   // 最大待ち時間(10ms)
                    )
{
  int i = 0;
  int w = 0;

  printLog("  Begin readLine()\r\n");
  *(lpszBuf + 0) = '\0';
  for (w = 0; w < timeout; w++) {
    if (this->m_pSwSerial->available()) {
      *(lpszBuf + i) = this->m_pSwSerial->read();
      *(lpszBuf + i + 1) = '\0';
      if ('\n' == *(lpszBuf + i) || i >= ((int) cbMax - 2)) {
        break;
      }
      i++;
    } else {
      delay(10);
    }
  }
  printLog("  Received: time=%d(x10ms), data=[%s]\r\n",
           w, StrUtil::trim(lpszBuf));
  return lpszBuf;
}

// ペイロードをセットする。
int
SonysLPWA::setPayload(const PAYLOAD_BUF &payload) {
  int i;
  int cb;

  for (i = 0; i < (int) sizeof(PAYLOAD_MASK); i++) {
    sprintf(this->m_pszArg1 + (i * 2),
            "%02X", payload.byte_info[i] & PAYLOAD_MASK[i]);
  }
  strcat(this->m_pszArg1, "\r\n");
  cb = m_pSwSerial->print(this->m_szSendBuf);
  printLog("Sended: %dbytes data=%s", cb, this->m_szSendBuf);

  // 基本的に正常終了のみ
  return 0;
}

PAYLOAD_RESULT
SonysLPWA::waitForPayloadResult(void)
{
  char *lpszTbl[5];
  int cnt;

  printLog("Begin waitForPayloadResult()\r\n");

  while (true) {
    readLine(this->m_szReceiveBuf, 24, this->m_payloadTimeout);

    if (!this->m_szReceiveBuf[0]) {
      return PL_TIMEOUT;
    }
    cnt = StrUtil::split(this->m_szReceiveBuf, lpszTbl, 5);
    if (3 <= cnt
        && 0 == strcmp("Set", lpszTbl[0])
        && 0 == strcmp("payload", lpszTbl[1])) {
      if (0 == strcmp("OK", lpszTbl[2])) {
        printLog("payLoad set OK\n");
        return OK;
      } else if (4 == cnt && 0 == strcmp("ERR", lpszTbl[2])) {
        printLog("payLoad set ERR\n");
        return (PAYLOAD_RESULT) strtol(lpszTbl[3], NULL, 16);
      } else {
        printLog("payLoad set result no payload\n");
        return NOT_PAYLOAD;
      }
    }
    if (2 == cnt && 0 == strcmp("Tx", lpszTbl[0])) {
      continue;
    }
    return NOT_PAYLOAD;
  }
}

TX_RESULT
SonysLPWA::waitForTxResult(void)
{
  char *lpszTbl[3];
  int cnt;

  printLog("Begin waitForTxResult()\r\n");

  readLine(this->m_szReceiveBuf, 24, this->m_txTimeout);

  if (!this->m_szReceiveBuf[0]) {
    return TX_TIMEOUT;
  }

  cnt = StrUtil::split(this->m_szReceiveBuf, lpszTbl, 3);
  if (2 == cnt && 0 == strcmp("Tx", lpszTbl[0])) {
    return (TX_RESULT) strtol(lpszTbl[1], NULL, 16);
  } else {
    return NOT_TX;
  }
}

TX_RESULT
SonysLPWA::waitForFinalTxResult(void)
{
  TX_RESULT result = STILL_TX;

  printLog("Begin waitForFinalTxResult()\r\n");

  for (int i = 0; i < NUM_OF_TX; i++) {
    TX_RESULT r = this->waitForTxResult();
    if (r < result) {
      result = r;
    }
  }
  return result;
}

PAYLOAD_RESULT
SonysLPWA::readPayloadResult(void)
{
  if (!this->m_pSwSerial->available()) {
    return STILL_PL;
  }
  printLog("Begin readPayloadResult()\r\n");
  return waitForPayloadResult();
}

TX_RESULT
SonysLPWA::readTxResult(void)
{
  if (!this->m_pSwSerial->available()) {
    return STILL_TX;
  }
  printLog("Begin readTxResult()\r\n");
  return waitForTxResult();
}


/*
 * Local Variables:
 * mode: arduino
 * coding: utf-8
 * End:
 */
