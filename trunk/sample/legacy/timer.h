#ifndef _TIMER_H_
#define _TIEMR_H_

#include "types.h"

/* タイマースタート */
extern void timer_start(void);

/* タイマーストップ */
extern void timer_stop(void);

/* 
 * コンペア値の設定 
 * 周期：      20MHz
 * コンペア値：cmp
 */
extern void timer_setcmp(uint16 cmp);

/* カウント取得 */
extern void timer_getcount(uint16 *count);

/* タイマー初期化 */
extern void timer_init(void);

#endif /* _TIMER_H_ */
