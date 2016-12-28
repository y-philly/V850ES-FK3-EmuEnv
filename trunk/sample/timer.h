#ifndef _TIMER_H_
#define _TIEMR_H_

#include "types.h"

/* �^�C�}�[�X�^�[�g */
extern void timer_start(void);

/* �^�C�}�[�X�g�b�v */
extern void timer_stop(void);

/* 
 * �R���y�A�l�̐ݒ� 
 * �����F      20MHz
 * �R���y�A�l�Fcmp
 */
extern void timer_setcmp(uint16 cmp);

/* �J�E���g�擾 */
extern void timer_getcount(uint16 *count);

/* �^�C�}�[������ */
extern void timer_init(void);

#endif /* _TIMER_H_ */
