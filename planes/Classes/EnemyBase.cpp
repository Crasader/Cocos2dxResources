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

#include "EnemyBase.h"
#include "ManagerBase.h"

USING_NS_CC;

bool EnemyBase::init()
{
	is_death_ = false;
	if (!Sprite::init())
		return false;
	return true;
}
void EnemyBase::onEnterTrnasitionDidFinish() {

}
 void EnemyBase::onExit() {
	 Sprite::onExit();
}

void EnemyBase::update(float delta) {
	//����ѱ�ը���ƶ�
	if (is_death_)
		return;
	float y = this->getPositionY() - 5;
	log("enemy positionY %f", y);
	this->setPositionY(y);
	if (y < -this->getContentSize().height) {//���߳�������Ƴ��ö���
		this->removeFromParentAndCleanup(true);
		remove_enemy();
	}
}
//7:44
void EnemyBase::initEnemy(int index /* = 1*/) {
	//�л�Ѫ��
	enemy_hp_ = index;
	//��ʼ���л�
	String* name_str= String::createWithFormat("plane/enemy%d.png", index);
	
	//auto cache = Director::getInstance()->getTextureCache()->addImage(name_str->getCString());
	//��ӵл�ͼƬ
	this->setTexture(name_str->getCString());

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 pos = Vec2(this->getContentSize().width/2+
		CCRANDOM_0_1()*(visibleSize.width - this->getContentSize().width),
		this->getContentSize().height*5
		+CCRANDOM_0_1()*(visibleSize.height - this->getContentSize().height*5));
	//Vec2 pos = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	this->setPosition(pos);
	this->scheduleUpdate();
	log("initEnemy");
}
void EnemyBase::enemy_death() {
	is_death_ = true;

	auto animation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		auto enemy_name = String::createWithFormat("plane/b%d.png", i);
		animation->addSpriteFrameWithFile(enemy_name->getCString());
	}
	animation->setDelayPerUnit(0.2f);
	animation->setLoops(1);

	//this->runAction(Animate::create(animation));
	//��ÿһ֡������ʱ�����涼��ȥ������ճؼ����������ϵ���Ƿ�С�ڵ���1��
	//����Ǿ�delete���տռ��ˣ���֮ʲô������
	//removeself�൱���ǰ����������뵽������ճ�
	//this->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(),NULL));
	//���ú�����һ����һ���ģ�ʹ����CallFunc
	this->runAction(Sequence::create(Animate::create(animation), 
		CallFunc::create(CC_CALLBACK_0(EnemyBase::enemy_death_action_end, this)),NULL));	
}
void EnemyBase::enemy_death_action_end() {
	this->removeFromParentAndCleanup(true);//��MainGame�������Ƴ�
	remove_enemy();
}
void EnemyBase::remove_enemy()
{
	ManagerBase::getInstance()->remove_enemy_list(this);
}

int EnemyBase::get_hp()
{
	return enemy_hp_;
}

void EnemyBase::set_hp(int hp)
{
	enemy_hp_ = hp;
}
