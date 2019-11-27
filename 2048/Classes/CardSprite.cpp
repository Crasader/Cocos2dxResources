#include "CardSprite.h"
void CardSprite::enemyInit(int numbers, int width, int height) {
	//��ʼ������
	number = numbers;
	//������Ϸ��Ƭ�ı�����ɫ�飬����15���صĿ���λ�ã��ĸ������7.5��
	layerColorBG = LayerColor::create(Color4B(200, 190, 180, 255), width - 15, height - 15);
	/*layerColorBGӦ����sprite�����м䣬������������ô����anchorPoint��ʾʱ���൱���ǣ�0,0��
	����x,y����ԭ��Ļ�����ƫ�Ƽ����һ�㡣*/
	layerColorBG->setPosition(Point(7.5, 7.5));

	////��������
	const char* label_str = "";
	if (number > 0)//�ж����������0����ʾ������Ϊ�� 
		label_str = String::createWithFormat("%i", number)->getCString();		
	labelCardNumber = Label::createWithSystemFont(
			label_str, "HirakakuProN-W6", 100);	
	labelCardNumber->setPosition(Point(layerColorBG->getContentSize().width / 2,
		layerColorBG->getContentSize().height / 2));
	layerColorBG->addChild(labelCardNumber);
	this->addChild(layerColorBG);
}
CardSprite* CardSprite::createCardSprite(int numbers, int width, int height) {
	CardSprite * enemy = new CardSprite();
	if (enemy&&enemy->init()) {
		enemy->autorelease();//�����Ǹ�ʲô��
		enemy->enemyInit(numbers, width, height);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);//��ʼ��ʧ�ܾ�ɾ��
	return NULL;
}
bool CardSprite::init() {
	if (!Sprite::init()) {
		return false;
	}
	return true;
}
int CardSprite::getNumber() {
	return number;
}
void CardSprite::setNumber(int num) {
	number = num;

	//������ʾ������
	if (number > 0) {
		labelCardNumber->setString(String::createWithFormat("%i", number)->getCString());
	}
	else {
		labelCardNumber->setString("");
	}
}