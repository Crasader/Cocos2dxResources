#include "HelloWorldScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//1�Ӵ�ͼ��ֱ�Ӳü�
	//auto tree1 = Sprite::create("tree1.png", Rect(604, 38, 302, 295));
	//tree1->setPosition(Vec2(500, 200));
	//this->addChild(tree1, 1);
	//2���������вü�
	//һ��������һ��ͼƬ��ķѱȽ϶��IOʱ�䣬����һ�������ɾ�������һ����ͼ��һ����
	//�����ڴ棨����������)��Ȼ��ü�����,���ַ����ڴ��ķѶ�һЩ��
	//�Ѵ�ͼ���������ڴ��У�Ȼ�����������淴��ʹ�ã�������Ҫ�Ĳ���
	//�����ͼ��ʱ��һ����һ��loading�Ĺ��̡����ʱ�������logo����ɢע����
	//�ڴ˹����л�����һ�����̣߳����м��غ�Ԥ��������ͱȽ�����

	//��������֡����
	//auto cache = Director::getInstance()->getTextureCache()->addImage("tree1.png");
	////ָ������Ͳü�������������������
	//auto tree2 = Sprite::create();
	//tree2->setTexture(cache);
	//tree2->setTextureRect(Rect(73, 72, 182, 270));
	////����ʱֱ��������������ʼ�����������������Ƿ���ת����Ĭ��false����ת
	////auto tree2 = Sprite::createWithTexture(cache,Rect(73, 72, 182, 270),false);
	//tree2->setPosition(Vec2(500, 200));
	//this->addChild(tree2,1);

	//ʹ�þ���֡����
	//��������
	auto bg = Sprite::create("background.png");
	//bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(bg, 0);
	//��������֡����
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("sprites.plist");
	//ͨ������ȥ���ʾ���֡�����еľ���֡
	auto mountain1 = Sprite::createWithSpriteFrameName("mountain1.png");
	mountain1->setAnchorPoint(Vec2::ZERO);
	mountain1->setPosition(Vec2(-200,80));
	this->addChild(mountain1);
	//�Ȼ�ȡ����֡�����еľ���֡�����ɾ���֡����������
	SpriteFrame* heroSF = frameCache->getSpriteFrameByName("hero1.png");
	auto hero = Sprite::createWithSpriteFrame(heroSF);
	hero->setPosition(Vec2(800,200));
	this->addChild(hero);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
