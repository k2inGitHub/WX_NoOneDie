//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "GameLayer.h"
#include "BeginLayer.h"
#include "OverLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;




CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer,-100);
    return scene;
}

GameLayer::GameLayer()
{
    game_mode=CCUserDefault::sharedUserDefault()->getIntegerForKey("gamemode");
    body_count=game_mode+2;
}
GameLayer::~GameLayer(){
}

bool GameLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))){
        return false;
    }
    
    
    {
        CCDelayTime* d=CCDelayTime::create(1.0f);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameLayer::everyplay_start));
        CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
        this->runAction(l);
    }
    
    {
        for(int i=0; i<body_count; i++){
            body[i]=new GameBody();
            body[i]->init(this, ccpPercent(50, 15+i*(75/body_count)), (0.75/body_count)*screenSize.height);
        }
    }
    
    
    
    this->schedule(schedule_selector(GameLayer::tick));
    
    
//    {
//        CCMenuItemImage *button = CCMenuItemImage::create(
//                                                          "icon7.png",
//                                                          "icon7.png",
//                                                          this,
//                                                          menu_selector(GameLayer::replaceScene) );
//        CCMenu* Menu = CCMenu::create(button, NULL);
//        Menu->setPosition(CCPointZero);
//        this->addChild(Menu,1);
//        button->setPosition(ccpPercent(10, 95));
//
//    }
    
    time_show = CCLabelTTF::create("0", "ttf/word.ttf", screenSize.height*0.05f);
    time_show->setAnchorPoint(ccp(1,1));
    this->addChild(time_show, 1);
    time_show->setPosition(ccpPercent(98, 95));
    time_show->setColor(ccc3(100, 100, 100));
    
    
    return true;
}

void GameLayer::tick(float dt){
    
    if(isover==false){
        gametime+=dt;
    }
    
    char text[200];
    sprintf(text, "%.3fs", gametime);
    time_show->setString(text);
    
    
}

bool GameLayer::check_is_over(){
    
    bool beover=false;

    for(int i=0; i<body_count; i++){
        if(body[i]->isghost==true){
            beover=true;
        }
    }
    
    if(beover==true){
        game_over();
    }
    
    return beover;
}

void GameLayer::game_over(){
    if(isover==false){
        isover=true;
        for(int i=0; i<body_count; i++){
            body[i]->player_over();
        }
        
        CCDelayTime* d=CCDelayTime::create(0.5f);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(GameLayer::replaceScene));
        CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
        this->runAction(l);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/fail.mp3", false);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/hit.mp3", false);
    }
}

void GameLayer::replaceScene(){
    
    CCUserDefault::sharedUserDefault()->setFloatForKey("gamescore", gametime);
    CCUserDefault::sharedUserDefault()->flush();
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, OverLayer::scene(), ccWHITE));
}


void GameLayer::everyplay_start(){
    
}

