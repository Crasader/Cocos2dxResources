#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"
USING_NS_CC;
class ContactListener :public b2ContactListener {
private:
	virtual void BeginContact(b2Contact* contact);//������Ӵ�ʱ��Ӧ
	virtual void EndContact(b2Contact* contact);//���������ʱ��Ӧ
	virtual void PreSolve(b2Contact* contact,const b2Manifold* oldManiFold);//�����Ӵ�ʱ��Ӧ�����ᱻ��ε���
	virtual void PostSolve(b2Contact* contact,const b2ContactImpulse* impulse);//�����Ӵ�ʱ��Ӧ����presolve֮�����
};