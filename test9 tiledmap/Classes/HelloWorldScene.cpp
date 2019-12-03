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
	//��ö����ڵ�ͼ�ϵ�opengl����
	//���ö�������½���map�е�opengl����
	float ox = originPoint["x"].asFloat();
	float oy = originPoint["y"].asFloat();
	//log("originPoint(%f,%f)", ox, oy);
	//�ƶ��㣬��������û�б�ģ����ϵĵ�ͼ�;��������ƶ�
	this->setPosition(-ox, -oy);
	this->addChild(myMap);

	//����player����λ�����þ���λ��
	float x = player["x"].asFloat();//��ö����x������
	float y = player["y"].asFloat();//��ö����y������
	//log("player(x=%f,y=%f)\n", x, y);
	playerSprite = Sprite::create("player.png");//�����з��þ���
	playerSprite->setPosition(Vec2(x, y));//�����λ�÷ŵ������λ��
	//�����λ���������½ǣ����Ծ����ê��ҲӦ�����ó����½�
	playerSprite->setAnchorPoint(Vec2(0, 0));
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
	Vec2 touchp = touch->getLocation();//��ȡ�������opengl��Ļ����
	Vec2 touchInLayer = touchp - this->getPosition();//�������ڲ��ϵ�opengl����
	//log("touch pos(%f,%f)", touchp.x, touchp.y);
	Vec2 playerp = playerSprite->getPosition();//�����ڲ��ϵ�opengl����
	//log("player pos(%f,%f)", playerp.x, playerp.y);
	Vec2 diff = touchInLayer - playerp;//�������ɫ֮��ľ����
	offset = Vec2(0, 0);//ÿ���ƶ���ƫ��ֵ�����û���ƶ�����0
	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			log("go right");
			offset.x = myMap->getTileSize().width;//ÿ��ֻ�ƶ�һ��
			playerSprite->runAction(FlipX::create(FALSE));
		}
		else {
			log("go left");
			offset.x = -myMap->getTileSize().width;
			playerSprite->runAction(FlipX::create(TRUE));
		}
	}
	else {
		if (diff.y > 0) {
			offset.y = myMap->getTileSize().height;
			log("go up");
		}
		else {
			offset.y = -myMap->getTileSize().height;
			log("go down");
		}
	}
	//�������������þ����ڲ��е�λ�ã�����ײ��⣩
	this->setPlayerPos(playerp+offset);
}
//�������ڲ��ϵ�opengl��������ת������Ƭ����
//posΪ�������ڲ��ϵ�opengl���꣬��ԭ��Ϊ���ԭ��
Vec2 HelloWorld::tileCoordFromPosition(Vec2 pos)
{
	//��ͼy����Ƭ��������ÿ����Ƭ�ĸ߶ȵõ���ͼ��y���ܸ߶�
	int mpy = myMap->getMapSize().height * myMap->getTileSize().height;

	//ת�����ڵ�ͼ��opengl�������ꡣ���ڲ��ԭ��Ҳ�ǵ�ͼ��openglԭ�㡣
	Vec2 posInMap = Vec2(pos.x, pos.y);
	//ת�����ڵ�ͼ��UI�������꣨ԭ�������Ͻǣ�
	Vec2 posInMapUI = Vec2(pos.x, mpy-pos.y );
	
	//ת������Ƭ����
	int x = posInMapUI.x / myMap->getTileSize().width;
	int y = posInMapUI.y / myMap->getTileSize().height;
	return Vec2(x,y);
}

//��ײ���
void HelloWorld::setPlayerPos(Vec2 pos)//posΪ�����ڲ��ϵ�opengl����,
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
	this->setViewPointCenter(pos);//���������ƶ���Ż��ƶ���
}
//�ƶ�Layer��֤player����λ����Ļ����
void HelloWorld::setViewPointCenter(Vec2 pos)//pos�Ǿ����ڲ��ϵ�opengl����
{
	//��ͼy����Ƭ��������ÿ����Ƭ�ĸ߶ȵõ���ͼ��y���ܸ߶�
	int mpy = myMap->getMapSize().height * myMap->getTileSize().height;
	int mpx = myMap->getMapSize().width * myMap->getTileSize().width;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 layerNextPos = this->getPosition() - offset;//����ƶ�����;�����ƶ������෴
	
	//��ֹ���ֺڱ�
	if (layerNextPos.x > 0)//������ƫ�ƣ���ô��߾ͻ��кڱ�
		layerNextPos.x =0;
	if (layerNextPos.x < -mpx + visibleSize.width)//������ƫ�ƣ�Ҫ����һ������Ļ���
		layerNextPos.x = -mpx + visibleSize.width;
	if (layerNextPos.y > 0)//������ƫ�ƣ���ô����ͻ��кڱ�
		layerNextPos.y = 0;
	if (layerNextPos.y < -mpy + visibleSize.height)//������ƫ�ƣ�Ҫ����һ������Ļ�߶�
		layerNextPos.y = -mpy + visibleSize.height;

	//����߽��Ҫ���ƶ�1/2��Ļ�ľ����ٹ�����Ļ
	float px = playerSprite->getPositionX();
	float py = playerSprite->getPositionY();
	if (px <= visibleSize.width / 2)
		layerNextPos.x = 0;
	if (px >= mpx - visibleSize.width / 2)
		layerNextPos.x = -mpx + visibleSize.width;
	if (py <= visibleSize.height / 2)
		layerNextPos.y = 0;
	if (py >= mpy - visibleSize.height / 2)
		layerNextPos.y = -mpy + visibleSize.height;

	this->setPosition(layerNextPos);//���õ���Layer��λ��,�ƶ�����൱���ƶ���ͼ
}

