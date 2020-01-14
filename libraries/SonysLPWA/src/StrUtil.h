/*
 * Copyright 2017 Sony Semiconductor Solutions Corporation
 *
 * $Rev: 1055 $
 * $Date:: 2017-08-29 13:44:38 +0900#$
 *
 * 文字列操作
 */

#ifndef StrUtil_h
#define StrUtil_h

class StrUtil {
 public:
  /*
   * 文字列の右側も空白を削除します。ここでの空白は、スペース、改行、復帰、
   * 改頁文字です。
   *
   * 引数で指定したポインタと同じ値を返却します。
   */
  static char *trimRight(char *lpszStr);

  /*
   * 文字列の左側の空白を削除します。ここでの空白は、スペース、改行、復帰、
   * 改頁文字です。
   *
   * 引数で指定した文字列と同じポインタを返却します。
   *
   */
  static char *trimLeft(char *lpszStr);

  /*
   * 文字列の左右の空白を削除します。ここでの空白は、スペース、改行、復帰、
   * 改頁文字です。
   *
   * 引数で指定したポインタと同じ値を返却します。
   */
  static char *trim(char *lpszStr);

  /*
   * 文字列を空白文字でスプリットした結果を配列へ格納します。元の文字列
   * はnull終端で区切られます。スプリット数を返却します。
   */
  static int split(char *lpszStr, char *lpszArray[], int iMax);
};
#endif
/*
 * Local Variables:
 * coding: utf-8
 * mode: Arduino
 * End:
 */
