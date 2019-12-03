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
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
    auto layer =  HelloWorld::create();
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
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin =  Director::getInstance()->getVisibleOrigin();

	//��ȡ��Ƭ��ͼ
	myMap = TMXTiledMap::create("map/mymap.tmx");
	
	//��ȡ����
	TMXObjectGroup* objects = myMap->getObjectGroup("object");//ͨ�����������ö������󼯺�
	ValueMap player = objects->getObject("player");//ͨ����������ö���
	ValueMap originPoint = objects->getObject("originPoint");//ͨ����������ö���

	//���õ�ͼƫ��
	ox = originPoint["x"].asFloat();//��ö����x��
	oy = originPoint["y"].asFloat();//��ö����y�����꣬�Ѿ�ת������opengl���꣬���ö�������½���map�е�opengl����
	log("originPoint(%f,%f)", ox, oy);//256,1632
	myMap->setPosition(Vec2(-ox, -oy));
	//this->setPosition(ox, oy);//�ƶ��ӵ㣨Ĭ����ԭ�㣨0,0����
	//this->setViewPointCenter(Vec2(-ox, -oy));
	this->addChild(myMap);

	//����player����λ�����þ���λ��
	float x = player["x"].asFloat();//��ö����x��
	float y = player["y"].asFloat();//��ö����y��
	log("player(x=%f,y=%f)\n", x, y);
	playerSprite = Sprite::create("player.png");//�����з��þ���
	playerSprite->setPosition(Vec2(x-ox, y-oy));//�����λ�÷ŵ������λ��
	playerSprite->setAnchorPoint(Vec2(0, 0));//�����λ���������½ǣ����Ծ����ê��ҲӦ�����ó����½�
	playerSprite->setAnchorPoint(Vec2(0, 1));//Ϊʲô�����Ͻ���
	this->addChild(playerSprite);

	//��ȡ�ϰ���
	coLayer = myMap->getLayer("collider");

	this->setTouchEnabled(true);//�����㴥������
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* envent) {
	return true;
}
void   HelloWorld::onTouchMoved(Touch* touch, Event* envent) {
	//log("touch moved");
}
void   HelloWorld::onTouchEnded(Touch* touch, Event* envent) {
	Vec2 touchp = touch->getLocation();//��ȡ�������opengl����
	Vec2 playerp = playerSprite->getPosition();//��������Ļ�ϵ�opengl����
	Vec2 diff = touchp - playerp;//�������ɫ֮��ľ����
	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerp.x += myMap->getTileSize().width;//ÿ��ֻ�ƶ�һ��
			playerSprite->runAction(FlipX::create(FALSE));
		}
		else {
			playerp.x -= myMap->getTileSize().width;
			playerSprite->runAction(FlipX::create(TRUE));
		}
	}
	else {
		if (diff.y > 0)
			playerp.y += myMap->getTileSize().height;
		else playerp.y -= myMap->getTileSize().height;
	}
	//playerSprite->setPosition(playerp.x, playerp.y);
	this->setPlayerPos(playerp);//�������������þ���λ��
}

Vec2 HelloWorld::tileCoordFromPosition(Vec2 pos)//posΪ�������opengl����
{
	//��ͼy����Ƭ��������ÿ����Ƭ�ĸ߶ȵõ���ͼ��y���ܸ߶�
	int mpy = myMap->getMapSize().height * myMap->getTileSize().height;
	int mpx = myMap->getMapSize().width * myMap->getTileSize().width;

	//ת�����ڵ�ͼ��opengl�������꣬��Ϊ��ͼ��ԭ�㲻����Ļԭ��
	Vec2 posInMap = Vec2(pos.x + ox, pos.y + oy);
	//ת�����ڵ�ͼ��UI�������꣨ԭ�������Ͻǣ�
	Vec2 posInMapUI = Vec2(pos.x + ox, mpy- (pos.y + oy));
	
	//ת������Ƭ����
	int x = posInMapUI.x / myMap->getTileSize().width;
	int y = posInMapUI.y / myMap->getTileSize().height;
	return Vec2(x,y);
}

void HelloWorld::setPlayerPos(Vec2 pos)//posΪ��������Ļ�ϵ�opengl����
{
	Vec2 tilePos = this->tileCoordFromPosition(pos);//ת������Ƭ�����Է����ж�
	int tileGid = coLayer->getTileGIDAt(tilePos);
	if (tileGid > 0) {
		Value prop = myMap->getPropertiesForGID(tileGid);//���gid������
		ValueMap propMap = prop.asValueMap();//ǿ������ת��
		if (propMap["Collidable"].asBool()) {
			SimpleAudioEngine::getInstance()->playEffect("empty.wav");
			return;
		}
	}
	playerSprite->setPosition(pos);
	this->setViewPointCenter(playerSprite->getPosition());
}

void HelloWorld::setViewPointCenter(Vec2 pos)//�������Ŀ���
{
	//��ͼy����Ƭ��������ÿ����Ƭ�ĸ߶ȵõ���ͼ��y���ܸ߶�
	int mpy = myMap->getMapSize().height * myMap->getTileSize().height;
	int mpx = myMap->getMapSize().width * myMap->getTileSize().width;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//x���겻��С����Ļ�����һ��
	int x = MAX(pos.x, visibleSize.width / 2);
	int y = MAX(pos.y, visibleSize.height / 2);
	x = MIN(x,mpx - visibleSize.width / 2);
	y = MIN(x, mpy - visibleSize.height / 2);

	Vec2 pointA = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	Vec2 pointB = Vec2(x, y);
	Vec2 offset = pointA - pointB;//�൱��AB����
	this->setPosition(offset);//���õ���Layer��λ��,�ƶ�����൱���ƶ���ͼ
}

