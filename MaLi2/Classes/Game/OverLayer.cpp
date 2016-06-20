//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "OverLayer.h"
#include "BeginLayer.h"
#include "GameLayer.h"
#include "HLAdManagerWrapper.hpp"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;




CCScene* OverLayer::scene()
{
    CCScene *scene = CCScene::create();
    OverLayer *layer = OverLayer::create();
    scene->addChild(layer,-100);
    return scene;
}

OverLayer::OverLayer()
{
    game_mode=CCUserDefault::sharedUserDefault()->getIntegerForKey("gamemode");
    game_score=CCUserDefault::sharedUserDefault()->getFloatForKey("gamescore");
    
    char name[200];
    sprintf(name, "bestscore%d", game_mode);
    bestscore=CCUserDefault::sharedUserDefault()->getFloatForKey(name);
    
    if(game_score>bestscore){
        bestscore=game_score;
        CCUserDefault::sharedUserDefault()->setFloatForKey(name, bestscore);
        CCUserDefault::sharedUserDefault()->flush();
        
        isnewscore=true;
        
        int subscore=bestscore*1000;

    }
    
    {
        char text[100];
        sprintf(text, "mode:%d,score:%d",game_mode,(int)game_score);

    }
    {
        if(CCUserDefault::sharedUserDefault()->getIntegerForKey("eachscore")==0){
            char text[100];
            sprintf(text, "mode:%d,score:%d",game_mode,(int)game_score);
            
            CCUserDefault::sharedUserDefault()->setIntegerForKey("eachscore", 1);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
}
OverLayer::~OverLayer(){
}

bool OverLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(95, 158, 79, 255))){
        return false;
    }
    
    
    {
        char name[200];
        sprintf(name, "begin/%d.png", game_mode);
        
        CCSprite* title=CCSprite::create(name);
        this->addChild(title);
        title->setScale(0.5*screenSize.width/title->getContentSize().width);
        title->setPosition(ccpPercent(50, 90));
    }
    
    {
        CCSprite* line=CCSprite::create("over/line.png");
        this->addChild(line);
        line->setScale(0.9*screenSize.width/line->getContentSize().width);
        line->setPosition(ccpPercent(50, 80));
    }
    
    
    {
        CCSprite* new_word=CCSprite::create("over/newscore.png");
        this->addChild(new_word,1);
        new_word->setScale(0.4*screenSize.width/new_word->getContentSize().width);
        new_word->setPosition(ccpPercent(50, 67));
        
        if(isnewscore==false){
            new_word->setVisible(false);
        }
        else{
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/newscore.mp3", false);
        }
    }

    
    {
        CCLabelTTF* score_show = CCLabelTTF::create("0", "ttf/word.ttf", screenSize.height*0.07f);
        score_show->setAnchorPoint(ccp(0.5f,0.5f));
        this->addChild(score_show, 1);
        score_show->setPosition(ccpPercent(50, 50));
        
        char text[200];
        sprintf(text, "%.3f秒",game_score);
        score_show->setString(text);
    }
    
    {
        CCLabelTTF* score_show = CCLabelTTF::create("0", "ttf/word.ttf", screenSize.height*0.04f);
        score_show->setAnchorPoint(ccp(0.5f,0.5f));
        this->addChild(score_show, 1);
        score_show->setPosition(ccpPercent(50, 37));
        
        char text[200];
        sprintf(text, "最佳：%.3f秒",bestscore);
        score_show->setString(text);
    }
    
    
    {
        CCSprite* line=CCSprite::create("over/line.png");
        this->addChild(line);
        line->setScale(0.9*screenSize.width/line->getContentSize().width);
        line->setPosition(ccpPercent(50, 30));
    }
    
    
    {
        CCMenuItemImage *button = CCMenuItemImage::create(
                                                          "over/back.png",
                                                          "over/back.png",
                                                          this,
                                                          menu_selector(OverLayer::back_menu) );
        CCMenu* Menu = CCMenu::create(button, NULL);
        Menu->setPosition(CCPointZero);
        this->addChild(Menu,1);
        button->setScale(0.18*screenSize.width/button->getContentSize().width);
        button->setPosition(ccpPercent(20, 20));
        Add_function::set_MenuItemImage_SelectedImage_Scale(button, 0.9f);
    }
    
    {
        CCMenuItemImage *button = CCMenuItemImage::create(
                                                          "over/reset.png",
                                                          "over/reset.png",
                                                          this,
                                                          menu_selector(OverLayer::replay_game) );
        CCMenu* Menu = CCMenu::create(button, NULL);
        Menu->setPosition(CCPointZero);
        this->addChild(Menu,1);
        button->setScale(0.18*screenSize.width/button->getContentSize().width);
        button->setPosition(ccpPercent(50, 20));
        Add_function::set_MenuItemImage_SelectedImage_Scale(button, 0.9f);
    }
    
//    {
//        CCMenuItemImage *button = CCMenuItemImage::create(
//                                                          "over/share.png",
//                                                          "over/share.png",
//                                                          this,
//                                                          menu_selector(OverLayer::share_friend) );
//        CCMenu* Menu = CCMenu::create(button, NULL);
//        Menu->setPosition(CCPointZero);
//        this->addChild(Menu,1);
//        button->setScale(0.18*screenSize.width/button->getContentSize().width);
//        button->setPosition(ccpPercent(80, 20));
//        Add_function::set_MenuItemImage_SelectedImage_Scale(button, 0.9f);
//    }
//    
//    {
//        CCMenuItemImage *button = CCMenuItemImage::create(
//                                                          "share/everyplay.png",
//                                                          "share/everyplay.png",
//                                                          this,
//                                                          menu_selector(OverLayer::everyplay) );
//        CCMenu* Menu = CCMenu::create(button, NULL);
//        Menu->setPosition(CCPointZero);
//        this->addChild(Menu,1);
//        button->setScale(0.18*screenSize.width/button->getContentSize().width);
//        button->setPosition(ccpPercent(18, 70));
//        button->setVisible(false);
//        
//        if(CCUserDefault::sharedUserDefault()->getIntegerForKey("everyplay")==1){
//            button->setVisible(true);
//        }
//        
//        {
//            float scale=button->getScale();
//            CCScaleTo* a1=CCScaleTo::create(0.5f, scale*1.2f);
//            CCScaleTo* a2=CCScaleTo::create(0.5f, scale);
//            CCSequence* l=(CCSequence*)CCSequence::create(a1,a2,NULL);
//            CCRepeatForever* r=CCRepeatForever::create(l);
//            button->runAction(r);
//        }
//    }
    
    {
        float delay=0.8f;
        float umeng_delay=CCUserDefault::sharedUserDefault()->getFloatForKey("FullScreenDelay");
        if(umeng_delay!=0){
            delay=umeng_delay;
        }
        
        CCDelayTime* d=CCDelayTime::create(delay);
        CCCallFunc * f=CCCallFunc::create(this, callfunc_selector(OverLayer::show_full_ad));
        CCSequence* l=(CCSequence*)CCSequence::create(d,f,NULL);
        this->runAction(l);
    }
    
    HLAdManagerWrapper::showUnsafeInterstitial();
    
    return true;
}

void OverLayer::tick(float dt){
    
}


void OverLayer::back_menu(Ref* obj){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, BeginLayer::scene(), ccWHITE));
}

void OverLayer::replay_game(Ref* obj){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, GameLayer::scene(), ccWHITE));
}

void OverLayer::show_full_ad(){
}

#pragma mark-
#pragma mark 分享


void OverLayer::share_friend(CCMenuItemImage*){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
}

void OverLayer::everyplay(CCMenuItemImage*){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.mp3", false);
    char number[200];
    sprintf(number, "%f",game_score);
}

