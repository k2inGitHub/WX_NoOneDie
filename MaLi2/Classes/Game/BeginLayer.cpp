//
//  BeginLayer.cpp
//  zuanshimicheng
//
//  Created by zhangrui on 14-2-12.
//
//



#include "BeginLayer.h"
#include "HLAdManagerWrapper.hpp"
using namespace cocos2d;
using namespace std;




Scene* BeginLayer::scene()
{
    Scene *scene = Scene::create();
    BeginLayer *layer = BeginLayer::create();
    scene->addChild(layer,-100);
    return scene;
}

BeginLayer::BeginLayer()
{

}
BeginLayer::~BeginLayer(){
}

bool BeginLayer::init()
{
    if(!LayerColor::initWithColor(Color4B(255, 255, 255, 255))){
        return false;
    }
    
    
    Sprite* title=Sprite::create("begin/title.png");
    this->addChild(title);
    title->setScale(0.9*screenSize.width/title->getContentSize().width);
    title->setPosition(ccpPercent(50, 90));
    
    for(int i=0; i<4; i++){
        
        auto button = MenuItemImage::create("begin/button.png", "begin/button.png", CC_CALLBACK_1(BeginLayer::enter_game, this));
        Menu* Menu = Menu::create(button, NULL);
        Menu->setPosition(Point::ZERO);
        this->addChild(Menu,1);
        button->setScale(0.4*screenSize.width/button->getContentSize().width);
        button->setPosition(ccpPercent(50, 75-14*i));
        button->setTag(i);
        Add_function::set_MenuItemImage_SelectedImage_Scale(button, 0.9f);
        
        char name[200];
        sprintf(name, "begin/%d.png",i);
        Sprite* word=Sprite::create(name);
        button->addChild(word);
        word->setPosition(ccpContent(button->getContentSize(), 50, 50));

    }
    
    Sprite* line=Sprite::create("begin/line.png");
    this->addChild(line);
    line->setScale(0.9*screenSize.width/line->getContentSize().width);
    line->setPosition(ccpPercent(50, 24));
    
    
    {
        sound_button = MenuItemImage::create("begin/sound.png", "begin/sound.png", CC_CALLBACK_1(BeginLayer::edit_sound, this));
        Menu* Menu = Menu::create(sound_button, NULL);
        Menu->setPosition(Point::ZERO);
        this->addChild(Menu,2);
        sound_button->setScale(0.18*screenSize.width/sound_button->getContentSize().width);
        sound_button->setPosition(ccpPercent(80, 15));
        Add_function::set_MenuItemImage_SelectedImage_Scale(sound_button, 0.9f);
        
        if(CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()==0){
            Add_function::setTextureByFile((Sprite*)sound_button->getNormalImage(), "begin/nosound.png");
        }
    }
    
    HLAdManagerWrapper::showUnsafeInterstitial();
    
    return true;
}

void BeginLayer::tick(float dt){
    

    
    
}

void BeginLayer::enter_game(Ref* obj){
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3", false);
    
    int tag=((Node *)obj)->getTag();
    UserDefault::getInstance()->setIntegerForKey("gamemode", tag);
    UserDefault::getInstance()->flush();
    
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameLayer::scene(), Color3B::WHITE));
}

void BeginLayer::edit_sound(Ref* obj){
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3", false);
    if(CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()==1){
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        
        Add_function::setTextureByFile((Sprite*)sound_button->getNormalImage(), "begin/nosound.png");
//        Add_function::setTextureByFile((Sprite*)sound_button->getSelectedImage(), "ui/begin/nosound.png");
    }
    else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        
        Add_function::setTextureByFile((Sprite*)sound_button->getNormalImage(), "begin/sound.png");
//        Add_function::setTextureByFile((Sprite*)sound_button->getSelectedImage(), "ui/begin/sound1.png");
    }
}


