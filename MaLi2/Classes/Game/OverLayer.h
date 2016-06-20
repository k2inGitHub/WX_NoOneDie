//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__OverLayer__
#define __zuanshimicheng__OverLayer__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"
#include "GameLayer.h"


class OverLayer : public cocos2d::CCLayerColor
{
public:
    static cocos2d::CCScene* scene();
    
    virtual bool init();
    OverLayer();
    ~OverLayer();
    
    void tick(float dt);
    
    void back_menu(Ref* obj);
    void replay_game(Ref* obj);
    
    
    void show_full_ad();
    void share_friend(CCMenuItemImage*);

    void everyplay(CCMenuItemImage*);
    
    
    
public:
    CREATE_FUNC(OverLayer);
    
    int game_mode;
    float game_score;
    float bestscore;
    
    bool isnewscore=false;
    
    
};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
