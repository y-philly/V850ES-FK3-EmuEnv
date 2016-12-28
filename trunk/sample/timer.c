#include "types.h"
#include "prc_sil.h"
#include "v850esfk3.h"

/* �^�C�}�[������ */
void timer_init(void)
{
	/* �N���b�N�ݒ� */
	sil_wrb_mem((void*)TAAnCTL0(2),0x00);

	/*
	 * �N���b�N�I��
	 * �^�C�}�E���[�h�̑I��
	 */
	sil_wrb_mem((void*)TAAnCTL1(2),0x00);

	/*
	 * �R���y�A�l�̐ݒ�
	 * 1msec : 20000clock 0x4E20
	 */
	sil_wrh_mem((void*)TAAnCCR0(2), 0x4E20);

	/* ���荞�݂̃}�X�N�ݒ�
	 * �����݂�������
	 * 6bit:   0�F����
	 * 0-2bit: 7:�Œ�D��x
	 */
	sil_wrb_mem((void*)0xFFFFF13C,0x07);


	return;
}

/* �^�C�}�[�X�^�[�g */
void timer_start(void)
{	
	/* ���싖��
	 * (���N���b�N�ݒ�(����3�r�b�g)���㏑�������̂œ����l�ɂȂ�悤����) 
	 */
	sil_wrb_mem((void*)TAAnCTL0(2),0x80);
	return;
}

/* �^�C�}�[�X�g�b�v */
void tiemr_stop(void)
{
	/* ����֎~
	 * (���N���b�N�ݒ�(����3�r�b�g)���㏑�������̂œ����l�ɂȂ�悤����) 
	 */
	sil_wrb_mem((void*)TAAnCTL0(2),0x00);
	return;
}

/* �R���y�A�l�̐ݒ� */
void timer_setcmp(uint16 cmp)
{
	/*
	 * ����(clock��)�ŃR���y�A�l�̐ݒ�
	 */
	sil_wrh_mem((void*)TAAnCCR0(2), cmp);
	
	return;
}

/* �J�E���g�擾 */
void timer_getcount(uint16 *count)
{
	*count = sil_reh_mem((void*)TAAnCNT(2));
	return;	
}

