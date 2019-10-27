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

#include "ActionSpeedControlTestScene.h"
#include "ActionSpeedControlTest2Scene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* ActionSpeedControlTest::createScene()
{
    return ActionSpeedControlTest::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in ActionSpeedControlTestScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ActionSpeedControlTest::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//����
	auto bg = Sprite::create("Background (2).png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);
	//���ز˵�
	MenuItemImage * backMenuItem = MenuItemImage::create(
		"Back-up (2).png",
		"Back-down (2).png",
		CC_CALLBACK_1(ActionSpeedControlTest::goBackCallback, this));
	auto backMenu = Menu::create(backMenuItem, NULL);
	backMenu->setAnchorPoint(Vec2(0.5f, 0.5f));
	backMenu->setPosition(Vec2(100, visibleSize.height - 50));
	this->addChild(backMenu);

	//go�˵�
	Label* label[10];
	label[0] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseIn");
	label[1] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseOut");
	label[2] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseInOut");
	label[3] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseSineIn");
	label[4] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseSineOut");
	label[5] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseSineInOut");
	label[6] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseExponentialIn");
	label[7] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseExponentialOut");
	label[8] = Label::createWithBMFont("fonts/fnt2.fnt", "EaseExponentialInOut");
	label[9] = Label::createWithBMFont("fonts/fnt2.fnt", "Speed");


	MenuItemLabel* m[10];
	for (int i = 0; i < 10; i++) {
		m[i] = MenuItemLabel::create(label[i],
			CC_CALLBACK_1(ActionSpeedControlTest::goCallback, this));
		m[i]->setTag(((ActionSpeedControlType)(102 + i)));
	}

	auto mn = Menu::create(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8],
		m[9],  NULL);
	mn->alignItemsInColumns(2,2,2,2,2, NULL);
	this->addChild(mn);
	return true;
}


void ActionSpeedControlTest::goBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}
void ActionSpeedControlTest::goCallback(Ref* pSender)
{
	MenuItem *mitem = (MenuItem*)pSender;
	log("item tag = %d", mitem->getTag());

	auto sc = Scene::create();
	auto layer = ActionSpeedControlTest2::create();
	sc->addChild(layer);
	layer->setTag(mitem->getTag());

	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	//Director::getInstance()->replaceScene(reScene);//replace
	Director::getInstance()->pushScene(reScene);//push
}
