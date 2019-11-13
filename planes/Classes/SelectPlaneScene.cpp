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

#include "SelectPlaneScene.h"
#include "SelectPlaneLayer.h"
#include "SelectPlanePage.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SelectPlaneScene::createScene()
{
    return SelectPlaneScene::create();
}


// on "init" you need to initialize your instance
bool SelectPlaneScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto size = Director::getInstance()->getWinSize();
    
	//��ӿɻ�������
	SelectPlaneLayer* scrollView = SelectPlaneLayer::create();
	//�ؿ�1-3
	for (int i = 1; i <= 3; i++) {
		auto page = SelectPlanePage::create(i);
		page->setTag(i);
		scrollView->addNode(page);
	}
	auto spritebg = Sprite::create("ui/bg_select.png");
	spritebg->setPosition(Vec2(size.width/2,size.height/2));
	addChild(spritebg);


	this->addChild(scrollView);
    return true;
}



