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

#include "SecondScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* Second::createScene()
{
	//auto scene = Second::create();
	auto scene = Scene::create();
	auto layer = Second::create();
	scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SecondScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Second::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	setTouchEnabled(true);
	//���ô����¼�Ϊ���㴥��
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin =  Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	/*�����Ĵ���*******************************************************************************/
	auto bg = Sprite::create("BackgroundTile.png",
		Rect(0,0,visibleSize.width,visibleSize.height));
	Texture2D::TexParams tp = { GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT };//���Բ�ֵ���ظ�ƽ��
	bg->getTexture()->setTexParameters(tp);
	bg->setPosition(center);
	this->addChild(bg);

	/*������������*******************************************************************************/

	boxA = Sprite::create("BoxA2.png");
	boxA->setPosition(center + Vec2(-120, 120));
	this->addChild(boxA, 10, BOX_A);

	boxB = Sprite::create("BoxB2.png");
	boxB->setPosition(center);
	this->addChild(boxB, 11, BOX_B);

	boxC = Sprite::create("BoxC2.png");
	boxC->setPosition(center + Vec2(120, 120));
	this->addChild(boxC, 12, BOX_C);

	auto nextLabel = Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 36);
	auto menuItem = MenuItemLabel::create(nextLabel,
		CC_CALLBACK_1(Second::goCallback,this));
	auto menu = Menu::create(menuItem, NULL);
	menu->setColor(Color3B(1, 0, 0));
	menu->setPosition(Vec2(100, visibleSize.height - 100));
	this->addChild(menu);
	return true;
}
/**
 * һ�����⣺��ȻҪ���ñ�ǩ����Ϊʲô������һ���������ó����Ա����
 */
void  Second::onEnter() {
	Layer::onEnter();
	//���ٶ��¼�������
	setAccelerometerEnabled(true);
}
void  Second::onExit() {
	Layer::onExit();
	EventDispatcher* disp = Director::getInstance()->getEventDispatcher();
	disp->removeEventListenersForTarget(this);
	//disp->removeAllEventListeners();
}

/*
������е�һ���Ӷ����Ѿ��������ˣ���ô��������Ͳ�����Ӧ�����¼���֪������������
*/
bool Second::onTouchBegan(Touch* touch, Event* event) {
	setTouchedObject(touch);
	if (touchedObject != NULL)
		touchedObject->runAction(ScaleTo::create(0.1f, 0.5f));
	return true;
}
void Second::onTouchMoved(Touch* touch, Event* event) {
	if(touchedObject !=NULL)
		touchedObject->setPosition(touchedObject->getPosition() + touch->getDelta());
}
void Second::onTouchEnded(Touch* touch, Event* event) {
	if (touchedObject != NULL)
		touchedObject->runAction(ScaleTo::create(0.1f, 1.0f));
}

void Second::onAcceleration(Acceleration * acc, Event * event)
{
	float SPEED = acc->x/acc->timestamp;//SPEEDû�ж��壬�ȼ�����һ����ֹ����
	log("onAcceleration x=%f,y=%f,z=%f", acc->x, acc->y, acc->z);
	auto size = Director::getInstance()->getVisibleSize();
	//auto ball = (Sprite*)this->getChildByTag(kball_tag);
	Size s = ball->getContentSize();
	Vec2 p0 = ball->getPosition();

	float p1x = p0.x + acc->x*SPEED;
	if (p1x + s.width / 2 < 0)
		p1x = s.width/2;
	if (p1x + s.width / 2 > size.width)
		p1x = size.width - s.width / 2;

	float p1y = p0.y + acc->y*SPEED;
	if (p1y + s.height / 2 < 0)
		p1y = s.height / 2;
	if (p1y + s.height / 2 > size.height)
		p1y = size.height - s.height / 2;

	ball->runAction(Place::create(Vec2(p1x, p1y)));
}

Sprite* Second::setTouchedObject(Touch * touch)
{
	Sprite* boxs[3];
	boxs[2] = boxA;
	boxs[1] = boxB;
	boxs[0] = boxC;
	for (int i = 0; i < 3; i++){ 
		//�����������������ת��Ϊnode�ı�������
		Vec2 TLocaltoNode = boxs[i]->convertToNodeSpace(touch->getLocation());
		Size s = boxs[i]->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		//�����Χ�жϼ��
		if (rect.containsPoint(TLocaltoNode)) {//boxC�����ȼ�ʱ��ߵ�,�ڵ�һ����⡣
			return touchedObject = boxs[i];
		}
	}
	return NULL;//һ����û�д�����
}


void Second::goCallback(Ref* pSender)
{
	auto sc = HelloWorld::createScene();
	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}
