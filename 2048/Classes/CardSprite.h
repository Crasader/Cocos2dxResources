
#ifndef __CardSprite_H__
#define __CardSprite_H__

#include "cocos2d.h"
USING_NS_CC;
class CardSprite : public cocos2d::Sprite
{
private:
	int number;//��ʾ�ڽ����е�����
	Label* labelCardNumber;//���ֱ�ǩ
	LayerColor* layerColorBG;//��ɫ��
private:
	void enemyInit(int numbers, int width, int height);
public:
	static CardSprite* createCardSprite(int numbers,int width,int height);
	virtual bool init();
	CREATE_FUNC(CardSprite);
	int getNumber();
	void setNumber(int num);
};

#endif // __CardSprite_H__
