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

#include "PlaneSupport.h"

USING_NS_CC;

bool PlaneSupport::init()
{
	if (!Sprite::init())
		return false;
	return true;
}

void PlaneSupport::init_support(int index)
{
	String* name_str = String::createWithFormat("plane/support%d.png", index);
	this->setTexture(name_str->getCString());

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto content_size = this->getContentSize();

	//���ᳬ����Ļ
	auto pos_x = CCRANDOM_0_1()*(visibleSize.width - content_size.width) + content_size.width/2;
	auto pos_y = CCRANDOM_0_1()*(visibleSize.height - content_size.height) + content_size.height/2;
	auto point = Vec2(pos_x, pos_y);
	this->setPosition(point);

	//��������������
	ccBezierConfig bezier;
	//���p1����
	pos_x = CCRANDOM_0_1()*(visibleSize.width - content_size.width) + content_size.width / 2;
	pos_y = CCRANDOM_0_1()*(visibleSize.height - content_size.height) + content_size.height / 2;
	bezier.controlPoint_1 = Vec2(pos_x, pos_y);
	//���p2����
	pos_x = CCRANDOM_0_1()*(visibleSize.width - content_size.width) + content_size.width / 2;
	pos_y = CCRANDOM_0_1()*(visibleSize.height - content_size.height) + content_size.height / 2;
	bezier.controlPoint_2 = Vec2(pos_x, pos_y);
	//p3����,����Բ����������ߵ���Ļ�����棨�·���
	pos_x = CCRANDOM_0_1()*(visibleSize.width - content_size.width) + content_size.width / 2;
	bezier.endPosition = Vec2(pos_x, -content_size.height);

	auto bezieraction = BezierTo::create(2.0, bezier);
	auto remover_action = RemoveSelf::create();//�Ƴ��Լ�
	auto ac = Sequence::create(bezieraction, remover_action, NULL);
	this->runAction(ac);	
}
