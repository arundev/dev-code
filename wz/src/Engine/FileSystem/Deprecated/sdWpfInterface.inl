////wpfinterface.h:wpf�ļ���ʽ�����ӿ�
////wpf�ļ���ʽ���:wpf��Dos\Windows�е�Fat32�ļ�ϵͳΪ����,����ʵ����Ҫ��һЩ�޸�.
////1.���������ÿ��(����)512�ֽڸ�ʽ��,������ƽ��ÿ���ļ�Ѱַ���40��,��ʱ0.048ms,����һ���Զ�ȡ20K(����ͼƬƽ����С)���ݵ�Ч��Ҫ�ȷ�40��ÿ�ζ�ȡ512�ֽڸ�1������,����
////  ����÷������洢���������еĺܶ�ӿ�Ҫ��д,wpf�еĲ��������洢,���¼��ļ�ʱ������������Ŀ�(����),����Ҳ�������EXPEND_LENΪ��λ��չ����ͼ����С,
////  ���ַ�ʽ��ȱ�������ײ�����Ƭ,��������ɾ��������ļ�����,����������¼�,����ɾ���ļ����µĿռ���Ա������¼ӵ��ļ�����,�кܶఴť�ļ�����С��512�ֽ�,�ܵ���˵��Ƭ����ܶ�,���⻹�ṩ��
////  ��Ƭ����ӿ�,�ڷ�����Ƭ̫���ʱ����Խ�����Ƭ����,����߿ռ�������,һ������´���������Ϊ95%����
////  ���ַ�ʽ��ȱ��������ײ�����Ƭ�Լ��洢ǰҪ֪���ļ��Ĵ�С,���������估��ʽ���䶼�������м���Ӱ��,�������Ǵ洢ǰһ�㶼֪���ļ���С,��������汾��ɾ�����޸Ĳ�������,����ѡ������������
////2.FCB(�ļ����ƿ�)�ṹ��ȥ��ʱ����ֶ�,���ļ�����չ��32�ֽ�(������չ��)
////3.Ŀ¼����һ���ļ�,��������Ϊ����Ŀ¼�����ļ���FCB,20����ļ���Ŀ¼���Ҫ13.7M�Ŀռ�洢�����Ϣ
////4.�²���ϵͳ�ڴ�������,������д��̿ռ�
////
////�ṹ����:
////              
////         S_WpfHeader�ļ�ͷ
////-------------------------------
////           �ļ�����
////-------------------------------
////           ���п�������Ϣ����λ��
////-------------------------------
////           FCB List  
//
//
////#pragma once
//
////#ifndef _WINDOWS_
////#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
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
//	EFA_BLANK              = 0,        //�հ�
//	EFA_DIR                = 0x0001,   //Ŀ¼
//	EFA_FILE               = 0x0002,   //�ļ�
//	EFA_COMPRESS           = 0x0004,   //ӵ�д����Ե��ļ�Ϊѹ���ļ�,ѹ���������ļ�ͷ�е�byCompressָ��
//	EFA_CRYPT              = 0x0008,   //ӵ�д����Ե��ļ�Ϊ�����ļ�,ѹ���������ļ�ͷ�е�byCryptָ��
//};
////����һ���ļ�ָ����ļ�,һ��wpf�ļ�֧�ֶ���߳�ͬʱ����
////�����й�wpf�ı༭������ʹ��EFT_MAINָ��,ֻ�������������ͬʱʹ������ָ��
//enum E_FPTYPE
//{
//	EFT_MAIN      = 0,  //���߳�
//	EFT_THREAD    = 1,  //�����߳�
//	EFT_NUM,
//};
//
////wfp�������ش���
//enum E_WPF_RTN
//{
//	EWR_OK                    = 0,   //�ɹ�
//	EWR_ERROR,                       //ʧ��
//	EWR_PARA_ERROR,                  //��������
//	EWR_FILE_NOT_OPEN,               //wpf�ļ�û�д�
//	EWR_FILE_EXIST,                  //�ļ��Ѿ�����
//	EWR_FILE_NOT_EXIST,              //�ļ��Ѿ�����
//	EWR_FILE_SAVE_FAIL,              //�ļ�����ʧ��
//	EWR_FILE_OPEN_FAIL,              //�ļ���ʧ��
//	EWR_NOT_WPF_FILE,                //�ļ���ʽ����
//	EWR_DIR_EXIST,                   //Ŀ¼�Ѿ�����
//	EWR_DIR_NOT_EXIST,               //Ŀ¼������
//	EWR_PATH_INVALID,                //�ļ�����·���Ƿ�
//	EWR_FILE_SAVING,                 //���ڱ����ļ�,��ʱ��д�������ᱻ�ܾ�,Ϊ������,����ϲ�������߳�ͬʱ����д����
//
//	EWR_UNKNOWN,                     //����δ֪�������
//};
//
////wpf�Ĵ򿪷�ʽ
//enum E_WPF_OPENTYPE
//{
//	EWOT_READONLY      = 0x0001,         //ֻ��,����ģʽ���ṹ��Ŀ¼��,�����ļ��ǰ�·����hashֵ����,�ٶȿ�,ʡ�ڴ�
//	EWOT_RDWR          = 0x0002,         //��дģʽ,Ҫ����ļ���Ŀ¼���,ɾ��,�޸ĵȲ���ʹ�ô�ģʽ,д����ʹ��EFT_MAIN�ļ�ָ��,�д�����ʱ���ļ������������̰߳�ȫ,���ⲿ��֤�̰߳�ȫ
//	EWOT_LISTDIR       = 0x0004,         //�г�Ŀ¼,EWOT_RDWR����EWOT_LISTDIR
//};
//
//#pragma pack (4)
//
////fcb��1����,������ʱ�õ�,Ϊ��ʡ�ڴ��fcb2�ֿ�
//struct S_Fcb1
//{
//	S_Fcb1()
//	{
//		dwStart = 0;
//		dwSize = 0;
//	}
//
//	DWORD   dwStart;        //������ļ���ʾ���ļ�����ʼ���,�����Ŀ¼��ʾ��Ŀ¼���ļ�Ŀ¼����ʼFCB���
//	DWORD   dwSize;         //�����ļ���ʾ�ļ����ȣ����ֽ�Ϊ��λ,����Ŀ¼��ʾ��Ŀ¼�е�FCB����
//	__int64 iHashKey;       //���ļ�ȫ·����hashֵ
//};
//
////fcb��2����,�༭����ʱ�õ�
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
//	char  strName[MAX_FILE_NAME_LEN];    //�ļ�������չ��,Ŀ¼��ΪĿ¼��
//	char  strMd5[16];                    //�ļ�MD5��
//	DWORD dwAttribute;                   //����E_FCTATTR����
//	DWORD dwRev;                         //��ӵ��EFA_COMPRESS����ʱ��ʾѹ��֮ǰ�Ĵ�С
//};
//
//
////wpfͷ�ṹ,64�ֽ�,2�ݹ�128�ֽ�
//struct S_WpfHeader
//{
//	S_WpfHeader()
//	{
//		dwMagic = WPF_MAGIC;
//		wHeaderSize = sizeof(S_WpfHeader) * 2;//�ļ�ͷ���ļ�ͷ����
//		iWpfSize = 0;
//		iBlankBlockPos = wHeaderSize;
//		dwBlankBlockSize = 0;
//		dwDirCount = 1;//Ĭ����һ����Ŀ¼
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
//		//memcpy(byRev2 + 64 - strlen("Design:�ι���,shandagames"),"Design:�ι���,shandagames",strlen("Design:�ι���,shandagames"));
//	}
//
//	DWORD     dwMagic;              //ħ��,�̶�Ϊ'WPF\x01'
//	WORD      wHeaderSize;          //ͷ�Ĵ�С,Ҳ�ǵ�һ���ļ���Ŀ¼����ʼλ��
//	WORD      wBytesPerBlock;       //ÿ������ֽ���
//
//	__int64   iBlankBlockPos;       //���п�������Ϣ����λ��
//	DWORD     dwBlankBlockSize;     //�ж��������п���Ϣ(��ʼλ��1,��С1,��ʼλ��2,��С2...)DWORD
//	__int64   iFatPos;              //�ļ����������λ��
//	DWORD     dwDirCount;           //Ŀ¼����
//	DWORD     dwFileCount;          //�ļ�����
//	DWORD     dwTotalBlocks;        //���ֵ��ܿ���
//	__int64   iWpfSize;             //����wpf�ļ��Ĵ�С,������ͷ,���п�������Ϣ���ļ������
//	BYTE      byRev1;               //������ʷԭ��,���ֽ�����
//	BYTE      byCrypt;              //��������
//	BYTE      byCompress;           //ѹ������
//	BYTE      byRev2;               //����
//	DWORD     dwRev[2];             //����
//	BYTE      byRev3[3];            //����
//	BYTE      bySavingHeader;       //�Ƿ����ڱ����ļ�ͷ,ΪTRUE��ʾ�ϴ�û�б����ļ�ͷ�ɹ�,ʧ�ܺ��´ζ�ȡʱ���ȡ�ϴε��ļ�ͷ����,���ƿ��Ա�֤ʧ�ܺ�ԭ����fcblist��������,�ش�汾�����޸�,����ͷдʧ���´λ���д����ͷ,����������ͷ��bySavingHeader��ΪTRUE,���Ǵ�����ʱ���ʧ����,��ô�Ͳ���һ��wpf�ļ�,��������򿪵�ʱ��᷵��ʧ��
//};
//#pragma pack ()
//
//typedef struct S_WpfHeader WpfHeader;
//typedef struct S_Fcb1   FCB1;
//typedef struct S_Fcb1*  PFCB1;
//typedef struct S_Fcb2   FCB2;
//typedef struct S_Fcb2*  PFCB2;
//
////wpf�ļ��б�ṹ
//struct S_WpfFileList;
//typedef struct S_WpfFileList WpfFileList;
//typedef struct S_WpfFileList* PWpfFileList;
//typedef std::map<std::string,PWpfFileList> MList;
//
//
//// fcb��¼�ṹ
//struct S_Fcb
//{ 
//	S_Fcb()
//	{
//		pfcb1 = NULL;
//		pfcb2 = NULL;
//		pParent = NULL;
//	}
//
//	PFCB1 pfcb1;          //fcb��һ����
//	PFCB2 pfcb2;          //fcb�ڶ�����
//	PWpfFileList pParent; //��Ŀ¼	
//};
//
//typedef struct S_Fcb  FCB;
//typedef struct S_Fcb* PFCB;
//typedef std::map<std::string,PFCB> MSFCB;
//typedef std::map<__int64,PFCB1> MIFCB1;
//
////Ŀ¼�ṹ
//struct S_WpfFileList
//{
//	S_WpfFileList()
//	{
//		pfcb = NULL;
//	}
//
//	PFCB        pfcb;   //��Ŀ¼���ļ���fcb
//	MList       MDirs;  //��Ŀ¼�µ���Ŀ¼�б�
//	MSFCB       MFiles; //��Ŀ¼�µ����ļ��б�
//};