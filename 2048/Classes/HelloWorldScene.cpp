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
	score = 0;//��ʼ����Ϸ����
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӱ���
	auto  layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
	this->addChild(layerColorBG);
	//���ķ�����ǩ
	auto dic = Dictionary::createWithContentsOfFile("fonts/2048.xml");
	auto str = (String*)(dic->objectForKey("score"));
	scoreLabel = Label::createWithTTF(str->getCString(), "fonts/STLITI.ttf", 60);
	scoreLabel->setPosition(Vec2(visibleSize.width / 5, visibleSize.height - 60));
	this->addChild(scoreLabel);
	//����ֵ��ǩ��
	scoreValueLabel = Label::createWithTTF(
		String::createWithFormat("%d",score)->getCString(), "fonts/Marker Felt.ttf", 60);
	scoreValueLabel->setPosition(Vec2(visibleSize.width*2/ 5, visibleSize.height - 60));
	scoreValueLabel->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(scoreValueLabel);
	//��Ӵ������¼�
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//��ӿ�Ƭ
	createCards(visibleSize);
	//���������
	autoCreateCardNumber();
	autoCreateCardNumber();
}
void  HelloWorld::createCards(Size size) {
	//�����Ԫ��߶�
	float lon = (size.width - 20) / 4;//���ҷ�����20����
	//���ֺ�card��ʣ��ĸ߶�
	float rest_height = size.height - lon * 4;
	//int arr[4][4] = {{2,2,4,8},
	//				{ 0,2,4,8 },
	//				{ 0,2,4,8 },
	//				{ 2,4,2,2 }};
	int arr[4][4] = {0};
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
	beginP = touch->getLocation();
	return true;
}
void  HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {
	endP = touch->getLocation();

	Vec2 chaP = beginP - endP;//����ȥ�յ�
	float chaX = chaP.x;
	float chaY = chaP.y;
	float minDistance = 5;//��С���룬������

	//���x�����ϵ��ƶ��������y�����ϵ��ƶ����룬���������ƶ������������ƶ�
	if (abs(chaX) > abs(chaY)) {
		if (chaX  > minDistance) {//begin��end�ұߣ�����
			if (doLeft()) {//return��true��ʾ�������ƶ����ߺϲ�(��ʾ���пո�)����ʱ��Ҫ���������
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
		else if(chaX < -minDistance){
			if(doRight()){
				autoCreateCardNumber();
				doCheckGameOver();
			}
		}
	}
	else {//opengl����ϵԭ�������½�,Խ��������Խ��
		if (chaY  > minDistance) {//first��end���ϱߣ����»�
			if (doDown()) {
				autoCreateCardNumber();
				doCheckGameOver();
			}
				
		}
		else if(chaY < -minDistance){
			if(doUp()){
				autoCreateCardNumber();
				doCheckGameOver();
			}
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
	int merge_num = 0;//�ϲ�����
	int move_num = 0;//�ƶ�����
	for (int y = 0; y < 4; y++) {//����ÿһ��
		/*
		x = 0:2222->4022	2248->4048->8008->16000
		x = 2:4022->4040	
		x = 0:8000	
		*/	
		bool isdo = false;//һ�λ������ϲ�һ��	
		for (int x = 0; x < 4; x++) {
			if (cardArr[y][x]->getNumber() > 0) {//�ǿո���x
				for (int nx = x + 1; nx < 4; nx++) {
					if (cardArr[y][nx] != 0) {//����һ���ǿո���
						if (cardArr[y][x]->getNumber() == cardArr[y][nx]->getNumber()) {//���
							//���кϲ�����
							cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
							cardArr[y][nx]->setNumber(0);
							//���ӷ���
							score += cardArr[y][x]->getNumber();
							scoreValueLabel->setString(String::createWithFormat("%d", score)->getCString());

							isdo = true;
							merge_num++;
						}
						else {//���������ķǿո��������������ȾͲ��ܺϲ�
							break;
						}
					}

					
				}
			}
		}
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
						move_num++;
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return merge_num>0||move_num>0;//�ϲ����ƶ����ͷ���true
}
bool  HelloWorld::doRight() {
	log("go right");
	int merge_num = 0;
	int move_num = 0;
	for (int y = 0; y < 4; y++) {//����ÿһ��
		//�ϲ�
		bool isdo = false;
		for (int x = 3; x >=0; x--) {
			if (cardArr[y][x]->getNumber() > 0) {//�ǿո���x
				for (int nx = x - 1; nx>=0; nx--) {//����һ���ǿ��Һ�x��ͬ�ĸ���
					if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[y][nx]->getNumber()) {
						//���кϲ�����
						cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
						cardArr[y][nx]->setNumber(0);
						//���ӷ���
						score += cardArr[y][x]->getNumber();
						scoreValueLabel->setString(String::createWithFormat("%d", score)->getCString());

						isdo = true;
						merge_num++;
					}
				}
			}
		}
		//�ƶ�
		for (int x = 3; x >=0; x--) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int nx = x - 1; nx >=0; nx--) {
					if (cardArr[y][nx]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[y][nx]->getNumber());
						cardArr[y][nx]->setNumber(0);
						move_num++;
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return merge_num>0 || move_num>0;
}
bool  HelloWorld::doUp() {
	log("go up");
	int merge_num = 0;
	int move_num = 0;
	for (int x = 0; x < 4; x++) {//����ÿһ��
		/*�ϲ�*/
		bool isdo = false;

		for (int y = 0; y < 4; y++) {
			if (cardArr[y][x]->getNumber() > 0) {//�ǿո���y
				for (int ny = y + 1; ny < 4; ny++) {//����һ���ǿ��Һ�y��ͬ�ĸ���
					if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[ny][x]->getNumber()) {
						//���кϲ�����
						cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
						cardArr[ny][x]->setNumber(0);
						//���ӷ���
						score += cardArr[y][x]->getNumber();
						scoreValueLabel->setString(String::createWithFormat("%d", score)->getCString());

						isdo = true;
						merge_num++;
					}
				}
			}
		}

		/*�ƶ�*/
		for (int y = 0; y < 4; y++) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int ny = y + 1; ny < 4; ny++) {
					if (cardArr[ny][x]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[ny][x]->getNumber());
						cardArr[ny][x]->setNumber(0);
						move_num++;
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return merge_num>0 || move_num>0;
}
bool  HelloWorld::doDown() {
	log("go down");
	int merge_num = 0;
	int move_num = 0;
	for (int x = 0; x < 4; x++) {//����ÿһ��
		//�ϲ�
		bool isdo = false;
		for (int y = 3; y >=0; y--) {
			if (cardArr[y][x]->getNumber() > 0) {//�ǿո���y
				for (int ny = y - 1; ny >=0; ny--) {//����һ���ǿ��Һ�y��ͬ�ĸ���
					if (/*cardArr[y][nx]!=0&&*/cardArr[y][x]->getNumber() == cardArr[ny][x]->getNumber()) {
						//���кϲ�����
						cardArr[y][x]->setNumber(cardArr[y][x]->getNumber() * 2);
						cardArr[ny][x]->setNumber(0);
						//���ӷ���
						score += cardArr[y][x]->getNumber();
						scoreValueLabel->setString(String::createWithFormat("%d", score)->getCString());

						isdo = true;
						merge_num++;
					}
				}
			}
		}
		//�ƶ�
		for (int y = 3;y>=0; y--) {
			if (cardArr[y][x]->getNumber() == 0) {//��һ���ո�
				for (int ny = y-1; ny >=0; ny--) {
					if (cardArr[ny][x]->getNumber() > 0) {//��һ���ǿ�
						cardArr[y][x]->setNumber(cardArr[ny][x]->getNumber());
						cardArr[ny][x]->setNumber(0);
						move_num++;
						break;//�ո�����ֱ��break
					}
				}
			}
		}
	}
	return merge_num>0||move_num>0;
}
//������ӱ�ռ���˻����ɾͻ�������ѭ��������Ӧ���ںϲ������ƶ�������ʾ�пո񣩺���������������
void HelloWorld::autoCreateCardNumber()
{
	int y, x;
	do {
		y = CCRANDOM_0_1() * 4;//[0,1]*4 ->[0,4],
		x = CCRANDOM_0_1() * 4;
		if (y == 4)y = 3;//����պõ���4�ͻ����
		if (x == 4)x = 3;
	} while (cardArr[y][x]->getNumber() > 0);//����ĸ��ӷǿվͼ��������һ����

	cardArr[y][x]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);//4�ĸ���Լ10%
}

void HelloWorld::doCheckGameOver()
{
	bool isGameOver = true;
	//�ж�ÿ�����ӣ����пո���Ϸ���Լ���
	//���߿��Ժ����ܱߵĸ��Ӻϲ������ԣ���ô��Ϸ���Լ���
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if ((cardArr[y][x]->getNumber() == 0) ||
				(y-1 >= 0 && (cardArr[y][x]->getNumber() == cardArr[y - 1][x]->getNumber())) ||
				(y+1 <= 3 && (cardArr[y][x]->getNumber() == cardArr[y + 1][x]->getNumber())) ||
				(x-1 >= 0 && (cardArr[y][x]->getNumber() == cardArr[y][x - 1]->getNumber())) ||
				(x+1 <= 3 && (cardArr[y][x]->getNumber() == cardArr[y][x + 1]->getNumber()))
				) {
				isGameOver = false;
				break;
			}			
		}
	}
	if (isGameOver) {//�����Ϸ�����������¼��س���
		auto scene = HelloWorld::createScene();
		auto tsc = TransitionFade::create(1, scene);
		Director::getInstance()->replaceScene(tsc);
	}
}
