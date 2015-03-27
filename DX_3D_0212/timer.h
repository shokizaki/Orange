#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_DIGITS ( 3 )
#define TIMERPOLYGON_VERTEX ( TIMER_DIGITS * 4 )		//���_��
#define TIMERPOLYGON_WIDTH ( 50 )						//�|���S���̕�
#define TIMERPOLYGON_HEIGHT ( 30 )						//�|���S���̍���
#define TIMERPOLYGON_TEXTURE ( 1 )						//�e�N�X�`����
#define TIMERTEXTURE_WIDTH ( 1 )						//�e�N�X�`���̍s����
#define TIMERTEXTURE_HEIGHT ( 10 )						//�e�N�X�`���̗񖇐�
#define LIMITTIME ( 500 )									//��������

int InitTimer(void);									//�|���S���̏���������
void UninitTimer(void);									//�|���S���̏I������
int UpdateTimer(void);									//�|���S���̍X�V����
void DrawTimer(void);									//�|���S���`�揈��
int GetTimer(void);
#endif