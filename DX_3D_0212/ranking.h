#ifndef _RANKING_H_
#define _RANKING_H_

#define MAX_RANKING ( 5 )								//���ʐ�
#define RANKING_POLYGON ( 4 )							//�X�R�A����
#define MAX_RANK ( 4 )									//���ʃ|���S���p
#define RANKINGPOLYGON_VERTEX ( (RANKING_POLYGON * 4) * MAX_RANKING  + (MAX_RANK * 4))		//���_��
#define RANKINGPOLYGON_WIDTH ( 80 )						//�|���S���̕�
#define RANKINGPOLYGON_HEIGHT ( 50 )					//�|���S���̍���
#define RANKPOLYGON_WIDTH ( 150 )						//�|���S���̕�
#define RANKPOLYGON_HEIGHT ( 60 )						//�|���S���̍���
#define RANKINGPOLYGON_TEXTURE ( 5 )					//�e�N�X�`����
#define RANKINGTEXTURE_WIDTH ( 1 )						//�e�N�X�`���̍s����
#define RANKINGTEXTURE_HEIGHT ( 10 )					//�e�N�X�`���̗񖇐�
#define NEXTRANKSPACE ( 130 )							//���ʂ܂ł̋��
#define STARTPOINTX ( 100 )								//�����L���OX�`��J�n�ʒu
#define STARTPOINTY ( 70 )								//�����L���OY�`��J�n�ʒu

int InitRanking(void);									//�|���S���̏���������
void UninitRanking(void);								//�|���S���̏I������
void UpdateRanking(void);								//�|���S���̍X�V����
void DrawRanking(void);									//�|���S���`�揈��

#endif