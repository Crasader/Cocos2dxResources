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

#include "MainGame.h"
#include "StartGame.h"
#include "SimpleAudioEngine.h"
#include "EnemyBase.h"
#include "Bullet.h"
#include "ManagerBase.h"
#include "PlaneSupport.h"
#include "SuspandLayer.h"
USING_NS_CC;

Scene* MainGame::createScene()
{
    //return MainGame::create();
	auto scene = Scene::create();
	auto layer = MainGame::create();
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainGameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainGame::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	plane_support_ = NULL;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//����1
	spr_bg1_ = Sprite::create("ui/bgs.png");
	spr_bg1_->setAnchorPoint(Vec2(0, 0));
	spr_bg1_->setPosition(Vec2(0, 0));
	this->addChild(spr_bg1_);

	//����2
	spr_bg2_ = Sprite::create("ui/bgs.png");
	spr_bg2_->setAnchorPoint(Vec2(0, 0));
	spr_bg2_->setPosition(Vec2(0, spr_bg1_->getContentSize().height));
	this->addChild(spr_bg2_);

	scheduleUpdate();
	//��ʼ���ɻ�
	init_hero_plane(1);

	//�����ͣ*********************************************************************/
	//���menu
	auto dictionnary = Dictionary::createWithContentsOfFile("fonts/text.xml");
	auto btn_label = Label::create();
	btn_label->setString(((String*)(dictionnary->objectForKey("suspend")))->getCString());
	btn_label->setSystemFontSize(30);
	auto start_menu = MenuItemLabel::create(btn_label, CC_CALLBACK_1(MainGame::suspend, this));

	auto menu = Menu::create(start_menu, NULL);
	menu->setPosition(Vec2(visibleSize.width - btn_label->getContentSize().width / 2-5,
		visibleSize.height - btn_label->getContentSize().height / 2-5));
	this->addChild(menu);

	//�����ͣ*********************************************************************/
	//���㴥��������
	auto m_touchListener = EventListenerTouchOneByOne::create();
	m_touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::onTouchBegan, this);
	m_touchListener->onTouchMoved = CC_CALLBACK_2(MainGame::onTouchMoved, this);
	//��������
	m_touchListener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(m_touchListener, hero_player_);
    return true;
}
void MainGame::onEnterTransitionDidFinish(){
	Layer::onEnterTransitionDidFinish();
	scheduleUpdate();

	//�л�
	this->schedule(SEL_SCHEDULE(&MainGame::add_enemy), 0.3f);
	//�ӵ�
	this->schedule(SEL_SCHEDULE(&MainGame::add_bullet), 0.09f);
	//��ײ���
	this->schedule(SEL_SCHEDULE(&MainGame::is_crash));
	//����
	this->schedule(SEL_SCHEDULE(&MainGame::add_support),9.0f);
}
void MainGame::hero_death()
{
	hero_player_->stopAllActions();
	this->unschedule(SEL_SCHEDULE(&MainGame::add_bullet));
	this->unschedule(SEL_SCHEDULE(&MainGame::is_crash));
	this->unschedule(SEL_SCHEDULE(&MainGame::add_support_bullet));

	auto animation = Animation::create();
	for (int i = 1; i <= 4; i++) {
		auto str_name = String::createWithFormat("plane/b%d.png", i);
		animation->addSpriteFrameWithFile(str_name->getCString());
	}
	animation->setDelayPerUnit(0.2f);
	animation->setLoops(1);

	//hero_player_->runAction(Animate::create(animation));
	hero_player_->runAction(Sequence::create(Animate::create(animation),
		CallFunc::create(CC_CALLBACK_0(MainGame::hero_player_action_end, this)),NULL));
}
void MainGame::hero_player_action_end()
{
	this->unschedule(SEL_SCHEDULE(&MainGame::add_enemy));
	hero_player_->removeFromParentAndCleanup(true);//�ӳ������Ƴ���û�и��������ͻᱻ���뵽������ճ�ɾ��
	//�ص��������
	Director::getInstance()->replaceScene(TransitionFadeTR::create(0.5, StartGame::createScene()));
}
void MainGame::onExit() {
	unscheduleUpdate();
	Layer::onExit();
}
void MainGame::update(float delta) {
	//ʵ�ֱ�������,��������ͼ�������
	float y1 = spr_bg1_->getPositionY() - 3.0f;//�����ƶ�
	float y2 = spr_bg2_->getPositionY() - 3.0f;

	spr_bg1_->setPositionY(y1);
	spr_bg2_->setPositionY(y2);

	//�жϱ���ͼƬ�Ƿ񳬳��߽�
	if (y1 < -spr_bg1_->getContentSize().height) {
		spr_bg1_->setPositionY(spr_bg2_->getPositionY() + spr_bg2_->getContentSize().height -2);
	}else if(y2 < -spr_bg2_->getContentSize().height) {
		spr_bg2_->setPositionY(spr_bg1_->getPositionY() + spr_bg1_->getContentSize().height -2);
	}

}

void MainGame::init_hero_plane(int index) {
	//��Ӷ���
	auto animation = Animation::create();
	for (int i = 0; i < 2; i++) {
		auto str_name = String::createWithFormat("plane/heros%d_%d.png", index, i + 1);
		animation->addSpriteFrameWithFile(str_name->getCString());
	}
	animation->setDelayPerUnit(0.5);
	animation->setLoops(-1);
	//�����ɻ�����
	hero_player_ = Sprite::create(String::createWithFormat("plane/heros%d_1.png", index)->getCString());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	hero_player_->setPosition(Vec2(visibleSize.width / 2, hero_player_->getContentSize().height/2));
	this->addChild(hero_player_);
	hero_player_->runAction(Animate::create(animation));
}

bool MainGame::onTouchBegan(Touch *touch, Event *unused_event) {
	log("onTouchBegan");
	//�жϵ���������Ƿ��ڷɻ���
	auto point = touch->getLocation();
	auto rect = hero_player_->getBoundingBox();
	if (rect.containsPoint(point)) {
		return true;
	}
	return false;
}
void MainGame::onTouchMoved(Touch *touch, Event *unused_event) {
	log("onTouchMoved");
	//��ȡ�ɻ��ƶ��ľ���
	auto add_point = touch->getLocation() - touch->getPreviousLocation();
	hero_player_->setPosition(hero_player_->getPosition() + add_point);
	//�µĻ�ȡ�ƶ�����ķ���,�������ô�ƶ�����
	//auto add_point = touch->getDelta();
	//�жϷɻ�һ���ľ��벻Ҫ�����߽�
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto hero_size = hero_player_->getContentSize();

	//x����
	if (hero_player_->getPositionX() < hero_size.width / 2) {
		hero_player_->setPositionX(hero_size.width / 2);
	}
	else if (hero_player_->getPositionX() > visibleSize.width -  hero_size.width / 2) {
		hero_player_->setPositionX(visibleSize.width - hero_size.width / 2);
	}
	//y����
	if (hero_player_->getPositionY() < hero_size.height / 2) {
		hero_player_->setPositionY(hero_size.height / 2);
	}
	else if (hero_player_->getPositionY() > visibleSize.height - hero_size.height / 2) {
		hero_player_->setPositionY(visibleSize.height - hero_size.height / 2);
	}
}

void MainGame::suspend(Ref * ref)
{
	unscheduleUpdate();
	//�Ȱ�֮ǰ���õ�����ɾ��
	CCTextureCache::getInstance()->removeUnusedTextures();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto texture = RenderTexture::create(visibleSize.width, visibleSize.height);
	//����---------------------
	texture->begin();
	//��Ⱦ
	Director::getInstance()->getRunningScene()->visit();
	texture->end();
	texture->saveToFile("suspand.png", Image::Format::PNG);
	//-------------------------
	//ʹ��schedule����һ֡����callback����
	//����ʱ����һ֡
	auto back = [](float dt) {
		//�л�����ͣ����
		auto scene = Scene::create();
		auto layer = SuspandLayer::create();
		scene->addChild(layer);
		Director::getInstance()->pushScene(scene);
	};
	//���ﴴ��layer����Ϊ��ֲandroidʱ���ܵ������߳��н������һ֡
	auto layer = Layer::create();
	this->addChild(layer);
	auto schedules = layer->getScheduler();
	schedules->schedule(back,layer, 0.0f, 0, 0.0f, false, "screenshot");
}

void MainGame::add_enemy(float delta) {
	//��ӵл�
	int enemy_x = CCRANDOM_0_1() * 9 + 1;//1~10
	if (enemy_x > 6) {//7~10,40%
		enemy_x = 2;
	}else {//1~6
		enemy_x = 1;
	}
	auto enemy = EnemyBase::create(); 
	enemy->initEnemy(enemy_x);
	this->addChild(enemy);
	//���л����������
	ManagerBase::getInstance()->set_enemy_list(enemy);
	log("add_enemy");//�������������û�ел�����
}
void  MainGame::add_bullet(float tm) {
	auto bullet = Bullet::create();
	bullet->initBullet("bullet1.png");

	auto point = Vec2(hero_player_->getPositionX(),
		hero_player_->getPositionY()+hero_player_->getContentSize().height/2 +10);
	bullet->setPosition(point);

	this->addChild(bullet);
	//���ӵ����������
	ManagerBase::getInstance()->set_bullet_list(bullet);
	//log("���ӵ������������");
	log("add bullet");
}

void MainGame::add_support(float tm)
{
	//��ʼ������
	plane_support_ = PlaneSupport::create();
	//�����֧Ԯ���ʵ�����
	int index = CCRANDOM_0_1() * 2 + 1;
	support_index_ = index;
	plane_support_->init_support(index);
	this->addChild(plane_support_);
	plane_support_->setTag(index);
}

void MainGame::add_support_bullet(float tm)
{
	//�Ե����ʺ���ӵ�
	auto bullet = Bullet::create();
	if (support_index_ == 2)
		bullet->initBullet("bullet_suspand2.png");
	else bullet->initBullet("bullet_suspand1.png");

	//����֧Ԯ���ʵ���ҷɻ������󣬻��жϣ���Ϊ��ҷɻ�������û�йر�add_support_bullet
	auto point = Vec2(hero_player_->getPositionX(),
		hero_player_->getPositionY() + hero_player_->getContentSize().height);
	bullet->setPosition(point);
	this->addChild(bullet);
	ManagerBase::getInstance()->set_bullet_list(bullet);
}

void MainGame::is_crash(float tm)
{
	auto & enemy_list = ManagerBase::getInstance()->getEnemyList();
	auto & bullet_list = ManagerBase::getInstance()->getBulletList();

	//��ҷɻ��ӵ��͵л�����ײ���
	//�������,�����ɵĵл����ӵ���������ײ
	for (int i = enemy_list.size() - 1; i >= 0; --i) {
		auto enemy = enemy_list.at(i);
		//��������ֵ��Ϊ0����ô���������list�У���Ϊ����������ֵΪ0ʱ��û�б��Ƴ�
		if (enemy->get_hp() <= 0) {
			continue;
		}
		auto enemy_size = enemy->boundingBox();
		//�ӵ�����
		for (int i = bullet_list.size() - 1; i >= 0; --i) {
			auto bullet = bullet_list.at(i);
			if (enemy_size.containsPoint(bullet->getPosition())) {
				enemy->set_hp(enemy->get_hp() - 1);
				//�Ƴ��ӵ�
				bullet->removeFromParentAndCleanup(true);//��MainGame�������Ƴ�
				ManagerBase::getInstance()->remove_bullet_list(bullet);//��list���Ƴ����´ξͲ��ᱻ�����

				//����л�Ѫ��Ϊ0���ű�ը��Ϊʲô����list���Ƴ�
				if (enemy->get_hp() <= 0) {
					enemy->enemy_death();
				}
			}
		}
	}

	//��ҷɻ��͵л�����ײ���
	for (auto i = enemy_list.size() - 1; i >= 0; --i) {
		auto enemy = enemy_list.at(i);
		if (enemy->get_hp() <= 0) {
			continue;
		}
		auto rect = this->hero_player_->getBoundingBox();
		auto point = enemy->getPosition();
		if (rect.containsPoint(point)) {
			hero_death();
		}
	}

	//��������ҷɻ�����ײ
	if (plane_support_ != NULL) {
		auto rect = hero_player_->getBoundingBox();
		auto point = plane_support_->getPosition();//֧Ԯ�����߳���Ļ����������
		if (rect.containsPoint(point)) {
			//���֧Ԯ���ʵ��ӵ�
			this->schedule(SEL_SCHEDULE(&MainGame::add_support_bullet), 0.1f,50,0);//50�����������������������ü���
			//ֹͣ��ͨ�ӵ�
			this->unschedule(SEL_SCHEDULE(&MainGame::add_bullet));
			//�����ӳ�ִ�������ͨ�ӵ�ʱ��
			this->schedule(SEL_SCHEDULE(&MainGame::add_bullet),0.1f,-1,5.0f);//5.0f�����������������������ݶ�ú󱻵���
			plane_support_->setVisible(false);
			plane_support_ = NULL;
		}
		//����������˳���2��ô���ӵ��Ƴ���Ļʱ��������һС��ʱ����û���Ƴ��ģ�����ٵ�����getPosition�����ͻ����
		if (plane_support_ != NULL && point.y < -plane_support_->getContentSize().height/2) {
			plane_support_->removeFromParent();
			plane_support_ = NULL;
		}
	}
	
}


