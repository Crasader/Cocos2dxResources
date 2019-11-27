/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CardSprite.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӱ���
	auto  layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
	this->addChild(layerColorBG);
	//��Ӵ������¼�
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//��ӿ�Ƭ
	createCards(visibleSize);
}
void  HelloWorld::createCards(Size size) {
	//�����Ԫ��߶�
	float lon = (size.width - 20) / 4;//���ҷ�����20����
	//���ֺ�card��ʣ��ĸ߶�
	float rest_height = size.height - lon * 4;
	int arr[4][4] = {{2,2,4,8},
					{ 0,2,4,8 },
					{ 0,2,4,8 },
					{ 0,0,4,8 }};
	//4*4�ĵ�Ԫ��,��x,y������card,ͬopengl����ϵ
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			auto card = CardSprite::createCardSprite(2 * x, lon, lon);
			//�������10����(����card�߽�����7.5,����ܹ�����17.5����)��y�᷽���ϵĿհ����¸�һ��
			card->setNumber(arr[y][x]);
			//card->setPosition(Vec2(lon*x + 10, lon*y - rest_height / 2 ));//(0,0)�����½�
			card->setPosition(Vec2(lon*x + 10, size.height-lon*y - rest_height / 2-lon));//(0,0)�����Ͻ�
			this->addChild(card);
			//��ӿ�Ƭ����ά������
			cardArr[x][y] = card;//��֤����x���Ǳ������ҷ���ĸ��Ӿ��С�
		}
	}
}

bool  HelloWorld::onTouchBegan(Touch *touch, Event *unused_event) {
	Point touchP0 = touch->getLocation();
	firstX = touchP0.x;
	firstY = touchP0.y;
	return true;
}
void  HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {
	Point touchP0 = touch->getLocation();
	endX = firstX - touchP0.x;
	endY = firstY - touchP0.y;
	
	//���x�����ϵ��ƶ��������y�����ϵ��ƶ����룬���������ƶ������������ƶ�
	if (abs(endX) > abs(endY)) {
		if (endX + 5 > 0) {//first��end�ұߣ�����
			doLeft();
		}
		else {
			doRight();
		}
	}
	else {//opengl����ϵԭ�������½�,Խ��������Խ��
		if (endY + 5 > 0) {//first��end���ϱߣ����»�������ΪʲôҪ+5������
			doDown();
		}
		else {
			doUp();
		}
	}

}

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
					else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber()*2);
						//�ϲ���x��x1��Ϊ�գ�Ȼ����һ��x1��x�����жϣ������ͬ������ϲ�
						//��[2,2,4,4]�ϲ������ս��Ϊ[8,0,0,4]
						cardArr[x1][y]->setNumber(0);
						isdo = true;
					}//x�ǿ��Һ�x1��ͬ��ʲô��������x1ͣ��ԭ��
				}//x1Ϊ��ʲô������
			}
		}
	}
	return isdo;
}
bool  HelloWorld::doRight() {
	log("go right");
	return true;
}
bool  HelloWorld::doUp() {
	log("go up");
	return true;
}
bool  HelloWorld::doDown() {
	log("go down");
	return true;
}