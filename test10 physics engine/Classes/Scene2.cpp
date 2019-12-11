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

#include "Scene2.h"
#include "SimpleAudioEngine.h"
#define TAG 100
USING_NS_CC;

Scene* Scene2::createScene()
{
	//auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	auto layer = Scene2::create();
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
bool Scene2::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin =  Director::getInstance()->getVisibleOrigin();

	setTouchEnabled(true);//�����㴥���¼�
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);//���㴥��

	//���ز˵�
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(40);
	auto backItem = MenuItemFont::create("Back",CC_CALLBACK_1(Scene2::backMenuCallback,this));
	auto mn = Menu::create(backItem, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(visibleSize.width - 70, 40));
	this->addChild(mn);
	//�����ǩ
	auto dic = Dictionary::createWithContentsOfFile("fonts/test.xml");
	auto str_en = String::createWithFormat("ch%d", 2);
	auto str_ch = (String*)(dic->objectForKey(str_en->getCString()));
	auto label = Label::create();
	label->setString(str_ch->getCString());
	label->setSystemFontSize(40);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	this->addChild(label);

	//��������Ҳ��һ�����壬��һ�������Ǵ�С���ڶ��������ǲ��ʣ��������Ǻ�ȣ�Ĭ��Ϊ1.0f
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0.5);
	auto edgeNode = Node::create();//����һ����Ϸ�еĽڵ�
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//ͨ�������������������״����Ϸ�еĽڵ����������ͬ������
	//����Ҫʹ��update��Ϸѭ�������ýǶȺ��ٶȵȣ���Ϊʹ���˵ײ��װ����������API
	edgeNode->setPhysicsBody(body);
	//edgeNode->setTag(-1);
	this->addChild(edgeNode);

	return true;
}

void Scene2::onEnter()
{
	Layer::onEnter();
	lis = EventListenerPhysicsContact::create();
	lis->onContactBegin = [](PhysicsContact& contact) {
		log("onContactBegin");
		auto spA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto spB = (Sprite*)contact.getShapeB()->getBody()->getNode();
		//if (spA && spA->getTag() == TAG && spB && spB->getTag == TAG) {
		//if (spA&&spA->getTag() >= 0 && spB&&spB->getTag() >= 0) {
		if (spA && spB) {//ֻҪ���������巢������ײ��tag��ֵ��+1��tagֵ����0������ɫ������0���ǰ�ɫ
			spA->setTag(spA->getTag() + 1);
			spB->setTag(spB->getTag() + 1);
			if(spA->getTag()>0)spA->setColor(Color3B::GREEN);
			if (spA->getTag()>0)spB->setColor(Color3B::GREEN);
		}
		return true;
	};
	lis->onContactPreSolve = [](PhysicsContact& contact, PhysicsContactPreSolve& solve) {
		log("onContactPreSolve");
		return true;
	};
	//ע�⣬�˺����޷���ֵ
	lis->onContactPostSolve = [](PhysicsContact& contact,const PhysicsContactPostSolve& solve) {
		log("onContactPostSolve");
	};
	lis->onContactSeparate = [](PhysicsContact& contact) {
		log("onContactSeparate");
		auto spA = (Sprite*)contact.getShapeA()->getBody()->getNode();
		auto spB = (Sprite*)contact.getShapeB()->getBody()->getNode();
		//if (spA&&spA->getTag() == TAG && spB&&spB->getTag == TAG) {
		//if (spA&&spA->getTag()>=0 && spB&&spB->getTag() >= 0) {
		if (spA && spB){
			spA->setTag(spA->getTag() - 1);
			spB->setTag(spB->getTag() - 1);
			if (spA->getTag()<=0)spA->setColor(Color3B::WHITE);
			if (spB->getTag()<=0)spB->setColor(Color3B::WHITE);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(lis,1);
}

void Scene2::onExit()
{
	Layer::onExit();
	Director::getInstance()->getEventDispatcher()->removeEventListener(lis);
}

bool Scene2::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 p2 = touch->getLocation();//��ȡ�������opengl����
	this->addNewSpriteAtPos(p2);
	return true;
}

void Scene2::addNewSpriteAtPos(Vec2 pos)
{
	auto sp = Sprite::create("BoxA2.png");
	sp->setTag(0);//�ѱ�ǩ�Ž�ȥΪ���ж��Ƿ��Ǹ�������ײ??����û�й�ϵ
	auto body = PhysicsBody::createBox(sp->getContentSize());

	//������ײ����������Bitmask����
	//�Ƿ������ײ���Ҫ��Bitmask���룬������С�λ�롱���� ���������Ƿ���
	//���������������ײ����������ײ��Ĭ��ֵ��0x1
	body->setContactTestBitmask(0x1);//a,b������ͬһ�֣�����λ������Ϊ��

	sp->setPhysicsBody(body);
	sp->setPosition(pos);
	this->addChild(sp);
}

void Scene2::nextMenuCallback(Ref* pSender){
	
}

void Scene2::backMenuCallback(Ref * pSender)
{
	Director::getInstance()->popScene();
}




