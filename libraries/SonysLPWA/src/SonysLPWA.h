/*
 * Copyright 2017 Sony Semiconductor Solutions Corporation
 *
 * $Rev: 1055 $
 * $Date:: 2017-08-29 13:44:38 +0900#$
 *
 */
#ifndef SonysLPWA_h
#define SonysLPWA_h
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "StrUtil.h"

#define REVISION "R1.0.0" "($Date:: 2017-08-29 13:44:38 +0900#$)"

// 送受信バッファのサイズ(論理的な最大サイズより余裕を持って定義してい
// ます)
#define SND_BUF_SIZE 64 // "Set Payload " + payload buffer + CRLF + buffer
#define RCV_BUF_SUZE SND_BUF_SIZE

// b40       b39b32   b31b24   b23b16   b15b8    b7b0
// +--------+--------+--------+--------+--------+--------+
// |XXXXXXX.|........|........|........|........|........|
// +--------+--------+--------+--------+--------+--------+
//        40                                     7      0
typedef union {
  struct {
    // b40は元々は、1bitで定義して先頭に7bitをパディングしていたが、先
    // 頭がLSBになったので、気持が良くないのと、処理系によって異なる可
    // 能性があるので、8bitで定義する事にした。
    unsigned int b40: 8;        // LSB 1bitのみ有効
    unsigned int b39b32: 8;
    unsigned int b31b24: 8;
    unsigned int b23b16: 8;
    unsigned int b15b8: 8;
    unsigned int b7b0: 8;
  } bit_info;
  unsigned char byte_info[6];
} PAYLOAD_BUF, *PPAYLOAD_BUF;

/* Set payload レスポンスコード */
typedef enum {
  OK = 0,                       /* 正常終了 */
  TOO_FEW_ARGUMENTS = 1,        /* 引数の数が少ない */
  TOO_MANY_ARGUMENTS,           /* 引数の数が多すぎる */
  COMMAND_ERROR,                /* コマンドがSetでない" */
  ARGUMENT1_ERROR,              /* 第一引数がpayloadでない */
  ARGUMENT2_CHARACTER_ERROR,    /* 第二引数に16進解釈できない文字 */
  ARGUMENT2_LENGTH_ERROR,       /* 第二引数の長さがbit長と一致しない */
  BUFFER_OVERFLOW,              /* 256文字以上終端コードの入力が無い */
  UNEXPECTED_ERROR = 255,       /* 想定外のエラーが発生した */
  /* 以下はライブラリ独自のコード */
  NOT_PAYLOAD = 4093,           /* PAYLOAD結果以外のパケットを受信した */
  PL_TIMEOUT = 4094,            /* タイムアウト */
  STILL_PL = 4095               /* まだ payload処理中(ノーブロッキング時) */
} PAYLOAD_RESULT;

/* 送信結果通知コード */
typedef enum {
  SUCCESS_HIGH_QUALITY = 0,   /* GNSS品質HighQuality */
  SUCCESS_LOW_QUALITY = 1,    /* GNSS品質LowQuality */
  FAILED_CARRIER_SENSE = 128, /* キャリアセンスに失敗 */
  FAILED_UNEXPECTED = 255,    /* 想定外の異常が発生 */
  /* 以下はライブラリ独自のコード */
  NOT_TX = 4093,              /* 送信結果ではないパケットを受信した */
  TX_TIMEOUT = 4094,          /* 受信タイムアウト */
  STILL_TX = 4095             /* まだ送信中(ノーブロッキング時) */
} TX_RESULT;

class SonysLPWA
{
  private:
    size_t (*m_pfPrint)(const char *);
    SoftwareSerial *m_pSwSerial;
    char m_szSendBuf[SND_BUF_SIZE];
    char m_szReceiveBuf[RCV_BUF_SUZE];
    char *m_pszArg1;
    long m_txTimeout;
    long m_payloadTimeout;

    void printLog(const char *, ...);
    char *readLine(char *lpszBuf, size_t cbMax, long timeout);
  public:
    SonysLPWA(void);
    static const char *getRevision(void);
    void begin(int payloadTimeout, int txTimeout);
    void begin(void);
    void end(void);
    void setDebugLogFunction(size_t (*pf)(const char *));
    int setPayload(const PAYLOAD_BUF &payload);

    PAYLOAD_RESULT waitForPayloadResult(void);
    TX_RESULT waitForTxResult(void);
    TX_RESULT waitForFinalTxResult(void);

    PAYLOAD_RESULT readPayloadResult(void);
    TX_RESULT readTxResult(void);
};

#endif

/*
 * Local Variables:
 * mode: arduino
 * coding: utf-8
 * End:
 */
