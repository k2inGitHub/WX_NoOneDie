//
//  FX_Teach.cpp
//
//
#include "FX_Teach.h"
#include "FX_App.h"
#include "SimpleAudioEngine.h"
#include "FX_Play.h"
#include "FX_Menu.h"
#include "HLAdManagerWrapper.hpp"

using namespace cocos2d;
using namespace CocosDenshion;



FX_Teach* FX_Teach::createNode(int index)
{
    FX_Teach *pRet = new FX_Teach();
    if (pRet && pRet->initRes(index))
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

FX_Teach::~FX_Teach()
{
    CCLOGINFO("delloc");
}

bool FX_Teach::initRes(int index)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    teach_index = index;
    
    size = (Director::getInstance())->getVisibleSize();

    if(index == 1)
    {
        LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100), size.width, size.height);
        this->addChild(black_layer);
        
        Sprite *lock = createCache_img("lock.png");
        lock->setAnchorPoint(Vec2(0.5, 0.5));
        lock->setPosition(Vec2(SCREEN_W/2, SCREEN_H/2));
        this->addChild(lock);
        
        Text *txt = Text::create("没工作就想相亲？", "Arial", 34);
        txt->setPosition(Vec2(lock->getContentSize().width/2, lock->getContentSize().height/2));
        txt->setColor(Color3B(0, 0, 0));
        lock->addChild(txt);
    }
    if(index == 2)
    {
        LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100), size.width, size.height);
        this->addChild(black_layer);
        
        Sprite *lock = createCache_img("lock.png");
        lock->setAnchorPoint(Vec2(0.5, 0.5));
        lock->setPosition(Vec2(SCREEN_W/2, SCREEN_H/2));
        this->addChild(lock);
        
        Text *txt = Text::create("没对象还敢回家过年？", "Arial", 34);
        txt->setPosition(Vec2(lock->getContentSize().width/2, lock->getContentSize().height/2));
        txt->setColor(Color3B(0, 0, 0));
        lock->addChild(txt);
    }
    if(index == 10)
    {
        Sprite *player = createCache_img("player.png");
        player->setAnchorPoint(Vec2(0, 0));
        player->setPosition(Vec2(10, 0));
        this->addChild(player);
        
        Sprite *ui_window = createCache_img("ui_window.png");
        ui_window->setAnchorPoint(Vec2(0, 0));
        ui_window->setPosition(Vec2(200, 300));
        this->addChild(ui_window,1,1);
    }
    
    if(index == 20)//失败
    {
        
        LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 200), size.width, size.height);
        this->addChild(black_layer);
        
        rootNode = CSLoader::createNode("kuang0.csb");
        this->addChild(rootNode);
        
        rootNode->setPosition(Vec2(SCREEN_W/2, SCREEN_H/2));
        
        Text *txt = (Text*)rootNode->getChildByTag(198);
        char t_num[128] = {0};
        sprintf(t_num,"年轻人，就你那点伎俩\n我们早已看穿了！");
        txt->setString(t_num);
        
        Button *btnCancel = (Button*)rootNode->getChildByTag(195);//返回
        btnCancel->addTouchEventListener([this,btnCancel](Ref* pSender,Widget::TouchEventType type){
            this->eventButton(pSender, type);
            if (type == Widget::TouchEventType::ENDED)
            {
                backMenu();
            }
        });
        
        Button *btnOK = (Button*)rootNode->getChildByTag(196);//重来
        btnOK->addTouchEventListener([this,btnOK](Ref* pSender,Widget::TouchEventType type){
            this->eventButton(pSender, type);
            if (type == Widget::TouchEventType::ENDED)
            {
                ((FX_Play*)this->getParent())->resetGame();
            }
        });
        Button *btnNext = (Button*)rootNode->getChildByTag(197);//下一关
        btnNext->setVisible(false);
        HLAdManagerWrapper::showUnsafeInterstitial();
        
    }
    if(index == 21)//胜利
    {
        LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 200), size.width, size.height);
        this->addChild(black_layer);
        
        rootNode = CSLoader::createNode("kuang0.csb");
        this->addChild(rootNode);
        
        rootNode->setPosition(Vec2(SCREEN_W/2, SCREEN_H/2));
        
        Sprite *back = (Sprite*)rootNode->getChildByTag(193);
        back->setTexture("txt_win.png");
        
        Text *txt = (Text*)rootNode->getChildByTag(198);
        
        const char* str_name[3] ={"面试","相亲","回家过年",};
        
        char t_num[128] = {0};
        sprintf(t_num,"太牛了，你已成功完成\n%s大考验！",str_name[MyApp->gk_index]);
        txt->setString(t_num);
        
        Button *btnCancel = (Button*)rootNode->getChildByTag(195);//返回
        btnCancel->addTouchEventListener([this,btnCancel](Ref* pSender,Widget::TouchEventType type){
            this->eventButton(pSender, type);
            if (type == Widget::TouchEventType::ENDED)
            {
                backMenu();
            }
        });
        
        Button *btnOK = (Button*)rootNode->getChildByTag(196);//重来
        btnOK->setVisible(false);
    
        Button *btnNext = (Button*)rootNode->getChildByTag(197);//下一关
        btnNext->addTouchEventListener([this,btnNext](Ref* pSender,Widget::TouchEventType type){
            this->eventButton(pSender, type);
            if (type == Widget::TouchEventType::ENDED)
            {
                ((FX_Play*)this->getParent())->nextGame();
            }
        });
        if(MyApp->gk_index == 2)
        {
            btnNext->setVisible(false);
            btnCancel->setPositionX(0);
        }
        
        
        HLAdManagerWrapper::showUnsafeInterstitial();
    }

    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FX_Teach::onTouchBegan,this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FX_Teach::onTouchMoved,this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FX_Teach::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}
void FX_Teach::backMenu()
{
    Scene *sc = Scene::create();
    FX_Menu *temp = FX_Menu::createNode();
    sc->addChild(temp);
    TransitionCrossFade *ts = TransitionCrossFade::create(0.5, sc);
    Director::getInstance()->replaceScene(ts);
}
void FX_Teach::eventButton(Ref* pSender,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
        ((Node*)pSender)->setScale(1.1f);
    if (type == Widget::TouchEventType::CANCELED)
        ((Node*)pSender)->setScale(1.0f);
    if (type == Widget::TouchEventType::ENDED)
    {
        ((Node*)pSender)->setScale(1.0f);
        MyApp->playSfx("sound/click.aif");
    }
}
void FX_Teach::updateGame(float dt)
{

}

bool FX_Teach::onTouchBegan(Touch *touch,Event *pEvent)
{
    if(teach_index == 10)
        ((FX_Menu*)this->getParent())->btn_layer->setVisible(true);
    
    if(teach_index!=20&&teach_index!=21)
       onExit();
    return true;
}
void FX_Teach::onTouchMoved(Touch *touch,Event *pEvent)
{
}
void FX_Teach::onTouchEnded(Touch *touch,Event *pEvent)
{
}
void FX_Teach::onEnterTransitionDidFinish()
{
    this->schedule(schedule_selector(FX_Teach::updateGame), 1.0f);
    Layer::onEnterTransitionDidFinish();
}

void FX_Teach::onExit()
{
    CCLOG("exit");
    
    this->unscheduleAllCallbacks();
    this->removeAllChildrenWithCleanup(true);
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    Layer::onExit();
}
