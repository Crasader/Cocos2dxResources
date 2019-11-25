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

#include "Scene4.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Scene4::createScene()
{
    return Scene4::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Scene4Scene.cpp\n");
}

// on "init" you need to initialize your instance
bool Scene4::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//���ز˵�
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(40);
	MenuItemFont *item1 = MenuItemFont::create("back",
		CC_CALLBACK_1(Scene4::menuItem1Callback, this));
	auto menu = Menu::create(item1, NULL);
	menu->setPosition(Vec2(visibleSize.width - 50, 50));
	this->addChild(menu);

	//
	auto bg = Sprite::create("background-1.png");
	bg->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	//
	auto p = ParticleSystemQuad::createWithTotalParticles(200);
	p->setTexture(Director::getInstance()->getTextureCache()->addImage("snow.png"));

	//��������
	p->setDuration(-1);//���÷������ӵĳ���ʱ��-1��ʾ��Զ����
	p->setGravity(Point(0, -240));//�� �����ӵ���������ϵ 
	p->setAngle(90);//���ýǶ��Լ�ƫ��
	p->setAngleVar(360);
	p->setRadialAccel(50);//���þ�����ٶ��Լ�ƫ��
	p->setRadialAccelVar(0);
	p->setTangentialAccel(30);//�������ӵ�������ٶ��Լ�ƫ��
	p->setTangentialAccelVar (0) ;
	p->setPosVar(Point(400, 0));// �������ӳ�ʼ��λ��ƫ��
	p->setLife (4) ;//���������������Լ�ƫ��
	p->setLifeVar (2);
	p->setStartSpin(30);//�������ӿ�ʼʱ����ת�Ƕ��Լ�ƫ��
	p->setStartSpinVar (60) ;
	p->setEndSpin(60);//���ý���ʱ�����ת�Ƕ��Լ�ƫ��
	p->setEndSpinVar(60) ;
	p->setStartColor(Color4F(1, 1, 1, 1));//���ÿ�ʼʱ �����ɫ�Լ�ƫ��
	p->setEndColor(Color4F(1, 1, 1, 1));//���ý� ��ʱ�����ɫ�Լ�ƫ��	
	p->setStartSize (30) ;//���ÿ�ʼʱ�����Ӵ�С�Լ�ƫ��
	p->setStartSizeVar(0) ;
	p->setEndSize(20.0f);//�������ӽ���ʱ���С�Լ�ƫ��
	p->setEndSizeVar(0);
	p->setEmissionRate(100);//����ÿ���Ӳ������ӵ�����

	p->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(p);
	return true;
}
void Scene4::menuItem1Callback(Ref* pSender) {
	Director::getInstance()->popScene();
}

