#include "CardSprite.h"
void CardSprite::cardInit(int numbers, int width, int height) {
	//��ʼ������
	number = numbers;
	//������Ϸ��Ƭ�ı�����ɫ�飬����15���صĿ���λ�ã��ĸ������7.5��
	layerColorBG = LayerColor::create(Color4B(200, 190, 180, 255), width - 15, height - 15);
	/*layerColorBGӦ����sprite�����м䣬������������ô����anchorPoint��ʾʱ���൱���ǣ�0,0��
	����x,y����ԭ��Ļ�����ƫ�Ƽ����һ�㡣*/
	layerColorBG->setPosition(Point(7.5, 7.5));
	//��������
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
//�ú����ͳ����� ����::create() ���������ƣ����Ǿ�̬�Ĵ���ʵ���ķ���
CardSprite* CardSprite::createCardSprite(int numbers, int width, int height) {
	CardSprite * card = new CardSprite();
	if (card&&card->init()) {//���ø����init����
		card->autorelease();//��ӵ��Զ��ͷų�
		card->cardInit(numbers, width, height);
		return card;
	}
	else {
		CC_SAFE_DELETE(card);//��ʼ��ʧ�ܾ�ɾ��
		return NULL;
	}
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

	//�������ֵĴ�С����������Ĵ�С
	if (number >= 0)
		labelCardNumber->setSystemFontSize(90);
	if(number>= 10)
		labelCardNumber->setSystemFontSize(70);
	if(number>=100)
		labelCardNumber->setSystemFontSize(50);
	if(number>=1000)
		labelCardNumber->setSystemFontSize(30);

	//�������ִ�С��������ɫ
	switch (number)
	{
	case 0:layerColorBG->setColor(Color3B(200, 190, 180)); break;
	case 2:layerColorBG->setColor(Color3B(240, 230, 220)); break;
	case 4:layerColorBG->setColor(Color3B(240, 220, 200)); break;
	case 8:layerColorBG->setColor(Color3B(240, 180, 120)); break;
	case 16:layerColorBG->setColor(Color3B(240, 140, 90)); break;
	case 32:layerColorBG->setColor(Color3B(240, 120, 90)); break;
	case 64:layerColorBG->setColor(Color3B(240, 90, 60)); break;
	case 128:layerColorBG->setColor(Color3B(240, 90, 60)); break;
	case 256:layerColorBG->setColor(Color3B(240, 200, 70)); break;
	case 512:layerColorBG->setColor(Color3B(240, 200, 70)); break;
	case 1024:layerColorBG->setColor(Color3B(0, 130, 0)); break;
	case 2048:layerColorBG->setColor(Color3B(0, 130, 0)); break;
	default:
		break;
	}

	//������ʾ������
	if (number > 0) {
		labelCardNumber->setString(String::createWithFormat("%i", number)->getCString());
	}
	else {
		labelCardNumber->setString("");
	}
}
