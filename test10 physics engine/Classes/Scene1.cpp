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

#include "Scene1.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Scene1::createScene()
{
	//auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	auto layer = Scene1::create();
	//������������ĵ���debud,���Կ�����ɫ�ı�Ե�����Թ���
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
bool Scene1::init()
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
	
	auto dic = Dictionary::createWithContentsOfFile("fonts/test.xml");
	auto str_en = String::createWithFormat("ch%d", 1);
	auto str_ch = (String*)(dic->objectForKey(str_en->getCString()));
	auto label = Label::create();
	label->setString(str_ch->getCString());
	label->setSystemFontSize(40);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	this->addChild(label);
	//���ز˵�
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(40);
	auto backItem = MenuItemFont::create("Back",CC_CALLBACK_1(Scene1::backMenuCallback,this));
	auto mn = Menu::create(backItem, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(visibleSize.width - 70, 40));
	this->addChild(mn);

	//��������Ҳ��һ�����壬��һ�������Ǵ�С���ڶ��������ǲ��ʣ��������Ǻ�ȣ�Ĭ��Ϊ1.0f
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0.5);
	auto edgeNode = Node::create();//����һ����Ϸ�еĽڵ�
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//ͨ�������������������״����Ϸ�еĽڵ����������ͬ������
	//����Ҫʹ��update��Ϸѭ�������ýǶȺ��ٶȵȣ���Ϊʹ���˵ײ��װ����������API
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);


	return true;
}

bool Scene1::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 p2 = touch->getLocation();//��ȡ�������opengl����
	this->addNewSpriteAtPos(p2);
	return true;
}

void Scene1::addNewSpriteAtPos(Vec2 pos)
{
	auto sp = Sprite::create("Ball.png");
	//С������������е�body����С��С���ĳ������Ĵ�С��һ��
	//Ϊ�����������-1��ȷ��body�ŵ�sp�С�
	auto body = PhysicsBody::createCircle(sp->getContentSize().height / 2 - 1);
	sp->setPhysicsBody(body);
	sp->setPosition(pos);
	this->addChild(sp);
}

void Scene1::nextMenuCallback(Ref* pSender){
	
}

void Scene1::backMenuCallback(Ref * pSender)
{
	Director::getInstance()->popScene();
}




