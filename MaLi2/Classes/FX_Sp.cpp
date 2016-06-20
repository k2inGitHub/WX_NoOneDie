//
//  FX_Sp.cpp
//
//

#include "FX_Sp.h"
#include "FX_App.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


FX_Sp* FX_Sp::createNode(const char* img,int index)
{
    FX_Sp *pRet = new FX_Sp();
    if (pRet && pRet->initSp(img,index))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}


FX_Sp::~FX_Sp()
{
    CCLOGINFO("delloc");
}

bool FX_Sp::initSp(const char* img,int index)
{
    if ( !Sprite::initWithFile(img))
        return false;
    
    sp_type = index;
    
    run_index = 0;
    
    show_angle = 0;
    disappear_angle = 0;
    show_ing = true;
    disappear_ing = false;
    
    this->setOpacity(0);

//    this->schedule(schedule_selector(FX_Sp::updateSp));
    
    
//    {
//        
//
//        char s_enemy_face[64] = {0};
//        sprintf(s_enemy_face,"shadow%d.png", sp_type-100);
//        Sprite* face1 = createCache((s_enemy_face));
//        face1->setPosition(Vec2(this->getContentSize().width/2-40,this->getContentSize().height/2-40));
//        this->addChild(face1,-1,10);
//        
//        Sprite* hp0 = createCache("enemy_hp0.png");
//        hp0->setAnchorPoint(Vec2(0, 0.5));
//        hp0->setPosition(Vec2(this->getContentSize().width/2-57,-12));
//        hp0->setVisible(false);
//        this->addChild(hp0,2,1);
//        
//        Sprite* hp1 = createCache("enemy_hp1.png");
//        hp1->setAnchorPoint(Vec2(0, 0.5));
//        hp1->setPosition(Vec2(this->getContentSize().width/2-53,-12));
//        hp1->setVisible(false);
//        this->addChild(hp1,3,2);
//    }
    
    return true;
}
void FX_Sp::updateSp(float dt)
{
    if(MyApp->game_over)
        return;
    
    if(show_ing)
    {
//        if(show_angle == 0)
//            this->runAction(FadeTo::create(0.1, 80));
        
        show_angle+=MyApp->sp_speed[sp_type-1];
        
        this->setOpacity(255*show_angle/90);

        if(show_angle>=90)
        {
            show_angle = 0;
            show_ing = false;
//            this->runAction(FadeTo::create(0.1, 255));
            this->setOpacity(255);
        }
    }
    
    Vec2 center = Vec2(SCREEN_W/2,SCREEN_H-606);

    sp_angle += MyApp->sp_speed[sp_type-1];
    add_angle += MyApp->sp_speed[sp_type-1];
    
    float offx = RR*sin((M_PI/180)*(sp_angle));
    float offy = RR*cos((M_PI/180)*(sp_angle));
    
    this->setPosition(Vec2(center.x+offx,center.y+offy));
    this->setRotation(90+sp_angle);
    
    if(sp_angle>=360)
    {
        sp_angle = 0;
    }
    if(add_angle>=360)
    {
        add_angle = 0;
        run_index++;
        if(run_index == 4)
        {
            disappear_ing = true;
            FadeTo* act0 = FadeTo::create(0.5, 0);
            CallFuncN* act1= CallFuncN::create(CC_CALLBACK_1(FX_Sp::move_over2,this));
            Sequence* act_x0 = (Sequence*)Sequence::create(act0,act1,NULL);
            this->runAction(act_x0);
        }
    }
    
}
void FX_Sp::move_over2(Node* pSender)
{
    pSender->setVisible(false);
}
void FX_Sp::onEnterTransitionDidFinish()
{
    this->schedule(schedule_selector(FX_Sp::updateSp));
    Sprite::onEnterTransitionDidFinish();
}
void FX_Sp::onExit()
{
    this->unscheduleAllCallbacks();
    Sprite::onExit();
}



