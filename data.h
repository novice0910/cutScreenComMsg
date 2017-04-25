#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QWidget>
#include <QImage>
#include <QMultiMap>
//#include <QCoreApplication>
#define CONFIGINI "config.ini"
#define MAX_PAGE_NUM 3
#define RUN_PATH QCoreApplication::applicationDirPath()

//寄存器地址
#define PIC_ID_0 0X03   //界面ID
#define PIC_ID_1 0x04
#define KEY_CODE 0x4F   //键值
#define RTC_COM_ADJ 0x1f //set time flag
#define RTC_NOW 0x20
#define RTC_NOW_TIME 0x24
#define LED_STA 0x1E   //背景亮度
//键值
#define MSGBOX_HIDE 0x06 //msgBox

//显示类型
enum DATA_TYPE{
    INT,BCD,CARD_NUMBER,CHAGER_LENGTH,ASCII
};
enum DATA_LENGTH{
    ONE_BYTE,TWO_BYTE,FOUR_BYTE
};
enum MSGBOX_NUM{
    MSGBOX_1 =1,MSGBOX_2,MSGBOX_3,MSGBOX_4,MSGBOX_5
    ,MSGBOX_6,MSGBOX_7,MSGBOX_8,MSGBOX_9,MSGBOX_10
    ,MSGBOX_11
};
//enum REG_ADDR{
//    Version,LED_SET,BZ_TIME,PIC_ID_0,PIC_ID_1,TP_Flag,TP_Status,TP_Position,
//    TPC_Enable,RUN_TIME_0,RUN_TIME_1,RUN_TIME_2,RUN_TIME_3,
//    R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,RA,RB,RC
//};

struct BTN_INFO
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int x;
    int y;
    int w;
    int h;
    QImage image;
    int regesitAddr;
    int regesitData;
    quint8 regDataOffset;//寄存器数据偏移量
    int dataStartAddr;//两个字节
    int dataDataLength;//最大两个字节
    int dataData;//最大两个字节
    quint8 dataType;//数据类型
    quint8 dataDataOffSet;//数据偏移量
};

struct NUM_KEYBOARD_INFO
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int x;
    int y;
    int w;
    int h;
    QString image;
    int dataAddr;   //数据储存区地址
    int dataDataLength;//数据储存区长度
    int dataType;//数据格式
    quint8 dataIntegerLength;//整数部分长度
    quint8 dataDecimalsLength;//小数部分长度 小数位数等于偏移量大小
};
struct SETTIME_KEY_INFO
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int x;
    int y;
    int w;
    int h;
    QString image;
    int dataAddr;   //数据储存区地址
    char *dataData; //数据储存区数据
    int dataDataLength;//数据储存区长度
    int dataType;//数据格式
    quint8 keyboardSum;
};
struct PASS_BTN_INFO
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int x;
    int y;
    int w;
    int h;
    QString image;
};

struct LAB_INFO
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int x;
    int y;
    int w;
    int h;
    QString image;
    int dataAddr;//数据寄存器地址
    int dataLength;//显示的数据长度
    int dataOffset;//数据偏移量
    int dataType;//数据类型
};

typedef struct
{
    QString ojName;
    QString string;
}INFO_LAB_SHOW;

struct MESSAGEBOX_INFO
{
    int page;//所属页面
    QImage image;
    QWidget *parent;//父窗体
    QString ojName;
    int keyNum;//键值 其所对的界面
    int x;
    int y;
    int w;
    int h;
};
typedef struct
{
    int page;//页码
    QWidget *parent;//父窗体
    QString ojName;
    int lineEdit_x;
    int lineEdit_y;
    int lineEdit_w;
    int lineEdit_h;
    //lineEdit 关联的数据信息
    int dataStartAddr;// 起始地址 长度为占几个字节
    int dataDataLength;//
    quint8 dataType;//数据类型
    quint8 dataIntegerLength;//整数部分长度
    quint8 dataDecimalsLength;//小数部分长度 小数位数等于偏移量大小
    int dataData;//数据 最大两个字节 lineEdit 内的数据为要输入的数据
    int key_x;
    int key_y;
    bool isPass;//is password keyboard?
//    int key_w;
//    int key_h;
    quint8 keyboardSum;
}KEYBOARD;

typedef QList<BTN_INFO> BTN_INFO_LIST;
typedef QList<LAB_INFO> LAB_INFO_LIST;
typedef QList<NUM_KEYBOARD_INFO> NUM_KEYBOARD_INFO_LIST;
typedef QList<MESSAGEBOX_INFO> MSGBOX_INFO_LIST;
typedef QList<INFO_LAB_SHOW> LAB_SHOW_LIST;
typedef struct{
    int pageNum;
    BTN_INFO_LIST currentPageBtnInfo;//当前页所有按键信息
    LAB_INFO_LIST currentPageLabInfo;//当前页所有显示label信息
    NUM_KEYBOARD_INFO_LIST curPageNumKbInfo;//当前页所有数字按键的信息
    MSGBOX_INFO_LIST curPageMsgBoxInfo;//当前页 msgBox的信息
    SETTIME_KEY_INFO curSetTimeInfo;//当前页 设置时间按键
    KEYBOARD curKeyboardInfo;//当前页keyboard信息
}PAGE_INFO;
/** 功能:int 转压缩BCD
 *    示例: 34 => 0x34, 71 => 0x71
 *    说明: int 数据不能超过99，范围0<= i <= 99;
 */
#define INTTOBCD(i)  ((((i)/10) << 4)+((i)%10))

/** 功能:压缩BCD转int
*     示例:  0x35 => 35;  0x10 => 10;
*     说明:注意只能转换1个字节的
*/
#define BCDTOINT(i)  ((((i)&0xf0) >>4)*10+((i)&0x0f))
#define QREGISTERMETATYPE()  {\
    qRegisterMetaType< PAGE_INFO >("PAGE_INFO");\
    qRegisterMetaType< LAB_SHOW_LIST >("LAB_SHOW_LIST");\
    qRegisterMetaType< BTN_INFO_LIST >("BTN_INFO_LIST");\
    qRegisterMetaType< LAB_INFO_LIST >("LAB_INFO_LIST");\
    qRegisterMetaType< NUM_KEYBOARD_INFO_LIST >("NUM_KEYBOARD_INFO_LIST");\
    qRegisterMetaType< MSGBOX_INFO_LIST >("MSGBOX_INFO_LIST");\
    qRegisterMetaType< SETTIME_KEY_INFO >("SETTIME_KEY_INFO");\
    qRegisterMetaType< MESSAGEBOX_INFO >("MESSAGEBOX_INFO");\
    qRegisterMetaType< KEYBOARD >("KEYBOARD");\
    }
#endif

