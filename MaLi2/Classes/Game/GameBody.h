//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__GameBody__
#define __zuanshimicheng__GameBody__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"
//#include "ActionDrop.h"
//#include "ActionStar.h"

class GameBody : public cocos2d::CCLayerColor
{
public:
    
    virtual bool init(CCNode* layer_, CCPoint p, float touch_height);
    GameBody();
    ~GameBody();
    
    
    
    
    void tick(float dt);
    
    void init_game();
    
    void add_hinder();
    
    void jump_action();
    void jump_action_over();
    
    CCRect get_player_bound();
    
    void player_to_ghost();
    void player_over();
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& pTouches,Event *pEvent);

    
public:

    
    CCNode* layer;
    
    float bodyhight;
    
    CCSprite* player;
    
    CCSequence* action_jump;
    
    bool isjump=false;
    
    CCArray* hinder_arrary;
    
    bool isghost=false;
    float ghosttime=0;
    
    bool isover=false;
    
    CCSprite* touchround;
    



};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
