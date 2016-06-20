//
//  AppDelegate.cpp
//

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "FX_Menu.h"
#include "FX_App.h"
#include "LaunchLayer.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ScoreWallCommon.h"
#include "HLInterfaceWrapper.hpp"
#include "AdUnlockScene.hpp"
#include "HLAdManagerWrapper.hpp"
#include "LoadingLayer.hpp"
#endif

#include "BeginLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("FJDZ");
        director->setOpenGLView(glview);
    }
    Size frameSize = glview->getFrameSize();
    float scrnRatio = frameSize.height / frameSize.width;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCLOG("scrnRatio = %f",scrnRatio);
    
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    
    //    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
//        glview->setDesignResolutionSize(640,  960, ResolutionPolicy::SHOW_ALL);
    
//    MyApp->is_pad = false;
//    if(scrnRatio<1.5)
//    {
//        MyApp->is_pad = true;
//        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
//        
//    }
    
    ////    if(CUtil::platformString() == "iPad")
    //    if(scrnRatio < 1.5f)
    //    {
    ////        glview->setDesignResolutionSize(designResolutionSize2.width, designResolutionSize2.height, ResolutionPolicy::SHOW_ALL);
    //    }
    ////    else
    //    {
    //        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    //    }
    
#endif
    
    director->setContentScaleFactor(1);
    
    FileUtils* pFileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    searchPaths.push_back("hd");
    searchPaths.push_back("res");
    pFileUtils->setSearchPaths(searchPaths);
    
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0 / 60);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    umengLoad();
    xxx002();
    
    
#endif
    
//    //------------
//    for (int i = 0; i<CHENGJ_NUM; i++)
//    {
//        for (int k = 0; k<20; k++)
//            for (int m = 0; m<10; m++)
//                MyApp->chengjiu_data[i][k][m] = 0;
//        
//        MyApp->chengjiu_lv_num[i] = 0;
//    }
//    
//    for (int i = 0; i<CHENGJ_NUM; i++)
//        MyApp->readCSV_ChengjData(1+i);
//    //------------
    

    
    
    director->runWithScene(LoadingLayer::scene([=](Ref *sender){
      if (HLInterfaceWrapper::girl_start() == 1) {
//        if(0){
            glview->setDesignResolutionSize(750, 1125, ResolutionPolicy::FIXED_WIDTH);
            director->runWithScene(LaunchLayer::scene());
            
        } else {
            BannerShow();
            director->runWithScene(BeginLayer::scene());
        }
    }));
    
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    CCSendMyMessage();////每日推送
#endif
    
    Director::getInstance()->pause();
    Director::getInstance()->stopAnimation();
    
//    if(MyApp->isMusic)
//        AudioEngine->pauseBackgroundMusic();
    if(MyApp->isSound)
        AudioEngine->stopAllEffects();
    
//    if(MyApp->in_battle&&!MyApp->ask_relive_state)
//    {
//        MyApp->gameBattle->gamePause(NULL);
//    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
    
//    if(MyApp->isMusic)
//        AudioEngine->resumeBackgroundMusic();
    
//    MyApp->refrushTime();
}
