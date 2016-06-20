//
//  BeginLayer.h
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//

#ifndef __zuanshimicheng__GameLayer__
#define __zuanshimicheng__GameLayer__

#include "cocos2d.h"
#include "GameHelper.h"
#include "Add_function.h"
#include "GameBody.h"

class GameLayer : public cocos2d::CCLayerColor
{
public:
    static cocos2d::CCScene* scene();
    
    virtual bool init();
    GameLayer();
    ~GameLayer();
    
    void tick(float dt);
public:
    CREATE_FUNC(GameLayer);
    
    
    bool check_is_over();
    void game_over();
    
    void replaceScene();

    void everyplay_start();
    
public:

    int game_mode;
    int body_count;
    GameBody* body[10];
    
    CCLabelTTF* time_show;
    float gametime=0;
    bool isover=false;

};

#endif /* defined(__zuanshimicheng__BeginLayer__) */
