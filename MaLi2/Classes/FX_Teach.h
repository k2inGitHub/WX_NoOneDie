//
//  FX_Teach.h
//
//

#ifndef gs_FX_Teach_h
#define gs_FX_Teach_h

#include "cocos2d.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace cocos2d;

class FX_Teach : public Layer
{
private:
    Size size;

    LayerColor* back_layer;
    
    int teach_index;
    Node *rootNode;

public:
    void backMenu();
    void eventButton(Ref* pSender,Widget::TouchEventType type);

    
    virtual bool initRes(int index);
    ~FX_Teach();
    static FX_Teach* createNode(int index);
    void onExit();
    void updateGame(float dt);
    void onEnterTransitionDidFinish();
    
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif
