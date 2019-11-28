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
			cardArr[y][x] = card;//��֤����x���Ǳ������ҷ���ĸ��Ӿ��С�
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
	���ƶ����ٺϲ�:
	4202->4220->4400->8000, �ƶ��׶Σ��������ұ�����������ӷǿ��������Ϊ�գ��ͰѸ��������ƶ�һ��
	4002->�ƶ��׶Σ�
	0204->�ƶ��׶Σ��������ұ������������aΪ�գ���ô��������������ң�ֱ���ҵ�һ���ǿյĸ���b��Ȼ��a=b,b=0��������������һ���յĸ��ӡ�
	4220->�ϲ��׶Σ���ʱ���и��Ӷ���������ڣ��������ұ�������һ����ͬ�ľͺϲ�->4400,֮���������߻�����µĿɺϲ����Ӷԣ�������Ҫ���´��������
	4220->4400->8000
	4420->8200
	4422->8220->8400->
	2222->4220->4400->8000
	2222�ϲ�->4022�ƶ�->4220�ϲ�->4400�ϲ�->8000, �ϲ�֮����Ҫ�����ƶ�����Ϊ���滹���ܳ��ֿɺϲ���
	2222�ϲ�->4022�ϲ�->4040�ƶ�->4400�ϲ�->8000
	�����������ϲ�ʱ���Կո�ֱ��û�пɺϲ��Ĳ��������ƶ�
	
	
	*/
	int isdo_num = 0;
	for (int y = 0; y < 4; y++) {//����ÿһ��
		/*
		x = 0:2222->4022	2248->4048->8008->16000
		x = 2:4022->4040	
		x = 0:8000
		
		*/	
		bool isdo = false;
		do{//���ͺϲ����Σ�ʱ�临�Ӷ��O(16*3);
			isdo = false;
			for (int x = 0; x < 4; x++) {
				if (cardArr[y][x]->getNumber() > 0) {//�ǿո���x
					for (int nx = x + 1; nx < 4; nx++) {//����һ���ǿ��Һ�x��ͬ�ĸ���
						if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[y][nx]->getNumber()) {
							//���кϲ�����
							cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
							cardArr[y][nx]->setNumber(0);
							isdo = true;
							isdo_num++;
						}
					}
				}
			}
		}while (isdo);//����ܺϲ��ͼ����������ٺϲ��˾��˳�

		//�ƶ�
		/*	�������ұ������������aΪ�գ���ô��������������ң�ֱ���ҵ�һ���ǿյĸ���b��
			Ȼ��a = b, b = 0��������������һ���յĸ��ӡ�
		*/
		for (int x = 0; x < 4; x++) {
			if (cardArr[y][x]->getNumber()==0) {//��һ���ո�
				for (int nx = x + 1; nx < 4; nx++) {
					if (cardArr[y][nx]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[y][nx]->getNumber());
						cardArr[y][nx]->setNumber(0);
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return isdo_num>0;//�ϲ���
}
bool  HelloWorld::doRight() {
	log("go right");
	int isdo_num = 0;
	for (int y = 0; y < 4; y++) {//����ÿһ��
		//�ϲ�
		bool isdo = false;
		do {//���ͺϲ����Σ�ʱ�临�Ӷ��O(16*3);
			isdo = false;
			for (int x = 3; x >=0; x--) {
				if (cardArr[y][x]->getNumber() > 0) {//�ǿո���x
					for (int nx = x - 1; nx>=0; nx--) {//����һ���ǿ��Һ�x��ͬ�ĸ���
						if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[y][nx]->getNumber()) {
							//���кϲ�����
							cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
							cardArr[y][nx]->setNumber(0);
							isdo = true;
							isdo_num++;
						}
					}
				}
			}
		} while (isdo);//����ܺϲ��ͼ����������ٺϲ��˾��˳�
		//�ƶ�
		for (int x = 3; x >=0; x--) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int nx = x - 1; nx >=0; nx--) {
					if (cardArr[y][nx]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[y][nx]->getNumber());
						cardArr[y][nx]->setNumber(0);
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return true;
}
bool  HelloWorld::doUp() {
	log("go up");
	int isdo_num = 0;
	for (int x = 0; x < 4; x++) {//����ÿһ��
		/*�ϲ�*/
		bool isdo = false;
		do {//���ͺϲ����Σ�ʱ�临�Ӷ��O(16*3);
			isdo = false;
			for (int y = 0; y < 4; y++) {
				if (cardArr[y][x]->getNumber() > 0) {//�ǿո���y
					for (int ny = y + 1; ny < 4; ny++) {//����һ���ǿ��Һ�y��ͬ�ĸ���
						if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[ny][x]->getNumber()) {
							//���кϲ�����
							cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
							cardArr[ny][x]->setNumber(0);
							isdo = true;
							isdo_num++;
						}
					}
				}
			}
		} while (isdo);//����ܺϲ��ͼ����������ٺϲ��˾��˳�
		/*�ƶ�*/
		for (int y = 0; y < 4; y++) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int ny = y + 1; ny < 4; ny++) {
					if (cardArr[ny][x]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[ny][x]->getNumber());
						cardArr[ny][x]->setNumber(0);
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return true;
}
bool  HelloWorld::doDown() {
	log("go down");
	int isdo_num = 0;
	for (int x = 0; x < 4; x++) {//����ÿһ��
		//�ϲ�
		bool isdo = false;
		do {//���ͺϲ����Σ�ʱ�临�Ӷ��O(16*3);
			isdo = false;
			for (int y = 3; y >=0; y--) {
				if (cardArr[y][x]->getNumber() > 0) {//�ǿո���y
					for (int ny = y - 1; ny >=0; ny--) {//����һ���ǿ��Һ�y��ͬ�ĸ���
						if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[ny][x]->getNumber()) {
							//���кϲ�����
							cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
							cardArr[ny][x]->setNumber(0);
							isdo = true;
							isdo_num++;
						}
					}
				}
			}
		} while (isdo);//����ܺϲ��ͼ����������ٺϲ��˾��˳�
		//�ƶ�
		for (int y = 3;y>=0; y--) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int ny = y-1; ny >=0; ny--) {
					if (cardArr[ny][x]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[ny][x]->getNumber());
						cardArr[ny][x]->setNumber(0);
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return true;
}
