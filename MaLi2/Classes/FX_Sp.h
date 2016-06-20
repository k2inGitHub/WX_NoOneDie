//
//  FX_Sp.h
//
//

#ifndef gs_FX_Sp_h
#define gs_FX_Sp_h

#include "cocos2d.h"
using namespace cocos2d;

class FX_Game;

class FX_Sp : public Sprite
{
    
public:
    
    ~FX_Sp();
    static FX_Sp* createNode(const char* img,int index);
    virtual bool initSp(const char* img,int index);

    void onExit();
    void onEnterTransitionDidFinish();

    void updateSp(float dt);
    void move_over2(Node* pSender);

public:
    int sp_type;
    cocos2d::Rect sp_rect;
    
    float sp_angle;
    
    int run_index;
    bool show_ing;
    bool disappear_ing;
    
    float show_angle;
    float add_angle;

    float disappear_angle;
};


#endif
