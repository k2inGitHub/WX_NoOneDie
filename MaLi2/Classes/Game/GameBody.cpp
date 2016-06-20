//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "GameBody.h"
#include "GameLayer.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

GameBody::GameBody()
{
    hinder_arrary=new CCArray();
    hinder_arrary->init();
}
GameBody::~GameBody()
{
    
}

bool GameBody::init(CCNode* layer_, CCPoint p, float touch_height)
{
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 0))){
        return false;
    }
    this->setTouchEnabled(true);
    
    layer=layer_;
    layer->addChild(this);
    this->setPosition(p);
    bodyhight=touch_height;
    
    init_game();
    
//    {
//        this->setScaleX(-1);
//        this->setPosition(p.x-screenSize.width, p.y);
//    }
    
    this->schedule(schedule_selector(GameBody::tick));
    
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(GameBody::onTouchesBegan, this);
    
    return true;
}

void GameBody::tick(float dt){
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(hinder_arrary, pObj)
    {
        CCSprite* hinder=(CCSprite*)pObj;
    
        if(get_player_bound().intersectsRect(hinder->boundingBox())){
            if(((GameLayer*)layer)->check_is_over()==false){
                if(isghost==false){
                    player_to_ghost();
                    hinder->setPosition(ccpPercent(-500, 0));
                }
            }
        }
        
        if(hinder->getPosition().x<-screenSize.width){
            hinder_arrary->removeObject(hinder);
            this->removeChild(hinder, true);
        }
        
    }
    
    if(isghost==true){
        ghosttime+=dt;
    }
    
}

void GameBody::init_game(){
    CCSprite* line=CCSprite::create("game/line.png");
    this->addChild(line);
    line->setScaleX(1.1*screenSize.width/line->getContentSize().width);
    

    player=CCSprite::create("game/run1.png");
    this->addChild(player,1);
    player->setScale(0.04*screenSize.height/player->getContentSize().height);
    player->setPosition(ccp(-screenSize.width*0.3f,player->boundingBox().size.height*0.5f));
    
    CCAnimation *animationSwim=Add_function::createWithFileName("game/run", 0.1f, 2, true, 1);
    CCAnimate *ani = CCAnimate::create(animationSwim);
    CCRepeatForever* r=CCRepeatForever::create(ani);
    r->setTag(120);
    player->runAction(r);
    
    {
        float nexttime=1+CCRANDOM_0_1()*2;
        CCDelayTime* d=CCDelayTime::create(nexttime);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameBody::add_hinder));
        CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
        this->runAction(l);
    }
    
    {
        touchround=CCSprite::create("game/hinder.png");
        this->addChild(touchround,-1);
        touchround->setOpacity(50);
        touchround->setAnchorPoint(ccp(0.5,0));
        touchround->setScaleX(screenSize.width/touchround->getContentSize().width);
        touchround->setScaleY(bodyhight/touchround->getContentSize().height);
    }
}

#pragma mark-
#pragma mark 障碍
void GameBody::add_hinder(){

    CCSprite* hinder=CCSprite::create("game/hinder.png");
    this->addChild(hinder,-1);
    hinder->setAnchorPoint(ccp(0.5f,0));
    
    float scalex=0.3+0.5*(CCRANDOM_0_1()*100/100.0f)*player->boundingBox().size.width/hinder->getContentSize().width;
    float scaley=0.5+0.7*(CCRANDOM_0_1()*100/100.0f)*player->boundingBox().size.height/hinder->getContentSize().height;
    hinder->setScaleX(scalex);
    hinder->setScaleY(scaley);
    hinder->setPosition(ccpPercent(60, 0));
    
    hinder_arrary->addObject(hinder);
    
    CCMoveBy* a1=CCMoveBy::create(1.0f, ccpPercent(-70, 0));
    CCRepeatForever* r=CCRepeatForever::create(a1);
    hinder->runAction(r);
    
    
    {
        float nexttime=1+CCRANDOM_0_1()*2;
        CCDelayTime* d=CCDelayTime::create(nexttime);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameBody::add_hinder));
        CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
        this->runAction(l);
    }
    
}


#pragma mark-
#pragma mark 动作
void GameBody::jump_action(){
    if(isjump==false&&isover==false){
        isjump=true;
        
        CCPoint p=player->getPosition();
        
        CCMoveTo* a1=CCMoveTo::create(0.25f, ccp(p.x, p.y+player->boundingBox().size.height*2.5f));
        CCEaseOut* b1=CCEaseOut::create(a1, 1.3f);
        CCDelayTime* d=CCDelayTime::create(0.05f);
        CCMoveTo* a2=CCMoveTo::create(0.25f, ccp(p.x, p.y));
        CCEaseIn* b2=CCEaseIn::create(a2, 1.3f);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameBody::jump_action_over));
        action_jump=(CCSequence*)CCSequence::create(b1,d,b2,f,NULL);
        player->runAction(action_jump);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/jump.mp3", false);
    }
}

void GameBody::jump_action_over(){
    isjump=false;
}

#pragma mark-
#pragma mark 功能
CCRect GameBody::get_player_bound(){
    return Add_function::get_scale_bound(player, 0.8f, 0.9f, 0, 0);
}

void GameBody::player_to_ghost(){
    isghost=true;
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/ghost.mp3", false);
    
    player->stopActionByTag(120);
    
    Add_function::setTextureByFile(player, "game/ghost1.png");
    CCAnimation *animationSwim=Add_function::createWithFileName("game/ghost", 0.1f, 2, true, 1);
    CCAnimate *ani = CCAnimate::create(animationSwim);
    CCRepeatForever* r=CCRepeatForever::create(ani);
    r->setTag(120);
    player->runAction(r);
    
    player->setOpacity(0);
    player->runAction(CCFadeTo::create(0.5f, 255));
    
    {
        CCSprite* bomb=CCSprite::create("game/bomb/1.png");
        this->addChild(bomb,10);
        bomb->setPosition(player->getPosition());
        
        CCAnimation *animationSwim=Add_function::createWithFileName("game/bomb/", 0.1f, 3, false, 1);
        CCAnimate *ani = CCAnimate::create(animationSwim);
        bomb->runAction(ani);
        
        Add_function::removeDelay(bomb, 0.3f);
    }
}

void GameBody::player_over(){
//    if(isjump==true){
//        player->stopAction(action_jump);
//    }
    
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(hinder_arrary, pObj)
    {
        CCSprite* hinder=(CCSprite*)pObj;
        hinder->stopAllActions();
    }
    
    this->stopAllActions();
    
    isover=true;
}


#pragma mark-
#pragma mark 触控
void GameBody::onTouchesBegan(const std::vector<cocos2d::Touch*>& pTouches,Event *pEvent){

    for(auto &item:pTouches){
        auto touch = item;
        Point Point = this->convertTouchToNodeSpace(touch);
        if(touchround->getBoundingBox().containsPoint(Point)){
            jump_action();
        }
    }
    
}


