////wpfinterface.h:wpf文件格式操作接口
////wpf文件格式简介:wpf以Dos\Windows中的Fat32文件系统为参照,根据实际需要有一些修改.
////1.经测试如果每块(扇区)512字节格式化,传世中平均每个文件寻址多达40次,耗时0.048ms,此外一次性读取20K(传世图片平均大小)内容的效率要比分40次每次读取512字节高1倍以上,另外
////  如果用非连续存储传世中现有的很多接口要重写,wpf中的采用连续存储,在新加文件时必须分配连续的块(扇区),如果找不到则以EXPEND_LEN为单位扩展整个图包大小,
////  这种方式的缺点是容易产生碎片,但传世中删除或更改文件很少,大多数都是新加,再者删除文件留下的空间可以被后面新加的文件利用,有很多按钮文件本身都小于512字节,总的来说碎片不会很多,此外还提供了
////  碎片整理接口,在发现碎片太多的时候可以进行碎片整理,以提高空间利用率,一般情况下磁盘利用率为95%左右
////  这种方式的缺点就是容易产生碎片以及存储前要知道文件的大小,但索引分配及链式分配都对性能有极大影响,鉴于我们存储前一般都知道文件大小,并且外面版本的删除及修改操作很少,所以选用了连续分配
////2.FCB(文件控制块)结构中去掉时间等字段,把文件名扩展到32字节(包含扩展名)
////3.目录看成一个文件,其中内容为其子目录及子文件的FCB,20万个文件及目录大概要13.7M的空间存储相关信息
////4.仿操作系统内存管理机制,管理空闲磁盘空间
////
////结构如下:
////              
////         S_WpfHeader文件头
////-------------------------------
////           文件数据
////-------------------------------
////           空闲块描述信息所在位置
////-------------------------------
////           FCB List  
//
//
////#pragma once
//
////#ifndef _WINDOWS_
////#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
////#include <windows.h>
////#endif
//
////#include <map>
////#include <string>
//
//#define  WPF_MAGIC           'WPF\x01'
//#define  MAX_FILE_NAME_LEN   256
//
//enum E_FCTATTR
//{
//	EFA_BLANK              = 0,        //空白
//	EFA_DIR                = 0x0001,   //目录
//	EFA_FILE               = 0x0002,   //文件
//	EFA_COMPRESS           = 0x0004,   //拥有此属性的文件为压缩文件,压缩类型由文件头中的byCompress指定
//	EFA_CRYPT              = 0x0008,   //拥有此属性的文件为加密文件,压缩类型由文件头中的byCrypt指定
//};
////用哪一个文件指针打开文件,一个wpf文件支持多个线程同时操作
////所有有关wpf的编辑操作都使用EFT_MAIN指针,只读操作可以随便同时使用所有指针
//enum E_FPTYPE
//{
//	EFT_MAIN      = 0,  //主线程
//	EFT_THREAD    = 1,  //其它线程
//	EFT_NUM,
//};
//
////wfp操作返回代码
//enum E_WPF_RTN
//{
//	EWR_OK                    = 0,   //成功
//	EWR_ERROR,                       //失败
//	EWR_PARA_ERROR,                  //参数错误
//	EWR_FILE_NOT_OPEN,               //wpf文件没有打开
//	EWR_FILE_EXIST,                  //文件已经存在
//	EWR_FILE_NOT_EXIST,              //文件已经存在
//	EWR_FILE_SAVE_FAIL,              //文件保存失败
//	EWR_FILE_OPEN_FAIL,              //文件打开失败
//	EWR_NOT_WPF_FILE,                //文件格式错误
//	EWR_DIR_EXIST,                   //目录已经存在
//	EWR_DIR_NOT_EXIST,               //目录不存在
//	EWR_PATH_INVALID,                //文件名或路径非法
//	EWR_FILE_SAVING,                 //正在保存文件,此时的写操作将会被拒绝,为了性能,设计上不允许多线程同时进行写操作
//
//	EWR_UNKNOWN,                     //其它未知错误代码
//};
//
////wpf的打开方式
//enum E_WPF_OPENTYPE
//{
//	EWOT_READONLY      = 0x0001,         //只读,这种模式不会构造目录树,查找文件是按路径的hash值查找,速度快,省内存
//	EWOT_RDWR          = 0x0002,         //读写模式,要求对文件或目录添加,删除,修改等操作使用此模式,写操作使用EFT_MAIN文件指针,有此属性时各文件操作函数非线程安全,由外部保证线程安全
//	EWOT_LISTDIR       = 0x0004,         //列出目录,EWOT_RDWR包含EWOT_LISTDIR
//};
//
//#pragma pack (4)
//
////fcb第1部分,读操作时用到,为了省内存和fcb2分开
//struct S_Fcb1
//{
//	S_Fcb1()
//	{
//		dwStart = 0;
//		dwSize = 0;
//	}
//
//	DWORD   dwStart;        //如果是文件表示该文件的起始块号,如果是目录表示该目录的文件目录的起始FCB编号
//	DWORD   dwSize;         //对于文件表示文件长度，以字节为单位,对于目录表示该目录中的FCB项数
//	__int64 iHashKey;       //该文件全路径的hash值
//};
//
////fcb第2部分,编辑操作时用到
//struct S_Fcb2
//{
//	S_Fcb2()
//	{
//		memset(strName,0,MAX_FILE_NAME_LEN);
//		memset(strMd5,0,16);
//		dwAttribute = EFA_BLANK;
//		dwRev = 0;
//	}
//
//	char  strName[MAX_FILE_NAME_LEN];    //文件名加扩展名,目录则为目录名
//	char  strMd5[16];                    //文件MD5码
//	DWORD dwAttribute;                   //属性E_FCTATTR类型
//	DWORD dwRev;                         //当拥有EFA_COMPRESS属性时表示压缩之前的大小
//};
//
//
////wpf头结构,64字节,2份共128字节
//struct S_WpfHeader
//{
//	S_WpfHeader()
//	{
//		dwMagic = WPF_MAGIC;
//		wHeaderSize = sizeof(S_WpfHeader) * 2;//文件头及文件头备份
//		iWpfSize = 0;
//		iBlankBlockPos = wHeaderSize;
//		dwBlankBlockSize = 0;
//		dwDirCount = 1;//默认有一个根目录
//		dwFileCount = 0;
//		dwTotalBlocks = 0;
//		wBytesPerBlock = 128;
//		byRev1 = 0;
//		byCompress = 0;
//		byCrypt = 0;
//		byRev2 = 0;	
//
//		memset(dwRev,0,sizeof(BYTE) * 2);
//		memset(byRev3,0,sizeof(BYTE) * 3);
//
//		bySavingHeader = TRUE;
//
//		//memcpy(byRev2 + 64 - strlen("Design:何国辉,shandagames"),"Design:何国辉,shandagames",strlen("Design:何国辉,shandagames"));
//	}
//
//	DWORD     dwMagic;              //魔数,固定为'WPF\x01'
//	WORD      wHeaderSize;          //头的大小,也是第一个文件或目录的起始位置
//	WORD      wBytesPerBlock;       //每个块的字节数
//
//	__int64   iBlankBlockPos;       //空闲块描述信息所在位置
//	DWORD     dwBlankBlockSize;     //有多少条空闲块信息(起始位置1,大小1,起始位置2,大小2...)DWORD
//	__int64   iFatPos;              //文件分配表所在位置
//	DWORD     dwDirCount;           //目录总数
//	DWORD     dwFileCount;          //文件个数
//	DWORD     dwTotalBlocks;        //划分的总块数
//	__int64   iWpfSize;             //整个wpf文件的大小,不包含头,空闲块描述信息及文件分配表
//	BYTE      byRev1;               //由于历史原因,此字节作废
//	BYTE      byCrypt;              //加密类型
//	BYTE      byCompress;           //压缩类型
//	BYTE      byRev2;               //保留
//	DWORD     dwRev[2];             //保留
//	BYTE      byRev3[3];            //保留
//	BYTE      bySavingHeader;       //是否正在保存文件头,为TRUE表示上次没有保存文件头成功,失败后下次读取时会读取上次的文件头备份,机制可以保证失败后原来的fcblist不被覆盖,重打版本可以修复,备份头写失败下次会重写备份头,不可能两份头的bySavingHeader都为TRUE,除非创建的时候就失败了,那么就不是一个wpf文件,这种情况打开的时候会返回失败
//};
//#pragma pack ()
//
//typedef struct S_WpfHeader WpfHeader;
//typedef struct S_Fcb1   FCB1;
//typedef struct S_Fcb1*  PFCB1;
//typedef struct S_Fcb2   FCB2;
//typedef struct S_Fcb2*  PFCB2;
//
////wpf文件列表结构
//struct S_WpfFileList;
//typedef struct S_WpfFileList WpfFileList;
//typedef struct S_WpfFileList* PWpfFileList;
//typedef std::map<std::string,PWpfFileList> MList;
//
//
//// fcb记录结构
//struct S_Fcb
//{ 
//	S_Fcb()
//	{
//		pfcb1 = NULL;
//		pfcb2 = NULL;
//		pParent = NULL;
//	}
//
//	PFCB1 pfcb1;          //fcb第一部分
//	PFCB2 pfcb2;          //fcb第二部分
//	PWpfFileList pParent; //父目录	
//};
//
//typedef struct S_Fcb  FCB;
//typedef struct S_Fcb* PFCB;
//typedef std::map<std::string,PFCB> MSFCB;
//typedef std::map<__int64,PFCB1> MIFCB1;
//
////目录结构
//struct S_WpfFileList
//{
//	S_WpfFileList()
//	{
//		pfcb = NULL;
//	}
//
//	PFCB        pfcb;   //该目录或文件的fcb
//	MList       MDirs;  //该目录下的子目录列表
//	MSFCB       MFiles; //该目录下的子文件列表
//};