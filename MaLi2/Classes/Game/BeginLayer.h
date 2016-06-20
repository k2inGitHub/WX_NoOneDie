//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__BeginLayer__
#define __zuanshimicheng__BeginLayer__

#include "cocos2d.h"
#include "GameHelper.h"
#include "GameLayer.h"
#include "Add_function.h"

class BeginLayer : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* scene();
    
    virtual bool init();
    BeginLayer();
    ~BeginLayer();
    
    MenuItemImage *sound_button;
    
    void tick(float dt);
public:
    CREATE_FUNC(BeginLayer);
    
    void enter_game(Ref* obj);
    
    void enter_gamecenter();
    void edit_sound(Ref* obj);
};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
