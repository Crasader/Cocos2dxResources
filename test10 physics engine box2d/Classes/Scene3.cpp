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

#include "Scene3.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Scene3::createScene()
{
	auto scene = Scene::create();
	auto layer = Scene3::create();
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
bool Scene3::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	setTouchEnabled(true);//�����㴥���¼�
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);//���㴥��

												  //���ı����ǩ
	auto dic = Dictionary::createWithContentsOfFile("fonts/test.xml");
	auto str_en = String::createWithFormat("ch%d", 3);
	auto str_ch = (String*)(dic->objectForKey(str_en->getCString()));
	auto label = Label::create();
	label->setString(str_ch->getCString());
	label->setSystemFontSize(40);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	this->addChild(label);

	//���ز˵�
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(40);
	auto backItem = MenuItemFont::create("Back", CC_CALLBACK_1(Scene3::backMenuCallback, this));
	auto mn = Menu::create(backItem, NULL);
	mn->alignItemsVertically();
	mn->setPosition(Vec2(visibleSize.width - 70, 40));
	this->addChild(mn);

	initPhysics();
	scheduleUpdate();

	return true;
}
//��Ϸѭ��������̶�
void Scene3::update(float dt)
{
	float timestep = 0.03f;//����dt(���ȶ�)��������һ������
	int32 velocityIterations = 8;//�ٶȵĵ���
	int32 positionIterations = 1;//λ�Ƶĵ���

	world->Step(timestep, velocityIterations, positionIterations);
	//�����������е���������ȡ����
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != nullptr) {
			auto sprite = (Sprite*)b->GetUserData();
			//�����뾫��λ��ͬ��
			sprite->setPosition(Vec2(b->GetPosition().x*
				PTM_RATIO, b->GetPosition().y*PTM_RATIO));
			//�����뾫�����תͬ��������������������������ת�������෴�ġ�
			//����ת�ɽǶ�
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

void Scene3::initPhysics()
{
	Size s = Director::getInstance()->getVisibleSize();
	b2Vec2 gravity;//��������
	gravity.Set(0.0f, -10.0f);

	world = new b2World(gravity);//������������
								 //�����������ߣ����徲ֹ״̬ʱ�����ߣ���Ҫ��ײ����ʱ
								 //�Ż��ѣ��Ա���ٻ��ƵĴ���
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);//���������������


	b2BodyDef gBodyDef;//ָ����������߽�
	gBodyDef.position.Set(0, 0);//������������ṹ��
	b2Body* gBody = world->CreateBody(&gBodyDef);//������������
	b2EdgeShape gBox;//������бߵ���״

					 //����ײ�����������Ϊ�ף������Ҫ����ת��
	gBox.Set(b2Vec2(0, 0), b2Vec2(s.width / PTM_RATIO, 0));
	//ʹ�ü� �߹̶���״�����壬����1����״������2���ܶȣ�
	//�±����迼����������������Ϊ0;
	gBody->CreateFixture(&gBox, 0);

	//�����
	gBox.Set(b2Vec2(0, s.height / PTM_RATIO), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	gBody->CreateFixture(&gBox, 0);
	//���
	gBox.Set(b2Vec2(0, 0), b2Vec2(0, s.height / PTM_RATIO));
	gBody->CreateFixture(&gBox, 0);
	//�ұ�
	gBox.Set(b2Vec2(s.width / PTM_RATIO, 0), b2Vec2(s.width / PTM_RATIO, s.height / PTM_RATIO));
	gBody->CreateFixture(&gBox, 0);
}

bool Scene3::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 p2 = touch->getLocation();//��ȡ�������opengl����
	this->addNewSpriteAtPos(p2);
	return true;
}

void Scene3::addNewSpriteAtPos(Vec2 pos)
{
	log("create sprite");
	//��������A
	auto spA = Sprite::create("boxA2.png");
	spA->setPosition(pos);
	this->addChild(spA);
	//��̬���嶨��A
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;//�ṹ������Ϊ��̬����
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y/PTM_RATIO);
	b2Body* bodyA = world->CreateBody(&bodyDef);
	bodyA->SetUserData(spA);//�󶨾���
	//��������B
	auto spB = Sprite::create("boxB2.png");
	spB->setPosition(pos+Vec2(100,-100));
	this->addChild(spB);

	//��̬���嶨��B
	bodyDef.type = b2_dynamicBody;//�ṹ������Ϊ��̬����
	Vec2 posB = spB->getPosition();
	bodyDef.position.Set(posB.x / PTM_RATIO, posB.y/PTM_RATIO);
	b2Body* bodyB = world->CreateBody(&bodyDef);
	bodyB->SetUserData(spB);//�󶨾���


	//��״
	b2PolygonShape dynamicBox;//2m�ĺ���
	dynamicBox.SetAsBox(1, 1);//�߳���һ����1m

	//��̬�о߶���
	b2FixtureDef FixtureDef;
	FixtureDef.shape = &dynamicBox;//���üо���״
	FixtureDef.density = 1.0f;//�����ܶ�
	FixtureDef.friction = 0.3f;//Ħ��ϵ��

	//���о߹̶���������
	bodyA->CreateFixture(&FixtureDef);
	bodyB->CreateFixture(&FixtureDef);

	//����ؽڶ���
	b2DistanceJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), bodyB->GetWorldCenter());
	jointDef.collideConnected = true;

	bodyA->GetWorld()->CreateJoint(&jointDef);//bodyA,bodB������getWorld
}

void Scene3::nextMenuCallback(Ref* pSender) {

}

void Scene3::backMenuCallback(Ref * pSender)
{
	Director::getInstance()->popScene();
}

Scene3::~Scene3()
{
	//delete(world);//�������Ұָ��
	CC_SAFE_DELETE(world);
}




