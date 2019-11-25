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
#include "SecondScene.h"
//#include "SecondScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
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
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin =  Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	/*�����Ĵ���*******************************************************************************/
	//����һ��һ��СͼƬ������������
	//auto bg = Sprite::create("BackGoundTile.png");
	//bg->setPosition(center);
	//this->addChild(bg);

	//��������������������ͼƬ
	//auto bg = Sprite::create("BackgroundTile.png", 
	//	Rect(0, 0, visibleSize.width, visibleSize.height));
	//bg->setPosition(center);
	//this->addChild(bg);

	//������
	/*
		ʹ��opengl�����������һ���������ԣ��������tp��һ���ṹ�壬
		�����еĲ������ú�����������ʹ��һ��С�ߴ�ͼƬ������Ⱦ�����ɵõ������Ч��
	*/
	auto bg = Sprite::create("BackgroundTile.png",
		Rect(0,0,visibleSize.width,visibleSize.height));
	Texture2D::TexParams tp = { GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT };//���Բ�ֵ���ظ�ƽ��
	bg->getTexture()->setTexParameters(tp);
	bg->setPosition(center);
	this->addChild(bg);

	/*������������*******************************************************************************/

	auto boxA = Sprite::create("BoxA2.png");
	boxA->setPosition(center + Vec2(-120, 120));
	this->addChild(boxA, 10, BOX_A);

	auto boxB = Sprite::create("BoxB2.png");
	boxB->setPosition(center);
	this->addChild(boxB, 11, BOX_B);

	auto boxC = Sprite::create("BoxC2.png");
	boxC->setPosition(center + Vec2(120, 120));
	this->addChild(boxC, 12, BOX_C);

	auto nextLabel = Label::createWithTTF("Next", "fonts/Marker Felt.ttf", 36);
	auto menuItem = MenuItemLabel::create(nextLabel,
		CC_CALLBACK_1(HelloWorld::goCallback,this));
	auto menu = Menu::create(menuItem, NULL);
	menu->setColor(Color3B(1, 0, 0));
	menu->setPosition(Vec2(100, visibleSize.height - 100));
	this->addChild(menu);
	return true;
}
/**
 * һ�����⣺��ȻҪ���ñ�ǩ����Ϊʲô������һ���������ó����Ա����
 */
void  HelloWorld::onEnter() {
	Scene::onEnter();
	//�¼��ַ���
	EventDispatcher* disp = Director::getInstance()->getEventDispatcher();
	/************************************************************************************************/
	//���㴥��������
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTEnded, this);	
	//ע�������
	disp->addEventListenerWithSceneGraphPriority(listener, this->getChildByTag(BOX_A));
	disp->addEventListenerWithSceneGraphPriority(listener->clone(), this->getChildByTag(BOX_B));
	disp->addEventListenerWithSceneGraphPriority(listener->clone(), this->getChildByTag(BOX_C));
	/************************************************************************************************/
	//�����¼�������
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyPressed, this);
	//ʹ������������lambda���ʽ
	keyListener->onKeyReleased = [](EventKeyboard::KeyCode code, Event * event)
	{
		log("keyReleased keycode = %u", code);
	};
	//ע�������
	disp->addEventListenerWithSceneGraphPriority(keyListener, this);
	/************************************************************************************************/
	//����¼�������
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = [](Event* event) {
		log("Mouse Up");
	};
	mouseListener->onMouseDown = [](Event* event) {
		log("Mouse Down");
	};
	mouseListener->onMouseScroll = [](Event* event) {
		log("Mouse Scroll");
	};
	mouseListener->onMouseMove = [](Event* event) {
		log("Mouse Move");
	};
	//ע�������
	disp->addEventListenerWithSceneGraphPriority(mouseListener, this);
	/************************************************************************************************/
	//���ٶ��¼�������
	auto accListener = EventListenerAcceleration::create([](Acceleration* acc, Event* event) {
		log("onAcceleration x=%f,y=%f,z=%f", acc->x, acc->y, acc->z);
	});
	disp->addEventListenerWithSceneGraphPriority(accListener, this);

}
void  HelloWorld::onExit() {

	Scene::onExit();//����Ϊʲô���Ƿ������
	EventDispatcher* disp = Director::getInstance()->getEventDispatcher();
	disp->removeEventListenersForTarget(this);//�Ƴ��ó����е����м�����
}

bool HelloWorld::onTBegan(Touch* touch, Event* event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 tworld = touch->getLocation();//��ȡ�������opengl��������
	Vec2 tlocal = target->convertToNodeSpace(tworld);

	auto size = target->getContentSize();
	auto rect = Rect(0, 0, size.width, size.height);

	if (rect.containsPoint(tlocal)) {
		target->runAction(ScaleBy::create(0.1f, 1.2f));
		return true;
	}
	return false;
}
void HelloWorld::onTMoved(Touch* touch, Event* event) {
	log("onTouchMoved");
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	target->setPosition(target->getPosition() + touch->getDelta());
}
void HelloWorld::onTEnded(Touch* touch, Event* event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 tworld = touch->getLocation();//��ȡ�������opengl��������
	Vec2 tlocal = target->convertToNodeSpace(tworld);

	auto size = target->getContentSize();
	auto rect = Rect(0, 0, size.width, size.height);

	if (rect.containsPoint(tlocal)) {
		target->runAction(ScaleTo::create(0.1f, 1.00f));
	}
}

void HelloWorld::keyPressed(EventKeyboard::KeyCode code, Event * event)
{
	log("keyPressed keycode = %d", code);
}

void HelloWorld::goCallback(Ref* pSender)
{
	auto sc = Second::createScene();
	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}

