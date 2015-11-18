//#pragma once
//
////#include "wpfinterface.h"
//
//#define  FCB1_SIZE           sizeof(S_Fcb1)
//#define  FCB2_SIZE           sizeof(S_Fcb2)
//#define  FCB_SIZE            (FCB1_SIZE + FCB2_SIZE)
//
//#define  MAX_WPF_HASH_BUFFF_SIZE   0x500 //用来计算hash值
//#define  BLOCK_SIZE 256*1024   //分块读写文件一块的大小
//
////新建文件夹信息,用于最后关闭时释放内存
//typedef struct NEW_DIR
//{
//	NEW_DIR()
//	{
//		pList = NULL;
//		pNext = NULL;
//	}
//
//	PWpfFileList pList;
//	NEW_DIR * pNext;
//}NEWDIR;
//
//typedef struct NEW_FCB
//{
//	NEW_FCB()
//	{
//		pfcb = NULL;
//		pNext = NULL;
//	}
//
//	PFCB pfcb;
//	NEW_FCB * pNext;
//}NEWFCB;
////空闲块描述信息结构
//typedef struct S_BlankBlock
//{
//	DWORD dwStart;//起始位置
//	DWORD dwSize;//大小
//}BlankBlock,*PBlankBlock;
//
////空闲块描述信息列表结构
//typedef struct S_BlankBlockNode
//{
//	S_BlankBlockNode()
//	{
//		blankBlock.dwStart = 0;
//		blankBlock.dwSize = 0;
//		pParent = NULL;
//		pNext = NULL;
//	}
//
//	S_BlankBlock  blankBlock;
//	S_BlankBlockNode* pParent;
//	S_BlankBlockNode* pNext;
//}BlankBlockNode,*PBlankBlockNode;
//
//
//struct LoadingFileData
//{
//	// 当前访问文件FCB1
//	PFCB1	m_pOpenFileFcb1[EFT_NUM];
//	PFCB2	m_pOpenFileFcb2[EFT_NUM];
//	// 当前访问文件的文件指针位置,相对于当前打开文件的起始位置
//	DWORD	m_iOpenFilePointPos[EFT_NUM];
//};
//
//typedef std::map<unsigned long, LoadingFileData> LoadingFileDataMap;
//
//class sdWpfLoader
//{
//	friend class __auto_inter_wpf_buffer;
//
//public:
//	sdWpfLoader(void);
//	virtual ~sdWpfLoader(void);
//
//public:
//	//打开一个Wpf文件,其中的目录及文件信息保存到一个MWpfFileList结构,可以通过GetRoot获得该
//	//iOffset:wpf文件内容在strPathName这个文件中的起始位置,如果整个文件就是一个wpf文件,取0
//	virtual bool WpfOpen(const char * strPathName,__int64 iOffset = 0,DWORD dwOpenType = EWOT_READONLY);
//	//关闭文件,清除文件列表
//	virtual void WpfClose();
//	//获得最后一次错误代码
//	virtual const char* GetWpfName();
//	virtual E_WPF_RTN GetLastError();
//	//获得指定wpf文件的文件列表map,Open后有效
//	virtual PWpfFileList GetRoot();
//	//获得文件头
//	virtual WpfHeader *GetHeader();
//	//查找目录
//	virtual PWpfFileList FindDir(const char * strPathName);
//	//查找文件,目录分隔符只能用"\",不能用,"/",因为效率问题,内部不做转换
//	virtual PFCB FindFile(const char * strPathName);
//	//操作一个文件之前要先打开这个文件,像Seek,Read等,就想打个磁盘中的一个文件一样,打开以后文件指针停在这个文件的起始位置
//	//同一时刻只能操作一个文件,如果要多个线程操作同一个wpf文件可以通过不同的文件指针,见E_FPTYPE,一个对象不能同时处理多个wpf图包中的文件,要处理多个自行使用多个对象,在外部管理
//	//FileWrite函数不会自动增大文件,文件的大小在创建时就定了,如果超过文件的最大长度写到文件结束为止
//	//打开一个文件,如果之前打开了其它文件会自动关闭原来的文件,打开一个新文件之前可以不调用FileClose(),目录分隔符只能用"\",不能用,"/",因为效率问题,内部不做转换
//	virtual long FileOpen(const char * strPathName,E_FPTYPE fpType = EFT_MAIN);
//	//关闭当前打开的文件
//	virtual bool FileClose(E_FPTYPE fpType = EFT_MAIN);
//	//在当前打开的文件内寻址,SEEK_END时,dwOffset也取正值,表示的位置为FileLength - dwOffset
//	virtual bool FileSeek(DWORD dwOffset,int iOrigin = SEEK_SET,E_FPTYPE fpType = EFT_MAIN);
//	//读取文件 返回 <= 0 表示失败
//	virtual DWORD FileRead(void* pBbuff,DWORD dwSize,E_FPTYPE fpType = EFT_MAIN);
//	//获得文件长度
//	virtual DWORD FileLength(E_FPTYPE fpType = EFT_MAIN);
//	//先打开文件,如果打开成功,获得打开文件相对文件头的偏移,否则返回-1
//	virtual __int64 GetFileOffset(const char * strPathName,E_FPTYPE fpType = EFT_MAIN);
//
//	void SetIsCompress(bool bCompress);
//
//	BYTE* AcquireFileUnCOmpressData(long lCurrFile, const LoadingFileData& LoadingFileDataObject, E_FPTYPE fpType, DWORD& dwUnCompressSize);
//	
//	// 返回文坚定对于其自身开始未知的偏移量
//	virtual size_t FillTell(long lCurrFile, E_FPTYPE fpType = EFT_MAIN);
//	// 是否到达文件尾
//	virtual bool FileEof(long lCurrFile, E_FPTYPE fpType = EFT_MAIN);
//	// 某文件是否存在
//	virtual bool IsFileExist(const char* strPathName);
//
//	// 获得目录的全路径名
//	std::string GetDirFullPath(PWpfFileList pList);
//
//	//内部用到的函数,为了效率内部不检查参数合法性,由调用处保证其合法性
//protected:
//	//初始化buff,用于计算hash值
//	bool InitBuffer();
//	//是否打开了wpf文件
//	bool IsOpenWpfFile();
//	//载入Fat到m_MWpfFileList,调用时要在打开wpf后,函数内部不再判断
//	bool WpfLoadFat();
//	//从列表查找目录
//	PWpfFileList FindDirFromList(const std::string &strDir,PWpfFileList pList);
//	//计算string的hash值
//	DWORD HashString(const char * strFileName,DWORD type);
//	//计算路径的hash值
//	__int64 HashFileName(const char * pbKey);
//	//清除申请的内存及状态
//	void WpfClear();
//
//protected:
//	static DWORD  m_dwCryptTable[MAX_WPF_HASH_BUFFF_SIZE];//用来计算hash值
//	std::string	  m_strWpfDir,m_strWpfFileName;//wpf路径信息
//	NEWDIR        m_CreateDirHeader;
//	NEWFCB        m_CreateFcbHeader;
//	DWORD         m_dwExpendLen;//没有空白空间时扩展的长度
//	bool          m_bSaveWhenExpended;//扩展之后是否立即保存
//
//	E_WPF_RTN     m_eWrLastError;//最后一次错误代码
//	WpfHeader     m_WpfHeader;//文件头
//	PWpfFileList  m_pWpfRoot;//根目录
//	PWpfFileList  m_pDirList;//目录列表
//	int           m_iFileHandle[EFT_NUM];//文件句柄
//	PBlankBlockNode   m_pBlankBlockList;//空闲块描述列表
//
//	bool          m_bModifyed;//是否对wpf文件进行了修改操作,如果有在最后要自动保存
//	bool          m_bSaving;//是否正在保存,如果正在保存,不允许其它地方进行写操作,为了速度设计上不允许多个线程同时进行写操作,否则可能破坏目录结构
//	__int64       m_iWpfStartPos;//wpf文件内容在整个打开文件中的起始位置
//	DWORD         m_dwOpenType;//打开文件的类型
//	MIFCB1        m_MhashFcb1;//各文件对应的hash值map,只读操作时用到,编辑操作用m_pDirList
//	PFCB1         m_pFcb1List;//fcb1列表
//	PFCB2         m_pFcb2List;//fcb1列表
//	PFCB          m_pFcbList;//fcb列表
//
//	LoadingFileDataMap m_mapLoadingFileData;
//
//	CRITICAL_SECTION m_lock;
//
//	long m_lCount;
//	long m_lCurrent;
//
//	bool m_bCompress;
//};
//
//sdWpfLoader* CreateWpfInterface();
//
//void DestroyWpfInterface(sdWpfLoader*& pkWpfInterface);