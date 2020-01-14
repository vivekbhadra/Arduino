/*
 * Copyright 2017 Sony Semiconductor Solutions Corporation
 *
 * $Rev: 1055 $
 * $Date:: 2017-08-29 13:44:38 +0900#$
 *
 * 文字列操作
 */
#ifndef _UNIT_TEST_
#    include <Arduino.h>
#else
#    include <stdio.h>
#    include <string.h>
#endif

#include "StrUtil.h"

static bool
isSpaceChar(char c)
{
  return '\r' == c || '\n' == c || '\t' == c || '\f' == c || ' ' == c;
}

/*
 * 文字列の右側も空白を削除します。ここでの空白は、スペース、改行、復帰、
 * 改頁文字です。
 *
 * 引数で指定したポインタと同じ値を返却します。
 */
char *
StrUtil::trimRight(char *lpszStr)
{
  size_t len = strlen(lpszStr);

  for (--len; len; len--) {
    if (isSpaceChar(*(lpszStr + len))) {
      *(lpszStr + len) = '\0';
    } else {
      break;
    }
  }
  return lpszStr;
}

/*
 * 文字列の左側の空白を削除します。ここでの空白は、スペース、改行、復帰、
 * 改頁文字です。
 *
 * 引数で指定した文字列と同じポインタを返却します。
 *
 */
char *
StrUtil::trimLeft(char *lpszStr)
{
  char *pFrom = lpszStr;
  char *pTo = lpszStr;

  for (; *pFrom; pFrom++) {
    if (!isSpaceChar(*pFrom)) {
      break;
    }
  }

  if (pFrom == pTo) {                         // 余分な文字は無い
    return lpszStr;
  }
  for (; *pFrom; pFrom++, pTo++) {
    *pTo = *pFrom;
  }
  *pTo = '\0';
  return lpszStr;
}

/*
 * 文字列の左右の空白を削除します。ここでの空白は、スペース、改行、復帰、
 * 改頁文字です。
 *
 * 引数で指定したポインタと同じ値を返却します。
 */
char *
StrUtil::trim(char *lpszStr)
{
  return trimLeft(trimRight(lpszStr));
}

/*
 * 文字列を空白文字でスプリットした結果を配列へ格納します。元の文字列は
 * null終端で区切られます。スプリット数を返却します。
 */
int
StrUtil::split(char *lpszStr, char *lpszArray[], int iMax)
{
  int cnt = 0;
  char *p = lpszStr;

  StrUtil::trim(lpszStr);

  for (p = lpszStr; *p ; p++) {
    if (!isSpaceChar(*p)) {
      lpszArray[cnt] = p;
      cnt++;
      if (cnt >= iMax) {
        return cnt;
      }
      while (!isSpaceChar(*p)) {
        p++;
        if ('\0' == *p) {
          return cnt;
        }
      }
      *p = '\0';
    }
  }
  return cnt;
}

#ifdef _UNIT_TEST_
int
main(void)
{
  char test[] = "          123     ";
  printf("[%s]\n", test);
  printf("[%s]\n", StrUtil::trimLeft(test));
  printf("[%s]\n", StrUtil::trimRight(test));

  char line[] = "  123   456   789  ABC  DEF   ";
  char *tbl[5];

  printf("[%s]\n", line);
  int cnt = StrUtil::split(line, tbl, 4);

  for (int i = 0; i < cnt; i++) {
    printf("[%d] %s\n", i, tbl[i]);
  }
}
#endif

/*
 * Local Variables:
 * coding: utf-8
 * mode: Arduino
 * End:
 */
