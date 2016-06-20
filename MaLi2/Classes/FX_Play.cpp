//
//  FX_Play.cpp
//
//
#include "FX_Play.h"
#include "FX_Teach.h"
//#include "FX_Info.h"
//#include "FX_Shop.h"
#include "FX_App.h"
//#include "uiMenuDialog.h"
//#include "FX_Load.h"
//#include "FX_Spcial.h"
//#include "FX_Role.h"
//#include "FX_Honor.h"

#include "CSVParser.h"
#include "SimpleAudioEngine.h"
#include "FX_Sp.h"
#include "FX_Menu.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Business.h"
#include "ScoreWallCommon.h"

#endif

using namespace cocos2d;
using namespace CocosDenshion;


const char* str_question[3][6] =
{
    {
        "请介绍一下你自己。",
        "你是哪里人？",
        "你是哪所院校毕业的？",
        "你最大的优点是什么？",
        "你有什么业余爱好？",
        "你的梦想是什么？",
    },
    {
        "你交过几个女朋友？",
        "你对女方有什么要求？",
        "你今年多大了？",
        "你在什么单位上班？",
        "你开的什么车？",
        "你银行存款有几位数？",
    },
    {
        "有女朋友了吗？",
        "找到工作了吗？",
        "一个月能赚多少钱？",
        "你最近忙啥呢？",
        "电视上的英语能听懂吗？",
        "啥时候买房啊？",
    }
};//

const char* str_answer[3][6][6] =
{
    {
        {"大家好!","我姓叶","大家都","叫我良辰",},
        {"我的",	"家在","东北","松花江上",},
        {"德州的","名校","家里蹲","大学",},
        {"本人",	"最大的","优点是","没有优点",},
        {"工作",	"是我","最业余","的爱好",},
        {"报名",	"参加","中国","好声音",},
    },
    {
        {"没有","这已经","是我的","第32次","相亲了",},
        {"没要求","只要","是个","女的","就行",},
        {"我还是","一位","自在	","如风的","少年",},
        {"我在","一家","大型的	","工地","搬砖",},
        {"一辆","360度","全景","天窗的","自行车",},
        {"刚刚","数过","小数点","后面","五位数",},
    },
    {
        {"有了!","被你","儿子","租","回家","过年了",},
        {"找到了!	","我","已经","被	","富婆","包养了",},
        {"不多!","你们","今年","打麻将","输了","多少钱",},
        {"研究","中老年","爱打听","是否","属于","精神病",},
        {"能听懂","意思是","问太多","的人","往往","都很丑",},
        {"阿姨","您老公","啥时候","能成为","公司","总经理",},
    },
};
//using namespace tinyxml2;
Scene* FX_Play::scene()
{
    Scene *scene = Scene::create();
    FX_Play *layer = FX_Play::createNode();
    scene->addChild(layer);
    return scene;
}

FX_Play* FX_Play::createNode() 
{
    FX_Play *pRet = new FX_Play();
    if (pRet && pRet->initRes())
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

FX_Play::~FX_Play()
{
    CCLOGINFO("delloc");
}

bool FX_Play::initRes()
{
    if (!Layer::init())
        return false;
    
    startTime = false;
    
    readCSV_CarData();

    speed_date[0] = MyApp->car_data[2];//1.5;//初始速度
    speed_date[1] = MyApp->car_data[3];//5;//油门速度
    speed_date[2] = MyApp->car_data[4];//0.4;//刹车速度
    
    MyApp->sp_speed[0] = MyApp->randMinMax(MyApp->car_data[8], MyApp->car_data[9]);//0.5*(3+(arc4random()%5));
    MyApp->sp_speed[1] = MyApp->randMinMax(MyApp->car_data[10], MyApp->car_data[11]);
    MyApp->sp_speed[2] = MyApp->randMinMax(MyApp->car_data[12], MyApp->car_data[13]);
    MyApp->sp_speed[3] = MyApp->randMinMax(MyApp->car_data[14], MyApp->car_data[15]);
//    MyApp->sp_speed[4] = MyApp->randMinMax(MyApp->car_data[8], MyApp->car_data[9]);

    role_speed = speed_date[0];
    
    car_num = 0;
    game_time = 6.0;
    speed_step = 5;
    MyApp->play_num = 0;
    boss_in_hit = false;
    role_in_hit = false;
    MyApp->game_over = false;
    MyApp->game_win = false;
    
    size = (Director::getInstance())->getVisibleSize();

    Sprite *game_bg = Sprite::create("bg.jpg");
    game_bg->setPosition(Vec2(SCREEN_W/2, SCREEN_H));
    game_bg->setAnchorPoint(Vec2(0.5, 1));
    this->addChild(game_bg);
    
    
    Node *scene = CSLoader::createNode("tucao0.csb");
    rootNode = (Scene*)scene;
    this->addChild(rootNode,1);
    
    for (int i = 0; i<6; i++)
    {
        rootNode->getChildByTag(160)->getChildByTag(300+i)->setVisible(false);
        game_xxx[i] = rootNode->getChildByTag(160)->getChildByTag(300+i)->getPositionX();
        game_yyy[i] = rootNode->getChildByTag(160)->getChildByTag(300+i)->getPositionY();
    }
    
    char t_num[128] = {0};
    sprintf(t_num,"bg_stage%d.jpg",MyApp->gk_index+1);
    Sprite *back = (Sprite*)rootNode->getChildByTag(158);
    back->setTexture(t_num);
    
    word_num = 4;
    if(MyApp->gk_index == 1)
    {
        word_num = 5;
        game_time = 5.0;
    }
    if(MyApp->gk_index == 2)
    {
        word_num = 6;
        game_time = 4.0;
    }
    
    for (int i = 0; i<word_num; i++)
    {
        Button *btnWord = (Button*)rootNode->getChildByTag(160)->getChildByTag(300+i);
        btnWord->addTouchEventListener([this](Ref* pSender,Widget::TouchEventType type){
            this->eventButton2(pSender, type);
            if (type == Widget::TouchEventType::ENDED)
            {
                ((Button*)pSender)->setEnabled(false);
                int index = ((Button*)pSender)->getTag()-300;
                press_count+=(press_word_num*index);
                press_word_num++;
                
                CCLOG("press_count = %d",press_count);
                if(press_word_num>=word_num)
                    dealResult();
            }
        });
    }
    
    rootNode->getChildByTag(160)->setVisible(false);
    rootNode->getChildByTag(188)->setVisible(false);
    
    MyApp->question_index = 0;

    Text* txt0 = (Text*)rootNode->getChildByTag(163);
    char str[64] = {0};
    sprintf(str,"%.1f",game_time);
    txt0->setString(str);
    
//
////    FX_Teach* teach_layer = FX_Teach::createNode(0);
////    this->addChild(teach_layer,10000);
//    
//    auto touchListener = EventListenerTouchOneByOne::create();
//    touchListener->setSwallowTouches(true);
//    touchListener->onTouchBegan = CC_CALLBACK_2(FX_Play::onTouchBegan,this);
//    touchListener->onTouchMoved = CC_CALLBACK_2(FX_Play::onTouchMoved,this);
//    touchListener->onTouchEnded = CC_CALLBACK_2(FX_Play::onTouchEnded, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}
void FX_Play::resetGame()
{
    Scene *sc = Scene::create();
    FX_Play *temp = FX_Play::createNode();
    sc->addChild(temp);
    TransitionCrossFade *ts = TransitionCrossFade::create(0.5, sc);
    Director::getInstance()->replaceScene(ts);
}
void FX_Play::nextGame()
{
    MyApp->gk_index++;
    Scene *sc = Scene::create();
    FX_Play *temp = FX_Play::createNode();
    sc->addChild(temp);
    TransitionCrossFade *ts = TransitionCrossFade::create(0.5, sc);
    Director::getInstance()->replaceScene(ts);
}
#pragma mark- ***读取外部文件*** -
void FX_Play::readCSV_CarData()
{
    for (int i = 0; i<50; i++)
        MyApp->car_data[i] = 0;
    
    char s_file[64] = {0};
    sprintf(s_file,"xml/car_data.csv");
    
    CSVParser::Csv date = CSVParser::Csv(s_file);
    int date_lenght = (int)date.getRowCount();
    CCLOG("@sssss = %s",s_file);
    
    
    //    CCLOG("@_date_lenght = %d",date_lenght);
    
    for (int i = 1; i<date_lenght; i++)
        MyApp->car_data[i+1] = (__String::createWithFormat("%s", date[i]["data"].c_str()))->floatValue();
    
        for (int i = 0; i<30; i++)
            CCLOG("%d_@_id = %f",i,MyApp->car_data[i]);
}
void FX_Play::refrushUI()
{
    startTime = false;
    game_time = 6.0;
    if(MyApp->gk_index == 1)
    {
        game_time = 5.0;
    }
    if(MyApp->gk_index == 2)
    {
        game_time = 4.0;
    }
    
    Text* txt1 = (Text*)rootNode->getChildByTag(163);
    char str[64] = {0};
    sprintf(str,"%.1f",game_time);
    txt1->setString(str);
    
    
    rootNode->getChildByTag(160)->setVisible(false);
    rootNode->getChildByTag(188)->setVisible(false);
    
    press_word_num = 0;
    press_count = 0;
    
    Text* txt0 = (Text*)rootNode->getChildByTag(188)->getChildByTag(189);
    txt0->setString(str_question[MyApp->gk_index][MyApp->question_index]);
    
    int number=0;
    int *temp=new int[word_num];
    for(int k=0;k<word_num;k++)
        temp[k]=k;
    
    for(int k=0;k<word_num;k++)
    {
        number=arc4random()%word_num;
        while(temp[number]==-1)
        {
            number=arc4random()%word_num;
        }
        world_index[k] = temp[number];
        CCLOG("world_index[k] = %d",world_index[k]);
        temp[number]=-1;
    }
    delete [] temp;
    temp = NULL;
    
    for (int i = 0; i<word_num; i++)
    {
        Button *btnWord = (Button*)rootNode->getChildByTag(160)->getChildByTag(300+i);
        btnWord->setVisible(true);
        
        btnWord->loadTextureDisabled("btn_select.png");
        btnWord->setEnabled(true);
        
        Text* txt1 = (Text*)rootNode->getChildByTag(160)->getChildByTag(300+i)->getChildByTag(10);
        txt1->setString(str_answer[MyApp->gk_index][MyApp->question_index][i]);
        
        btnWord->setPosition(Vec2(game_xxx[world_index[i]],game_yyy[world_index[i]]));
    }

    Sprite *startGame0 = Sprite::create("txt_ready.png");
    startGame0->setPosition(Vec2(772, SCREEN_H-550));
    this->addChild(startGame0,100);
    MoveBy* act0 = MoveBy::create(0.2, Vec2(-(SCREEN_W/2+132),0));
    DelayTime* act1 = DelayTime::create(0.5);
    MoveBy* act2 = MoveBy::create(0.2, Vec2(-(SCREEN_W/2+132),0));
    CallFuncN* act3 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over1,this));
    RemoveSelf* act4 = RemoveSelf::create();
    startGame0->runAction((Sequence*)Sequence::create(act0,act1,act2,act3,act4,NULL));
}
void FX_Play::dealResult()
{
    int result_count = 14;
    if(word_num == 5)
        result_count = 30;
    if(word_num == 6)
        result_count = 55;
    
    if(press_count == result_count)
    {
        MyApp->playSfx("sound/right.mp3");
        MyApp->game_win = true;
        for (int i = 0; i<word_num; i++)
        {
            Button *btnWord = (Button*)rootNode->getChildByTag(160)->getChildByTag(300+i);
            btnWord->loadTextureDisabled("btn_ok.png");
        }
    }
    else
    {
        MyApp->playSfx("sound/wrong.mp3");
        MyApp->game_over = true;
        for (int i = 0; i<word_num; i++)
        {
            Button *btnWord = (Button*)rootNode->getChildByTag(160)->getChildByTag(300+i);
            btnWord->loadTextureDisabled("btn_woring.png");
        }
    }
    
    DelayTime* act1 = DelayTime::create(1);
    CallFuncN* act2 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over2,this));
    DelayTime* act3 = DelayTime::create(2);
    CallFuncN* act4 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over3,this));

    this->runAction((Sequence*)Sequence::create(act1,act2,act3,act4,NULL));
}
void FX_Play::updateTime(float dt)
{
}
void FX_Play::goBossFight()
{}
void FX_Play::goNormalFight()
{}
void FX_Play::videoPlayed(Ref* rtn)
{}
void FX_Play::closeDialogCallback(Node *pNode)
{
    if (pNode->getTag() == 27)//战机到达10级
    {
        goNormalFight();
    }
    if (pNode->getTag() == 28)//拥有2架以上战机
    {
        menuRole(NULL);
    }
}
void FX_Play::aniCallback_menu(Armature * armature, MovementEventType type, const std::string& movementID)
{
    if (type == COMPLETE)
    {
        if(movementID.compare("efx_main_begin") == 0)
        {
            armature->getAnimation()->play("efx_main_ing", -1, 1);
        }
    }
}
void FX_Play::eventButton(Ref* pSender,Widget::TouchEventType type)
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
void FX_Play::eventButton2(Ref* pSender,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
        ((Node*)pSender)->setColor(Color3B(255,169,20));
    if (type == Widget::TouchEventType::CANCELED)
        ((Node*)pSender)->setColor(Color3B(255,255,255));
    if (type == Widget::TouchEventType::ENDED)
    {
        ((Node*)pSender)->setColor(Color3B(255,255,255));
        MyApp->playSfx("sound/click.aif");
    }
}
void FX_Play::menuRole(Ref* sp)
{

}

void FX_Play::updateYun(float dt)
{
    if(MyApp->game_over)
        return;
    if(MyApp->game_win)
        return;
    if(!startTime)
        return;
    
    game_time-=0.1;
    
    Text* txt0 = (Text*)rootNode->getChildByTag(163);
    char str[64] = {0};
    sprintf(str,"%.1f",game_time);
    txt0->setString(str);
    
    if(game_time < 0.1)
    {
        MyApp->game_over = true;
        startTime = false;
        FX_Teach* teach_layer = FX_Teach::createNode(20);
        this->addChild(teach_layer,10000);
    }
//
//    if(car_num == 0)
//    {
//        if(game_time>=MyApp->car_data[5])
//        {
//            game_time = 0;
//            addCar();
//        }
//    }
//    if(car_num == 1)
//    {
//        if(game_time>=MyApp->car_data[6])
//        {
//            game_time = 0;
//            addCar();
//        }
//    }
//    if(car_num >= 2)
//    {
//        if(game_time>=MyApp->car_data[7])
//        {
//            game_time = 0;
//            addCar();
//        }
//    }
}

void FX_Play::updateRolePosition(float dt)
{

}
void FX_Play::updateGame(float dt)
{
    if(MyApp->game_over)
        return;
    
//    Vec2 center = Vec2(SCREEN_W/2,SCREEN_H-332);
//    
//    role_angle+=role_speed;
//    if(role_angle>=360)
//    {
//        role_angle = 0;
//        
//        MyApp->play_num++;
//        char t_10[64] = {0};
//        sprintf(t_10,"%d",MyApp->play_num);
//        ((LabelAtlas*)this->getChildByTag(100))->setString(t_10);
//    }
//    float offx = RR*sin((M_PI/180)*(role_angle));
//    float offy = RR*cos((M_PI/180)*(role_angle));
//    
//    sp_role->setPosition(Vec2(center.x+offx,center.y+offy));
//    sp_role->setRotation(90+role_angle);
//    
//    Vector<Node*> vEnemy = play_layer->getChildren();
//    if(!vEnemy.empty())
//    {
//        for (int i = 0; i<vEnemy.size(); i++)
//        {
//            FX_Sp* enemy = (FX_Sp*)vEnemy.at(i);
//            if(enemy->isVisible()&&!enemy->show_ing&&!enemy->disappear_ing)
//            {
//                Rect role_rect = Rect(sp_role->getPosition().x-15, sp_role->getPosition().y-35, 30, 70);
//                Rect enemy_rect = Rect(enemy->getPosition().x-15, enemy->getPosition().y-35, 30, 70);
//
//                if(enemy_rect.intersectsRect(role_rect))
//                {
//                    if(!MyApp->game_over)
//                    {
//                        MyApp->game_over = true;
//                        
//                        CallFuncN* act0 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over0,this));
//                        DelayTime* act1 = DelayTime::create(2);
//                        CallFuncN* act2 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over1,this));
//                        this->runAction((Sequence*)Sequence::create(act0,act1,act2,NULL));
//                    }
//                }
//
//            }
//        }
//    }
//    
//    if(!vEnemy.empty())
//    {
//        for (int i = 0; i<vEnemy.size(); i++)
//        {
//            FX_Sp* enemy = (FX_Sp*)vEnemy.at(i);
//            if(!enemy->isVisible())
//            {
//                enemy->stopAllActions();
//                play_layer->removeChild(enemy, true);
//            }
//        }
//    }
}

void FX_Play::move_over3(Node* pSender)
{
    if(MyApp->game_win)
    {
        startTime = false;
        MyApp->game_win = false;
        MyApp->question_index++;
        
        if(MyApp->question_index == 6)
        {
            if (MyApp->gk_index<2)
            {
                MyApp->role_open[MyApp->gk_index+1] = true;
                
                char s2[64] = {0};
                sprintf(s2,"role_open%d",MyApp->gk_index+1);
                MyApp->saveGameData_bool(s2, MyApp->role_open[MyApp->gk_index+1]);
                
                FX_Teach* teach_layer = FX_Teach::createNode(21);
                this->addChild(teach_layer,10000);
                
                showVideo();
            }
            else
            {
                FX_Teach* teach_layer = FX_Teach::createNode(21);
                this->addChild(teach_layer,10000);
                
                showVideo();
            }

        }
        else
        {
            refrushUI();
        }
    }
    if(MyApp->game_over)
    {
        startTime = false;
        MyApp->game_over = false;
        FX_Teach* teach_layer = FX_Teach::createNode(20);
        this->addChild(teach_layer,10000);
        
        showVideo();
    }
}
void FX_Play::addHitEfx(int type,float x,float y)
{
    if(type == 0)
    {
        Sprite *temp_efx=createCache("efx_hit0.png");
        temp_efx->setPosition(Vec2(x,y));
        this->addChild(temp_efx,800);
        Animation* temp_act = loadAct_x("efx_hit",3,0.05,0);
        Animate *act0 = Animate::create(temp_act);
        CallFuncN* act1 = CallFuncN::create(CC_CALLBACK_1(FX_Play::act_over,this));
        temp_efx->runAction((Sequence*)Sequence::create(act0,act1, NULL));
    }
}
void FX_Play::act_over(Node* pSender)
{
    this->removeChild(pSender,true);
}
Animation* FX_Play::loadAct_x(const char* str,int frameCount,float delay,int index)
{
    Vector<SpriteFrame*> array;
    for(int i = 0; i < frameCount; i++)
    {
        char t[32] = {0};
        sprintf(t, "%s%d.png",str,i);
        
        SpriteFrame *frame = spCache(t);
        if (frame == NULL) {
            Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage(t);
            Rect rect = Rect::ZERO;
            rect.size = pTexture->getContentSize();
            frame = SpriteFrame::createWithTexture(pTexture, rect);
            SpriteFrameCache::getInstance()->addSpriteFrame(frame, t);
        }
        array.pushBack(frame);
    }
    return Animation::createWithSpriteFrames(array, delay);
}
void FX_Play::menuXingJi(Ref* pSender)
{

}
void FX_Play::menuHonor(Ref* pSender)
{

}

void FX_Play::menuStart(Ref* pSender)
{}
void FX_Play::menuInfo(Ref* pSender)
{}

void FX_Play::menuShop(Ref* pSender)
{}

void FX_Play::menuLoop(Ref* pSender)
{}

void FX_Play::addBTN_ex(const char* img,Node* toNode,Vec2 xy,Vec2 anchor,const ccMenuCallback selector,bool vis,int z,int tag)
{
    Sprite* normal = createCache((img));
    normal->setColor(Color3B::WHITE);
    Sprite* selected = createCache((img));
    selected->setColor(Color3B(150, 150, 150));
    MenuItemSprite* temp = MenuItemSprite::create(normal,selected,NULL,selector);
    temp->setTag(tag);
    temp->setAnchorPoint(anchor);
    
    Menu* tempButton = Menu::create(temp, NULL);
    tempButton->setPosition(xy);
    tempButton->setVisible(vis);
    toNode->addChild(tempButton,z,tag);
}

void FX_Play::addBTN(const char* img,const char* img_sel,Vec2 xy,const ccMenuCallback selector,bool vis,int z,int tag)
{
    Sprite* normal = createCache((img));
    Sprite* selected = createCache((img_sel));
    MenuItemSprite* temp = MenuItemSprite::create(normal,selected,NULL,selector);
    
    temp->setTag(tag);
	    
    Menu* tempButton = Menu::create(temp, NULL);
    tempButton->setPosition(xy);
    tempButton->setVisible(vis);
    this->addChild(tempButton,z,tag);
}

bool FX_Play::onTouchBegan(Touch *touch,Event *pEvent)
{
    if(role_act_index>0)
        return false;
    Vec2 point = touch->getLocation();
    Director::getInstance()->convertToGL(point);
    
    return true;
}

void FX_Play::onTouchMoved(Touch *touch,Event *pEvent)
{

}

void FX_Play::onTouchEnded(Touch *touch,Event *pEvent)
{
    
}

void FX_Play::move_over0(Node* pSender)
{
    
    MyApp->playSfx("sound/game_over.mp3");

}
void FX_Play::move_over1_0(Node* pSender)
{
    rootNode->getChildByTag(160)->setVisible(true);
    rootNode->getChildByTag(188)->setVisible(true);
    startTime = true;
}
void FX_Play::move_over1(Node* pSender)
{
    Sprite *startGame0 = Sprite::create("txt_go.png");
    startGame0->setPosition(Vec2(772, SCREEN_H-550));
    this->addChild(startGame0,100);
    MoveBy* act0 = MoveBy::create(0.2, Vec2(-(SCREEN_W/2+132),0));
    DelayTime* act1 = DelayTime::create(0.5);
    MoveBy* act2 = MoveBy::create(0.2, Vec2(-(SCREEN_W/2+132),0));
    CallFuncN* act3 = CallFuncN::create(CC_CALLBACK_1(FX_Play::move_over1_0,this));
    RemoveSelf* act4 = RemoveSelf::create();
    startGame0->runAction((Sequence*)Sequence::create(act0,act1,act2,act3,act4,NULL));

    
//    Vector<Node*> vEnemy = play_layer->getChildren();
//    if(!vEnemy.empty())
//    {
//        for (int i = 0; i<vEnemy.size(); i++)
//        {
//            FX_Sp* enemy = (FX_Sp*)vEnemy.at(i);
//            enemy->stopAllActions();
//            play_layer->removeChild(enemy, true);
//        }
//    }
//    
//    if(MyApp->play_num>MyApp->info_best_num)
//    {
//        MyApp->info_best_num = MyApp->play_num;
//        MyApp->saveGameData_int("info_best_num", MyApp->info_best_num);
//        
//        uploadLoop1(MyApp->info_best_num);
//    }
//    
//    char t_10[64] = {0};
//    sprintf(t_10,"%d",MyApp->play_num);
//    ((LabelAtlas*)over_layer->getChildByTag(10))->setString(t_10);
//    
//    char t_11[64] = {0};
//    sprintf(t_11,"%d",MyApp->info_best_num);
//    ((LabelAtlas*)over_layer->getChildByTag(11))->setString(t_11);
//    
//    over_layer->setVisible(true);
//    
    
}
void FX_Play::showVideo()//要改
{

}
void FX_Play::move_over2(Node* pSender)
{
    for (int i = 0; i<word_num; i++)
    {
        Button *btnWord = (Button*)rootNode->getChildByTag(160)->getChildByTag(300+i);
        btnWord->runAction(MoveTo::create(0.5, Vec2(game_xxx[i],game_yyy[i])));
    }
}
void FX_Play::onEnterTransitionDidFinish()
{
    this->schedule(schedule_selector(FX_Play::updateGame));
    this->schedule(schedule_selector(FX_Play::updateYun),0.13);
    this->schedule(schedule_selector(FX_Play::updateRolePosition));
    
    
    refrushUI();

    Layer::onEnterTransitionDidFinish();
}

void FX_Play::onExit()
{
    CCLOG("exit");

    this->unscheduleAllCallbacks();
    this->removeAllChildrenWithCleanup(true);
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    Layer::onExit();
}
