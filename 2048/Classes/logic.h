#pragma once
// ��
void isLeft() {
	/**
	* ������߼���������� 
	1.���Ϊ�գ�ֱ���󻬵����һ�� 
	2.��������ĵ�һ��������ȵģ������ 
	3.��������ĵ�һ�����ǲ���ȵģ������Ա�

	���ƶ����ٺϲ���
	2024->0224->0008

	*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// ������һ��һ��ȥ������
			for (int j2 = j + 1; j2 < 4; j2++) {
				// ���˵������ֵ
				if (cards[j2][i].getNum() > 0) {
					// �����ǰλ����Ϊ0,�ͷ������λ����ȥ
					if (cards[j][i].getNum() <= 0) {
						cards[j][i].setNum(cards[j2][i].getNum());
						// ��ԭ��λ���ϵ��������
						cards[j2][i].setNum(0);
						// ��ͼ�μ�������
						j--;

						// ��ֵ�����һ���ͬ
					}
					else if (cards[j][i].equals(cards[j2][i])) {
						// �ϲ�,��������һ���������д����������ӣ���ʵ2048�����ϵ����ֶ���˫���ģ�
						//��������ֻҪԭ����*2�Ϳ�����
						cards[j][i].setNum(cards[j][i].getNum() * 2);
						// ��ԭ��λ���ϵ��������
						cards[j2][i].setNum(0);

					}
					break;
				}
			}
		}
	}
}
/******************************************************************/
bool  HelloWorld::doLeft() {
	log("go left");
	/*
	��һ����Ϸ���߼�������ʱ
	step1:���е�card���������ƣ�����ֵΪ0��card),����߻��������
	step2:�϶�����ߵ��Ⱥϲ����������ͬ�ľͺϲ����ϲ����ֻ��п�card,��card�����cardҪ���card��λ�á�
	*/
	bool isdo = false;
	for (int y = 0; y < 4; y++) {
		//bool flag = true;
		//while (flag) {
		//	int x_num = 4;
		//	for (int x = 0; x < x_num; x++) {
		//		//����������Ϊ�գ�����Ķ���ǰ��
		//		if (cardArr[x][y]->getNumber() == 0) {
		//			for (int x1 = x + 1; x1 < x_num; x1++) 
		//				cardArr[x1 - 1][y]->setNumber(cardArr[x1][y]->getNumber());		
		//			cardArr[3][y]->setNumber(0);
		//			x_num--;//�ƶ������һ���ǿգ������������ж���
		//		}
		//		//������ӷǿգ���ô�жϺϲ�����if������else����Ϊ�ƶ�����ܷǿ�
		//		if(cardArr[x][y]->getNumber() != 0){
		//			for (int x1 = x + 1; x1 < x_num; x1++) {
		//				if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
		//					cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
		//					cardArr[x1][y]->setNumber(0);
		//				}
		//				else break;//����Ⱦ�ɶҲ����
		//			}
		//		}
		//	}
		//}


		for (int x = 0; x < 4; x++) {//�ϲ���x
									 //������x,card������ǰ�ƣ���x�����(x+1)�Ⱥ�x���е����ж�
									 //�жϵ�����Ҫ���е����������
			for (int x1 = x + 1; x1 < 4; x1++) {//x1�Ǳ��ϲ��ĸ���
				if (cardArr[x1][y]->getNumber()>0) {//x1�ǿ�
													//xΪ�գ�x1�ƶ���x��λ�ã�x1ԭλ���ÿ�
													//���ӣ�x=0,x1=1,[0,2,2,2],���󻬶���[2,0,2,2],x1=2,[4,0,0,2],x1=3,[4,0,0,2]
													//xΪ��ֻ�������for�г���һ�Σ���һ�ι���ͻᱻ����Ϊ�ǿա�
					if (cardArr[x][y]->getNumber() <= 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						//x--;//Ȼ����һ��x1�ж�ʱ���ȡcardArr[-1][y]�����жϣ���[0,2,4,8]

						//log("x=%d", x);		
						isdo = true;
					}//x�ǿգ��ж��Ƿ���x��ͬ,��ͬ��ϲ���x
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						//�ϲ���x��x1��Ϊ�գ�Ȼ����һ��x1��x�����жϣ������ͬ������ϲ�
						//��[2,2,4,4]�ϲ������ս��Ϊ[8,0,0,4]
						cardArr[x1][y]->setNumber(0);
						isdo = true;
					}//x�ǿ��Һ�x1��ͬ��ʲô��������x1ͣ��ԭ��
					break;
				}//x1Ϊ��ʲô������
			}
		}
	}
	return isdo;
}