#include "Add_function.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameHelper.h"


using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

Add_function::~Add_function()
{
    
}


Add_function::Add_function()
{

}

void Add_function::initFadeTo(Node* node, float t)
{
    FadeTo* a=FadeTo::create(0, 0);
    DelayTime* d =DelayTime::create(t);
    FadeTo* a1=FadeTo::create(1.0f, 255.0f);
    Sequence * l =(Sequence * )Sequence::create(a, d, a1, NULL);
    node->runAction(l);
}

void Add_function::setTextureByFile(Sprite* node, const char *pszFilename)
{
    if(node){
        Texture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(pszFilename);
        if( pTexture ) {
            Rect rect = Rect::ZERO;
            rect.size = pTexture->getContentSize();
            node->setTexture(pTexture);
            node->setTextureRect(rect);
        }
    }
}

Node* Add_function::setNodeTextureByFile(Node* node, const char *pszFilename)
{
    if(node){
        Node* change=Sprite::create(pszFilename);
        change->setPosition(node->getPosition());
        node->getParent()->addChild(change, node->getLocalZOrder());
        node->removeFromParentAndCleanup(true);
        node=change;
    }
    return node;
}

void Add_function::setBackgroundScale(Node* node)
{
    float width = node->getContentSize().width;
    float height = node->getContentSize().height;
    if (height/width < LPRatio) {
        node->setScale(screenSize.height/height);
    } else {
        
        node->setScale(screenSize.width/width);
    }
}

SpriteFrame* Add_function::getSpriteFrame(char* filename){
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    
    if (frame == NULL) {
        Texture2D *pTexture = TextureCache::sharedTextureCache()->addImage(filename);
        Rect rect = Rect::ZERO;
        rect.size = pTexture->getContentSize();
        frame = SpriteFrame::createWithTexture(pTexture, rect);
        SpriteFrameCache::getInstance()->addSpriteFrame(frame, filename);
    }
    
    return frame;
}

Animation* Add_function::createWithFileName(const char *framePrefix, float delayPerUnit, int framesNumber, unsigned int loops, int startIndex)
{
    Animation *animation = Animation::create();
    for (int i = startIndex; i < startIndex + framesNumber; i++) {
        const char * name =CCString::createWithFormat("%s%d.png", framePrefix, i)->getCString();
        animation->addSpriteFrameWithFileName(name);
    }
    animation->setDelayPerUnit(delayPerUnit);
    animation->setLoops(loops);
    return animation;
}

void Add_function::setTime(LabelAtlas* label, float time){
    int m=time/60;
    int s=time-m*60;
    if(m<10){
        if(s<10){
            char number[200] = {0};
            sprintf(number,"0%d/0%d", m,s);
            label->setString(number);
        }
        else{
            char number[200] = {0};
            sprintf(number,"0%d/%d", m,s);
            label->setString(number);
        }
    }
    else{
        if(s<10){
            char number[200] = {0};
            sprintf(number,"%d/0%d", m,s);
            label->setString(number);
        }
        else{
            char number[200] = {0};
            sprintf(number,"%d/%d", m,s);
            label->setString(number);
        }
    }
}

void Add_function::initRock(Node* node)
{
    RotateTo* a1=RotateTo::create(0.02f, 15);
    RotateTo* a2=RotateTo::create(0.02f, 0);
    RotateTo* a3=RotateTo::create(0.02f, -15);
    RotateTo* a4=RotateTo::create(0.02f, 0);
    Sequence * l1 =(Sequence * )Sequence::create(a1, a2, a3, a4, NULL);
    Repeat* r1=Repeat::create(l1, 5);
    node->runAction(r1);
}

Point Add_function::get_center(Point p1, Point p2)
{
    return Point((p1.x+p2.x)/2.0f,(p1.y+p2.y)/2.0f);
}

void Add_function::playsound_delay(float d, char* name, Node* node)
{
    soundname=name;
    DelayTime* d1=DelayTime::create(d);
    CallFunc * f1=CallFunc::create(this, callfunc_selector(Add_function::playsound));
    Sequence * l =(Sequence * )Sequence::create(d1, f1, NULL);
    node->runAction(l);
}

void Add_function::playsound()
{
    SimpleAudioEngine::getInstance()->playEffect(soundname);
}
bool Add_function::pixelIsNullEx(Sprite* sprite,Point screen_p){
    Point pos_in_sprite = sprite->convertToNodeSpace(screen_p);
    unsigned char data[4] = {0};
    RenderTexture* canvas = RenderTexture::create(1, 1, Texture2D::PixelFormat::RGBA8888);
    Point pre_pos = sprite->getPosition();
    Point pre_anchor = sprite->getAnchorPoint();
    
    canvas->begin();
    sprite->setPosition(Point::ZERO);
    sprite->setAnchorPoint(Point(pos_in_sprite.x/sprite->getContentSize().width,pos_in_sprite.y/sprite->getContentSize().height));
    sprite->visit();
    glReadPixels(0,0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    canvas->end();
    sprite->setPosition(pre_pos);
    sprite->setAnchorPoint(pre_anchor);
    
//    printf("X:%.0f y:%.0f R: %u, G: %u, B: %u, A: %u\n",pos_in_sprite.x ,pos_in_sprite.y, data[0], data[1], data[2], data[3]);
    
    
    if(data[0] != 0 || data[1] != 0 || data[2] != 0 || data[3] != 0)
    {
        return false;
    }
    
    return true;
}

bool Add_function::PixelIsNull(Sprite* sprite, Point p){
    
    
    Point location =  Point(p.x*CC_CONTENT_SCALE_FACTOR()*sprite->getScaleX(), p.y*CC_CONTENT_SCALE_FACTOR()*sprite->getScaleY());
    int nw=sprite->getContentSize().width*sprite->getScaleX()*CC_CONTENT_SCALE_FACTOR();
    int nh=sprite->getContentSize().height*sprite->getScaleY()*CC_CONTENT_SCALE_FACTOR();
    Rect rc(0,0,nw,nh);
    
    if(rc.containsPoint(location)){
        
        char data[4] = {0};
        
        RenderTexture* renderTexture = RenderTexture::create(/*sprite->getContentSize().width*sprite->getScaleX()*scale, sprite->getContentSize().height*sprite->getScaleY()*scale,*/1,1, Texture2D::PixelFormat::RGBA8888);
        
        Point p1=sprite->getPosition();
        Point ap=sprite->getAnchorPoint();
        float rotate=sprite->getRotation();
        
//        sprite->setScaleX(sprite->getScaleX()*scale);
//        sprite->setScaleY(sprite->getScaleY()*scale);
        sprite->setAnchorPoint(Point(0,0));
        sprite->setPosition(Point(-location.x,-location.y));
        sprite->setRotation(0);
        
        
        
        renderTexture->begin();
        sprite->visit();
        
        glReadPixels((GLint)location.x,(GLint)location.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        renderTexture->end();
        renderTexture->release();
        
        sprite->setPosition(p1);
        sprite->setAnchorPoint(ap);
//        sprite->setScaleX(sprite->getScaleX()/scale);
//        sprite->setScaleY(sprite->getScaleY()/scale);
        sprite->setRotation(rotate);
        
        
        printf("X:%.0f y:%.0f R: %u, G: %u, B: %u, A: %u\n",location.x ,location.y, data[0], data[1], data[2], data[3]);
        
        
        if(data[0] != 0 || data[1] != 0 || data[2] != 0 || data[3] != 0)
        {
            return false;
        }
    }
    return true;
}


void Add_function::set_MenuItemImage_SelectedImage_Scale(MenuItemImage* button, float s){
    button->getSelectedImage()->setScale(s);
    ((Sprite*)button->getSelectedImage())->setColor(Color3B(150, 150, 150));
    button->getSelectedImage()->setPosition(ccpContent(button->getSelectedImage()->getContentSize(), 100*(1-s)*0.5, 100*(1-s)*0.5));
}

Rect Add_function::get_scale_bound(Sprite* sprite, float scalex, float scaley, float xmove, float ymove){
    
    Rect bound=sprite->getBoundingBox();
    bound.setRect(bound.origin.x+0.5*(1-scalex)*bound.size.width+xmove*bound.size.width, bound.origin.y+0.5*(1-scaley)*bound.size.height+ymove*bound.size.height, bound.size.width*scalex, bound.size.height*scaley);
    
    return bound;
}

void Add_function::removeDelay(Sprite* sprite, float t){
    DelayTime* delay = DelayTime::create(t);
    sprite->runAction(Sequence::create(delay,CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sprite)), NULL));
}



