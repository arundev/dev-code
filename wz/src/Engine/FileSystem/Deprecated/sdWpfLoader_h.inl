//#pragma once
//
////#include "wpfinterface.h"
//
//#define  FCB1_SIZE           sizeof(S_Fcb1)
//#define  FCB2_SIZE           sizeof(S_Fcb2)
//#define  FCB_SIZE            (FCB1_SIZE + FCB2_SIZE)
//
//#define  MAX_WPF_HASH_BUFFF_SIZE   0x500 //��������hashֵ
//#define  BLOCK_SIZE 256*1024   //�ֿ��д�ļ�һ��Ĵ�С
//
////�½��ļ�����Ϣ,�������ر�ʱ�ͷ��ڴ�
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
////���п�������Ϣ�ṹ
//typedef struct S_BlankBlock
//{
//	DWORD dwStart;//��ʼλ��
//	DWORD dwSize;//��С
//}BlankBlock,*PBlankBlock;
//
////���п�������Ϣ�б�ṹ
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
//	// ��ǰ�����ļ�FCB1
//	PFCB1	m_pOpenFileFcb1[EFT_NUM];
//	PFCB2	m_pOpenFileFcb2[EFT_NUM];
//	// ��ǰ�����ļ����ļ�ָ��λ��,����ڵ�ǰ���ļ�����ʼλ��
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
//	//��һ��Wpf�ļ�,���е�Ŀ¼���ļ���Ϣ���浽һ��MWpfFileList�ṹ,����ͨ��GetRoot��ø�
//	//iOffset:wpf�ļ�������strPathName����ļ��е���ʼλ��,��������ļ�����һ��wpf�ļ�,ȡ0
//	virtual bool WpfOpen(const char * strPathName,__int64 iOffset = 0,DWORD dwOpenType = EWOT_READONLY);
//	//�ر��ļ�,����ļ��б�
//	virtual void WpfClose();
//	//������һ�δ������
//	virtual const char* GetWpfName();
//	virtual E_WPF_RTN GetLastError();
//	//���ָ��wpf�ļ����ļ��б�map,Open����Ч
//	virtual PWpfFileList GetRoot();
//	//����ļ�ͷ
//	virtual WpfHeader *GetHeader();
//	//����Ŀ¼
//	virtual PWpfFileList FindDir(const char * strPathName);
//	//�����ļ�,Ŀ¼�ָ���ֻ����"\",������,"/",��ΪЧ������,�ڲ�����ת��
//	virtual PFCB FindFile(const char * strPathName);
//	//����һ���ļ�֮ǰҪ�ȴ�����ļ�,��Seek,Read��,�����������е�һ���ļ�һ��,���Ժ��ļ�ָ��ͣ������ļ�����ʼλ��
//	//ͬһʱ��ֻ�ܲ���һ���ļ�,���Ҫ����̲߳���ͬһ��wpf�ļ�����ͨ����ͬ���ļ�ָ��,��E_FPTYPE,һ��������ͬʱ������wpfͼ���е��ļ�,Ҫ����������ʹ�ö������,���ⲿ����
//	//FileWrite���������Զ������ļ�,�ļ��Ĵ�С�ڴ���ʱ�Ͷ���,��������ļ�����󳤶�д���ļ�����Ϊֹ
//	//��һ���ļ�,���֮ǰ���������ļ����Զ��ر�ԭ�����ļ�,��һ�����ļ�֮ǰ���Բ�����FileClose(),Ŀ¼�ָ���ֻ����"\",������,"/",��ΪЧ������,�ڲ�����ת��
//	virtual long FileOpen(const char * strPathName,E_FPTYPE fpType = EFT_MAIN);
//	//�رյ�ǰ�򿪵��ļ�
//	virtual bool FileClose(E_FPTYPE fpType = EFT_MAIN);
//	//�ڵ�ǰ�򿪵��ļ���Ѱַ,SEEK_ENDʱ,dwOffsetҲȡ��ֵ,��ʾ��λ��ΪFileLength - dwOffset
//	virtual bool FileSeek(DWORD dwOffset,int iOrigin = SEEK_SET,E_FPTYPE fpType = EFT_MAIN);
//	//��ȡ�ļ� ���� <= 0 ��ʾʧ��
//	virtual DWORD FileRead(void* pBbuff,DWORD dwSize,E_FPTYPE fpType = EFT_MAIN);
//	//����ļ�����
//	virtual DWORD FileLength(E_FPTYPE fpType = EFT_MAIN);
//	//�ȴ��ļ�,����򿪳ɹ�,��ô��ļ�����ļ�ͷ��ƫ��,���򷵻�-1
//	virtual __int64 GetFileOffset(const char * strPathName,E_FPTYPE fpType = EFT_MAIN);
//
//	void SetIsCompress(bool bCompress);
//
//	BYTE* AcquireFileUnCOmpressData(long lCurrFile, const LoadingFileData& LoadingFileDataObject, E_FPTYPE fpType, DWORD& dwUnCompressSize);
//	
//	// �����ļᶨ����������ʼδ֪��ƫ����
//	virtual size_t FillTell(long lCurrFile, E_FPTYPE fpType = EFT_MAIN);
//	// �Ƿ񵽴��ļ�β
//	virtual bool FileEof(long lCurrFile, E_FPTYPE fpType = EFT_MAIN);
//	// ĳ�ļ��Ƿ����
//	virtual bool IsFileExist(const char* strPathName);
//
//	// ���Ŀ¼��ȫ·����
//	std::string GetDirFullPath(PWpfFileList pList);
//
//	//�ڲ��õ��ĺ���,Ϊ��Ч���ڲ����������Ϸ���,�ɵ��ô���֤��Ϸ���
//protected:
//	//��ʼ��buff,���ڼ���hashֵ
//	bool InitBuffer();
//	//�Ƿ����wpf�ļ�
//	bool IsOpenWpfFile();
//	//����Fat��m_MWpfFileList,����ʱҪ�ڴ�wpf��,�����ڲ������ж�
//	bool WpfLoadFat();
//	//���б����Ŀ¼
//	PWpfFileList FindDirFromList(const std::string &strDir,PWpfFileList pList);
//	//����string��hashֵ
//	DWORD HashString(const char * strFileName,DWORD type);
//	//����·����hashֵ
//	__int64 HashFileName(const char * pbKey);
//	//���������ڴ漰״̬
//	void WpfClear();
//
//protected:
//	static DWORD  m_dwCryptTable[MAX_WPF_HASH_BUFFF_SIZE];//��������hashֵ
//	std::string	  m_strWpfDir,m_strWpfFileName;//wpf·����Ϣ
//	NEWDIR        m_CreateDirHeader;
//	NEWFCB        m_CreateFcbHeader;
//	DWORD         m_dwExpendLen;//û�пհ׿ռ�ʱ��չ�ĳ���
//	bool          m_bSaveWhenExpended;//��չ֮���Ƿ���������
//
//	E_WPF_RTN     m_eWrLastError;//���һ�δ������
//	WpfHeader     m_WpfHeader;//�ļ�ͷ
//	PWpfFileList  m_pWpfRoot;//��Ŀ¼
//	PWpfFileList  m_pDirList;//Ŀ¼�б�
//	int           m_iFileHandle[EFT_NUM];//�ļ����
//	PBlankBlockNode   m_pBlankBlockList;//���п������б�
//
//	bool          m_bModifyed;//�Ƿ��wpf�ļ��������޸Ĳ���,����������Ҫ�Զ�����
//	bool          m_bSaving;//�Ƿ����ڱ���,������ڱ���,�����������ط�����д����,Ϊ���ٶ�����ϲ��������߳�ͬʱ����д����,��������ƻ�Ŀ¼�ṹ
//	__int64       m_iWpfStartPos;//wpf�ļ��������������ļ��е���ʼλ��
//	DWORD         m_dwOpenType;//���ļ�������
//	MIFCB1        m_MhashFcb1;//���ļ���Ӧ��hashֵmap,ֻ������ʱ�õ�,�༭������m_pDirList
//	PFCB1         m_pFcb1List;//fcb1�б�
//	PFCB2         m_pFcb2List;//fcb1�б�
//	PFCB          m_pFcbList;//fcb�б�
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