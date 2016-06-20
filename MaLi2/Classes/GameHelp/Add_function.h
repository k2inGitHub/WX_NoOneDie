

#ifndef _Add_function_h
#define _Add_function_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace std;
typedef std::vector<std::string> strArray;


class Add_function: public cocos2d::Node
{
public:
    
    Add_function();
    ~Add_function();
    
    static void initFadeTo(Node* node, float t);

    static void setTextureByFile(Sprite* node, const char *pszFilename);
    static Node* setNodeTextureByFile(Node* node, const char *pszFilename);
    static void setBackgroundScale(Node* node);

    static Animation* createWithFileName(const char *framePrefix, float delayPerUnit, int framesNumber, unsigned int loops, int startIndex);
    
    static SpriteFrame* getSpriteFrame(char* filename);
    
    static void setTime(LabelAtlas* label, float time);
    
    static void initRock(Node* node);
    
    static Point get_center(Point p1, Point p2);
    
    void playsound_delay(float d, char* name, Node* node);
    void playsound();
    
    static bool pixelIsNullEx(Sprite* sprite,Point screen_p);
    static bool PixelIsNull(Sprite* sprite, Point p);
    
    static void splitByline(std::string src, strArray& vect);
    static void splitByToken(std::string src, const char* token,strArray& vect);
    static int firstPos(std::string src,int start_pos,int* next_pos);
    static void splitByline1(std::string src, strArray &vect);
    
    
    static unsigned char* getFileByName(string pFileName);
    static string getWriteableFileByName(string pFileName);
    static bool saveFile(char *pContent, string pFileName);
    static string searchTxt(char *filepath, char *searchtitle, int searchid);

    static void set_MenuItemImage_SelectedImage_Scale(MenuItemImage* button, float s);
    
    static Rect get_scale_bound(Sprite* sprite, float scalex, float scaley, float xmove, float ymove);
    
    
    static void removeDelay(Sprite* sprite, float t);
    
public:
    char* soundname;


private:


};

#endif
