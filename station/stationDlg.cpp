
// stationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "station.h"
#include "stationDlg.h"
#include "afxdialogex.h"
#include "DIJoystick.h"



#include <highgui.h>
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Joystick����
CDIJoystick myJoystick;
//Joystick����
CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CvVideoWriter* writer = 0;

unsigned char UART_CommandRoute(unsigned char *com_rx_buffer,long len);


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CstationDlg �Ի���




CstationDlg::CstationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstationDlg::IDD, pParent)
	, CommState(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_str_rec = _T("");
	m_str_send = _T("");
	m_i_send_data1 = 0;
	m_i_send_data2 = 0;
	m_i_send_data3 = 0;
	m_i_send_data4 = 0;
	m_str_rec_data1 = _T("");
	m_str_rec_data2 = _T("");
	m_str_rec_data3 = _T("");
	m_str_rec_data4 = _T("");
	m_XAxis = _T("");
	m_YAxis = _T("");
	m_ZAxis = _T("");
	m_RZAxis = _T("");
	m_str_yaw = _T("");
	m_str_pitch = _T("");
	m_str_press = _T("");
	m_str_roll = _T("");
	m_str_tempr = _T("");
	m_str_lon = _T("");
	m_str_lat = _T("");
	m_str_altitud = _T("");
	m_str_svnum = _T("");
	m_str_speed = _T("");
	m_str_ns = _T("");
}

void CstationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BUTTON_NAMES, m_ctlButtonNames);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_COM_COMX, m_ctrlComx);
	DDX_Control(pDX, IDC_COM_BAUD, m_ctrlBaud);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);


	DDX_Text(pDX, IDC_EDIT_REC, m_str_rec);
	DDX_Text(pDX, IDC_EDIT_SEND, m_str_send);
	DDX_Text(pDX, IDC_SEND_DATA1, m_i_send_data1);
	DDX_Text(pDX, IDC_SEND_DATA2, m_i_send_data2);
	DDX_Text(pDX, IDC_SEND_DATA3, m_i_send_data3);
	DDX_Text(pDX, IDC_SEND_DATA4, m_i_send_data4);
	DDX_Text(pDX, IDC_REC_DATA1, m_str_rec_data1);
	DDX_Text(pDX, IDC_REC_DATA2, m_str_rec_data2);
	DDX_Text(pDX, IDC_REC_DATA3, m_str_rec_data3);
	DDX_Text(pDX, IDC_REC_DATA4, m_str_rec_data4);
	DDX_Control(pDX, IDC_JOYSTICK_NAME, m_ctlJoystickName);
	DDX_Text(pDX, IDC_XAXIS, m_XAxis);
	DDX_Text(pDX, IDC_YAXIS, m_YAxis);
	DDX_Text(pDX, IDC_ZAXIS, m_ZAxis);
	DDX_Text(pDX, IDC_RZAXIS, m_RZAxis);
	DDX_Control(pDX, IDC_BUTTON_NAMES, m_ctlButtonNames);
	DDX_Text(pDX, IDC_YAW, m_str_yaw);
	DDX_Text(pDX, IDC_PITCH, m_str_pitch);
	DDX_Text(pDX, IDC_PRESS, m_str_press);
	DDX_Text(pDX, IDC_ROLL, m_str_roll);
	DDX_Text(pDX, IDC_TEMPR, m_str_tempr);
	DDX_Text(pDX, IDC_LON, m_str_lon);
	DDX_Text(pDX, IDC_LAT, m_str_lat);
	DDX_Text(pDX, IDC_ALTITUD, m_str_altitud);
	//  DDX_Control(pDX, IDC_SVNUM, m_str_svnum);
	DDX_Text(pDX, IDC_SVNUM, m_str_svnum);
	DDX_Text(pDX, IDC_SEEED, m_str_speed);
	DDX_Control(pDX, IDC_STATIC_EW, m_str_ew);
	DDX_Text(pDX, IDC_STATIC_NS, m_str_ns);
}

BEGIN_MESSAGE_MAP(CstationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CstationDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CstationDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEND, &CstationDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_AGREE_SEND, &CstationDlg::OnBnClickedBtnAgreeSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_OPEN_CAMERA, &CstationDlg::OnBnClickedOpenCamera)
	ON_BN_CLICKED(ID_CLOSE_CAMERA, &CstationDlg::OnBnClickedCloseCamera)
END_MESSAGE_MAP()

//ö�ٴ���
void EnumerateSerialPorts(CUIntArray& ports)
{
	//Make sure we clear out any elements which may already be in the array
	ports.RemoveAll();

		//Use QueryDosDevice to look for all devices of the form COMx. This is a better
		//solution as it means that no ports have to be opened at all.
		TCHAR szDevices[65535];
		DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
		if (dwChars)
		{
			int i = 0;
			for (;;)
			{
				//Get the current device name
				TCHAR* pszCurrentDevice = &szDevices[i];
				//If it looks like "COMX" then
				//add it to the array which will be returned
				int nLen = _tcslen(pszCurrentDevice);
				if (nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
				{
					//Work out the port number
					int nPort = _ttoi(&pszCurrentDevice[3]);
					ports.Add(nPort);
				}
				// Go to next NULL character
				while (szDevices[i] != _T('\0'))
					i++;
				// Bump pointer to the next string
				i++;
				// The list is double-NULL terminated, so if the character is
				// now NULL, we're at the end
				if (szDevices[i] == _T('\0'))
					break;
			}
		}
		else
			TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
}
// CstationDlg ��Ϣ�������

BOOL CstationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ʼ������
	m_btnOpen.EnableWindow(TRUE);
	m_btnClose.EnableWindow(FALSE);
	m_btnSend.EnableWindow(FALSE);
	m_ctrlComx.EnableWindow(TRUE);

	
	//��ʼ��combox
	CommState=false;//Ĭ�ϴ��ڹر�
	CUIntArray ports;
	EnumerateSerialPorts(ports);
	for (int i = 0; i<ports.GetSize(); i++)
	{
		CString str_com;
//		CString temp;
		CString str;
		str.Format(_T("%d"), ports.ElementAt(i));
//		str.Format("%d",ports.ElementAt(i));
		str_com = (CString)"COM" + str;
//		str_com = temp+str;
		if (str_com != "COM0")
		{
			m_ctrlComx.AddString(str_com);
//			m_JoyComx.AddString(str_com);
		}
	}
	m_ctrlBaud.AddString(_T("9600"));
	m_ctrlBaud.AddString(_T("38400"));
	m_ctrlBaud.AddString(_T("19200"));
	m_ctrlBaud.AddString(_T("57600"));
	m_ctrlBaud.AddString(_T("115200"));


	m_ctrlComx.SetCurSel(0);
//	m_JoyComx.SetCurSel(0);
	m_ctrlBaud.SetCurSel(0);

//////////////////////////////////////////////////////////////////////
//
// ����ͷ���ں���
//
//////////////////////////////////////////////////////////////////////
	// TODO: Add extra initialization here
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);
        pDC =pwnd->GetDC();
	//pDC =GetDC();
        hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	// ���ü�ʱ��,ÿxms����һ���¼�
	SetTimer(1, 50, NULL);
	SetTimer(2, 80, NULL);
	SetTimer(3, 10, NULL);//����ͷ10ms



//////////////////////////////////////////////////////////////////////
//
// Set the HWND for this Dialog Instance To Enable DInput!
//�ֱ���ʼ��
//
//////////////////////////////////////////////////////////////////////
	CWnd *jb = this;

	myJoystick.SetHWND(jb->m_hWnd);

	// Reset Combo Control
	m_ctlJoystickName.ResetContent();

	LPCDIDEVICEINSTANCE lpddi = NULL;

	// Ensure you have First Joystick ID to start search for additional Devices!
	lpddi = myJoystick.GetFirstJoystickID();

	if (!lpddi)
	{
		// No joysticks have been found!
		//AfxMessageBox("I have not been able to find a joystick on your system.", "No Joystick Detected", MB_ICONHAND | MB_OK);
		AfxMessageBox(_T("I have not been able to find a joystick on your system."));

		//OnCancel();
	}
	else
	{

		while (lpddi)
		{
			int x = m_ctlJoystickName.AddString(lpddi->tszInstanceName);
			m_ctlJoystickName.SetItemDataPtr(x, (void*)lpddi);
			lpddi = myJoystick.GetNextJoystickID();
		}

		m_ctlJoystickName.SetCurSel(0);
		OnCbnSelchangeButtonNames();

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CstationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CstationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CstationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CstationDlg, CDialogEx)
	ON_EVENT(CstationDlg, IDC_MSCOMM1, 1, CstationDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


////////////////////////////���´���Э��//////////////////////////////
//uart reicer flag
#define b_uart_head  0x80  //�յ�A5 ͷ ��־λ
#define b_rx_over    0x40  //�յ�������֡��־
// USART Receiver buffer

//volatile unsigned char rx_wr_index; //����дָ��
volatile unsigned char RC_Flag;  //����״̬��־�ֽ�
#define RX_BUFFER_SIZE 50 //���ջ������ֽ���
unsigned char data_buffer[RX_BUFFER_SIZE];//��Ч���ݻ�����
CString str_rec_temp1;
CString str_rec_temp2;
CString str_rec_temp3;
CString str_rec_temp4;

 //--У�鵱ǰ���յ���һ֡�����Ƿ� ��֡У���ֽ�һ��
//unsigned char Sum_check(unsigned char *check_buffer)
unsigned char Sum_check(void)
{ 
	unsigned char i;
	unsigned int checksum=0; 
  for(i=0;i<data_buffer[0]-2;i++)
	  checksum+=data_buffer[i];
	if((checksum%256)==data_buffer[data_buffer[0]-2])
		return(0x01); //Checksum successful
	else
		return(0x00); //Checksum error
}


void CstationDlg::OnCommMscomm1()
{
	// TODO: �ڴ˴������Ϣ����������
/////////////////////////////////////////////////////////////
///////////////////������̬��GPS����////////////////////////////////////	  	
	//�Ӵ��ڽ�����,Э�����
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //����BYTE����
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp=m_ctrlComm.get_Input(); //����������Ϣ
		safearray_inp=variant_inp; ///����ת��
		len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			UpdateData(FALSE); //���±༭������	
		}
	}

	
	/*/////////////////////////////////////////////////////////////
///////////////////��ʱ������int*4����,���յ���λ��+1������ݴ���////////////////////////////////////	  	
	//�Ӵ��ڽ�����,Э�����
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //����BYTE����
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp=m_ctrlComm.get_Input(); //����������Ϣ
		safearray_inp=variant_inp; ///����ת��
		len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			m_str_rec_data1=str_rec_temp1; //������ձ༭���Ӧ�ַ���
			m_str_rec_data2=str_rec_temp2; //������ձ༭���Ӧ�ַ���
			m_str_rec_data3=str_rec_temp3; //������ձ༭���Ӧ�ַ���
			m_str_rec_data4=str_rec_temp4; //������ձ༭���Ӧ�ַ���
			UpdateData(FALSE); //���±༭������	
		}
	}
*/
/*	
/////////////////////////////////////////////////////////////
///////////////////"Э�鷢��"////////////////////////////////////	  	
	//�Ӵ��ڽ�����,Э�����
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //����BYTE����
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp=m_ctrlComm.get_Input(); //����������Ϣ
		safearray_inp=variant_inp; ///����ת��
		len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);

		if(UART_CommandRoute(rxdata,len))
		{
			m_str_rec_data1=str_rec_temp1; //������ձ༭���Ӧ�ַ���
			m_str_rec_data2=str_rec_temp2; //������ձ༭���Ӧ�ַ���
			m_str_rec_data3=str_rec_temp3; //������ձ༭���Ӧ�ַ���
			m_str_rec_data4=str_rec_temp4; //������ձ༭���Ӧ�ַ���
			UpdateData(FALSE); //���±༭������	
		}
	}
*/	
/*		
/////////////////////////////////////////////////////////////
///////////////////�����յ�����ʵʱ��ʾ////////////////////////////////////
	//�Ӵ��ڽ������ݲ���ʾ�ڱ༭����
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	long len,k;
	byte rxdata[512]; //����BYTE����
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp=m_ctrlComm.get_Input(); //����������Ϣ
		safearray_inp=variant_inp; ///����ת��
		len=safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);
		for(k=0;k<len;k++) //������ת��ΪCString�ͱ���
		{
			char bt=*(char*)(rxdata+k); //�ַ���
			strtemp.Format(_T("%c"),bt); //���ַ�������ʱ����strtemp���
			m_str_rec+=strtemp; //������ձ༭���Ӧ�ַ���
		}
	}
	UpdateData(FALSE); //���±༭������	
////////////////////////////////////////////////////////////
*/


}


void CstationDlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str_com ;
	CString Baud_rate;
	int opencom = 0;
	m_ctrlComx.GetWindowText(str_com);
	m_ctrlBaud.GetWindowText(Baud_rate);

	m_btnOpen.EnableWindow(FALSE);
	m_btnClose.EnableWindow(TRUE);
	m_btnSend.EnableWindow(TRUE);
	m_ctrlComx.EnableWindow(TRUE);

	if (str_com == "COM1")
		   opencom = 1;
	else if(str_com == "COM2")
		   opencom = 2;
	else if (str_com == "COM3")
		opencom = 3;
	else if (str_com == "COM4")
		opencom = 4;
	else if (str_com == "COM5")
		opencom = 5;
	else if (str_com == "COM6")
		opencom = 6;
	else if (str_com == "COM7")
		opencom = 7;
	else if (str_com == "COM8")
		opencom = 8;
	else if (str_com == "COM9")
		opencom = 9;
	else if (str_com == "COM10")
		opencom = 10;
	else if (str_com == "COM11")
	opencom = 11;
	else if (str_com == "COM12")
		opencom = 12;
	else if (str_com == "COM13")
		opencom = 13;
	else if (str_com == "COM14")
		opencom = 14;
	else if (str_com == "COM15")
		opencom = 15;
	else
		opencom = 0;

	
	m_ctrlComm.put_CommPort(opencom);//ѡ��com��
	m_ctrlComm.put_InputMode(1);//���뷽ʽΪ�����Ʒ�ʽ
	m_ctrlComm.put_InBufferSize(1024);//���뻺������СΪ1024byte
	m_ctrlComm.put_OutBufferSize(512);//�����������СΪ512byte

	if (Baud_rate == "9600")
		m_ctrlComm.put_Settings(_T("9600,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	else if (Baud_rate == "19200")
		m_ctrlComm.put_Settings(_T("19200,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	else if (Baud_rate == "57600")
		m_ctrlComm.put_Settings(_T("57600,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	else if (Baud_rate == "38400")
		m_ctrlComm.put_Settings(_T("38400,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	else if (Baud_rate == "115200")
		m_ctrlComm.put_Settings(_T("115200,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	else
		m_ctrlComm.put_Settings(_T("9600,n,8,1"));//���ô��ڲ�����9600�����ʣ�����żУ�飬8������λ��1��ֹͣλ
	
	if (!m_ctrlComm.get_PortOpen())
		m_ctrlComm.put_PortOpen(opencom);//�򿪴���
	else
		////MessageBox("Open COM fail!");
		AfxMessageBox(_T("Open COM fail!"));
	m_ctrlComm.put_RThreshold(1);//ÿ�����ڽ��ջ������ж�������1���ַ�ʱ������һ���������ݵ�oncomm�¼�
	m_ctrlComm.put_InputLen(0);//���õ�ǰ���������ݳ���Ϊ0
	m_ctrlComm.get_Input();//Ԥ������������ղ�������
//	Receive_Flag = 0;
	CommState=true;//����״̬Ϊ��

}


void CstationDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnOpen.EnableWindow(TRUE);
	m_btnClose.EnableWindow(FALSE);
	m_btnSend.EnableWindow(FALSE);
	m_ctrlComx.EnableWindow(TRUE);
	m_ctrlComm.put_PortOpen(0);//�رմ���
	CommState=false;//����״̬Ϊ�ر�
}


void CstationDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//�����͡�����
	UpdateData(true); //��ȡ�༭������
	m_ctrlComm.put_Output(COleVariant(m_str_send));//��������
//	m_strSendData.Empty(); //���ͺ���������
	UpdateData(false); //���±༭������
	//�Է��͵����ݽ���ǿ������ת������ CString �ַ�������ת��Ϊ VARIANT ���͡�


}


void CstationDlg::OnBnClickedBtnAgreeSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: Add your control notification handler code here
	
	unsigned char ch_send;
	unsigned int i_and=0;//
	UpdateData(true); //��ȡ�༭������
	CByteArray tempArray;
	tempArray.RemoveAll();
	tempArray.SetSize(10);//

	
	tempArray.SetAt(0,0xA5);//A5��5A�����ݰ���ͷ
	tempArray.SetAt(1,0x5A);//A5��5A�����ݰ���ͷ

	tempArray.SetAt(2,4+2+2);//���ȣ�4���ֽڵ�����+2���ֽڵĿ�ʼ�����Ⱥ����ݰ�ID��+2���ֽڵĽ�����У��ͽ�����־aa��
	i_and+=8;//У����ۼ�
	
	tempArray.SetAt(3,0xA3);//A3�����ݰ�ID
	i_and+=0xA3;//У����ۼ�

	ch_send=m_i_send_data1;//��ȡ��һ����������
	tempArray.SetAt(4,ch_send);
	i_and+=ch_send;//У����ۼ�

	ch_send=m_i_send_data2;//��ȡ��һ����������
	tempArray.SetAt(5,ch_send);
	i_and+=ch_send;//У����ۼ�

	ch_send=m_i_send_data3;//��ȡ��һ����������
	tempArray.SetAt(6,ch_send);
	i_and+=ch_send;//У����ۼ�

	ch_send=m_i_send_data4;//��ȡ��һ����������
	tempArray.SetAt(7,ch_send);
	i_and+=ch_send;//У����ۼ�
	
	tempArray.SetAt(8,i_and%256);//У���
	tempArray.SetAt(9,0xAA);//������־aa
	m_ctrlComm.put_Output(COleVariant(tempArray));//��������
	m_ctrlComm.put_Output(COleVariant(tempArray));//��������
}

////////////////////////////////////////////////////////////////////
//�ֱ����Ƴ����                                                     
////////////////////////////////////////////////////////////////////
void CstationDlg::OnCbnSelchangeButtonNames(void)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWnd*jb = this;
	myJoystick.SetHWND(jb->m_hWnd);

	LPCDIDEVICEINSTANCE lpddi = NULL;

//	m_ctlButtonNames.ResetContent();

	int x = m_ctlJoystickName.GetCurSel();

	lpddi = (LPCDIDEVICEINSTANCE)m_ctlJoystickName.GetItemDataPtr(x);

	GUID myguid;

	if (lpddi && ((int)lpddi != -1))
	{

		memcpy(&myguid, &(lpddi->guidInstance), sizeof(GUID));

		//bool flag=myJoystick.CreateDevice(&myguid);
		myJoystick.SetPreferredDevice(&myguid);

//		m_Buttons = myJoystick.HowManyButtons();

		TCHAR* name = NULL;
		name = myJoystick.GetFirstButtonName();


		while (name)
		{
			m_ctlButtonNames.AddString(name);
			name = myJoystick.GetNextButtonName();

		}
	}

	m_ctlButtonNames.SetCurSel(0);
}


void CstationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: Add your message handler code here and/or call default
	/************************************************************************/
	/* ��ʾ����ͷ                                                           */
	/************************************************************************/
	IplImage* m_Frame;
	m_Frame=cvQueryFrame(capture);
	CvvImage m_CvvImage;
	myJoystick.PollDevice();//ң�����豸

	LPDIJOYSTATE2 joy = myJoystick.GetJoystickStateInfo();
	
//	m_XAxis.Format("%d", joy->lX);
	m_XAxis.Format(_T("%d"), joy->lX);
	m_YAxis.Format(_T("%d"), joy->lY);
	m_ZAxis.Format(_T("%d"), joy->lRz);

//	m_RXAxis.Format(_T("%d"), joy->lRx);
//	m_RYAxis.Format(_T("%d"), joy->lRy);
	m_RZAxis.Format(_T("%d"), joy->lZ);


	switch (nIDEvent)
	{
	case 1:
		UpdateData(FALSE);//�ؼ�������ʾ
		break;
	case 2:
		if(CommState==true)
			MyCommAgreeSend();
		UpdateData(FALSE);//�ؼ�������ʾ
/*		m_ctrlComm.put_OutBufferCount(0);
		if (JoyFlag == 1)
			m_JoyComm.put_Output(COleVariant(hexdata));//��������
*/		break;
	case 3://����ͷˢ����ʾ
		m_CvvImage.CopyOf(m_Frame,1);	
		if (true)
		{
			m_CvvImage.DrawToHDC(hDC, &rect);
			//cvWaitKey(10);
		}
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CstationDlg::OnBnClickedOpenCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox("OK");
	if(!capture)
	{
		capture = cvCaptureFromCAM(0);
		//AfxMessageBox("OK");
	}
	if (!capture)
	{
		AfxMessageBox(_T("�޷�������ͷ"));
		return;
	}

	// ����
	IplImage* m_Frame;
	m_Frame=cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame,1);	
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);
	}

	// ���ü�ʱ��,ÿ10ms����һ���¼�
//	SetTimer(1,10,NULL);
}


void CstationDlg::OnBnClickedCloseCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: Add your control notification handler code here
	cvReleaseCapture(&capture);
	CDC MemDC; 
	CBitmap m_Bitmap1;
//	m_Bitmap1.LoadBitmap(IDB_BITMAP1); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
}

//����Э�鷢������
void CstationDlg::MyCommAgreeSend(void)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: Add your control notification handler code here

	LPDIJOYSTATE2 joy = myJoystick.GetJoystickStateInfo();
	
	unsigned char ch_send;
	unsigned int i_and=0;//
	UpdateData(true); //��ȡ�༭������
	CByteArray tempArray;
	tempArray.RemoveAll();
	tempArray.SetSize(15);//

	
	tempArray.SetAt(0,0xA5);//A5��5A�����ݰ���ͷ
	tempArray.SetAt(1,0x5A);//A5��5A�����ݰ���ͷ

	ch_send=8+2+2;
	tempArray.SetAt(2,ch_send);//���ȣ�4���ֽڵ�����+2���ֽڵĿ�ʼ�����Ⱥ����ݰ�ID��+2���ֽڵĽ�����У��ͽ�����־aa��
	i_and+=ch_send;//У����ۼ�
	
	ch_send=0xA3;
	tempArray.SetAt(3,ch_send);//A3�����ݰ�ID
	i_and+=ch_send;//У����ۼ�

joy->lX=123;//���Դ���ͨ��
joy->lY=256;
joy->lRz=1000;
joy->lZ=2048;
	ch_send=joy->lX/256;//��ȡ��1����������
	tempArray.SetAt(4,ch_send);
	i_and+=ch_send;//У����ۼ�	
	ch_send=joy->lX%256;//��ȡ��1����������
	tempArray.SetAt(5,ch_send);
	i_and+=ch_send;//У����ۼ�


	ch_send=joy->lY/256;//��ȡ��2����������
	tempArray.SetAt(6,ch_send);
	i_and+=ch_send;//У����ۼ�	
	ch_send=joy->lY%256;//��ȡ��2����������
	tempArray.SetAt(7,ch_send);
	i_and+=ch_send;//У����ۼ�

	ch_send=joy->lRz/256;//��ȡ��3����������
	tempArray.SetAt(8,ch_send);
	i_and+=ch_send;//У����ۼ�
	ch_send=joy->lRz%256;//��ȡ��3����������
	tempArray.SetAt(9,ch_send);
	i_and+=ch_send;//У����ۼ�

	ch_send=joy->lZ/256;//��ȡ��4����������
	tempArray.SetAt(10,ch_send);
	i_and+=ch_send;//У����ۼ�
	ch_send=joy->lZ%256;//��ȡ��4����������
	tempArray.SetAt(11,ch_send);
	i_and+=ch_send;//У����ۼ�
	
	tempArray.SetAt(12,i_and%256);//У���
	tempArray.SetAt(13,0xAA);//������־aa
	m_ctrlComm.put_Output(COleVariant(tempArray));//��������	
	m_ctrlComm.put_Output(COleVariant(tempArray));//��������

}
//--����ӳ�����Ҫ���������� ��ʱ����,�Լ�� �����Ƿ������һ֡����
/*
unsigned char UART_CommandRoute(unsigned char *com_rx_buffer,long len)
//unsigned char UART_CommandRoute(long len)
{
	unsigned int rx_wr_index = 0; //�����Ļ���дָ��	
	unsigned int loop_index; //ѭ��ָ��	
	unsigned char res;
	float f_temp;
	signed short int temp;
//	AfxMessageBox("UART_CommandRoute");	
	for(loop_index=0;loop_index<len;loop_index++)
	{
		res = com_rx_buffer[loop_index];
		if(res==0xa5) 
		{
			RC_Flag|=b_uart_head; //������յ�A5 ��λ֡ͷ��רλ
			data_buffer[rx_wr_index++]=res; //��������ֽ�.
		}
		else if(res==0x5a)
		{ 
			if(RC_Flag&b_uart_head) //�����һ���ֽ���A5 ��ô�϶� �����֡��ʼ�ֽ�
			{
				rx_wr_index=0;  //���� ������ָ��
				RC_Flag&=~b_rx_over; //���֡�Ÿոտ�ʼ��
			}
			else //��һ���ֽڲ���A5
				data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head; //��֡ͷ��־
		}
		else
		{ 
			data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head;
			if(rx_wr_index==data_buffer[0]) //�չ����ֽ���.
			{  
				rx_wr_index = 0;
				RC_Flag|=b_rx_over; //��λ ����������һ֡����
			}
		}

		if(rx_wr_index==RX_BUFFER_SIZE) //��ֹ���������
		rx_wr_index--;
	}


	if(RC_Flag&b_rx_over)
	{  //�Ѿ�������һ֡?

		RC_Flag&=~b_rx_over; //���־��
//		if(Sum_check(data_buffer))
		if(Sum_check())
		{ 
			
			if(data_buffer[1]==0xA3) //REALL  ����ALL������
			{
//			AfxMessageBox(_T("Sum_check success!"));	
				
				temp = 0;
				temp = data_buffer[2];
				temp <<= 8;
				temp |= data_buffer[3];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //ƫ����
				
			} 
		}//У���Ƿ�ͨ��?
		else
		{		
//			AfxMessageBox(_T("Sum_check Fail!"));	

		}
		return 1;
	}
	else 
		return 0;
}

*/
unsigned char CstationDlg::UART_CommandRoute(unsigned char * com_rx_buffer, long len)
{
	unsigned int rx_wr_index = 0; //�����Ļ���дָ��	
	unsigned int loop_index; //ѭ��ָ��	
	unsigned char res;
	float f_temp;
	signed short int temp=0;
	unsigned long int li_temp;
//	AfxMessageBox("UART_CommandRoute");	
	for(loop_index=0;loop_index<len;loop_index++)
	{
		res = com_rx_buffer[loop_index];
		if(res==0xa5) 
		{
			RC_Flag|=b_uart_head; //������յ�A5 ��λ֡ͷ��רλ
			data_buffer[rx_wr_index++]=res; //��������ֽ�.
		}
		else if(res==0x5a)
		{ 
			if(RC_Flag&b_uart_head) //�����һ���ֽ���A5 ��ô�϶� �����֡��ʼ�ֽ�
			{
				rx_wr_index=0;  //���� ������ָ��
				RC_Flag&=~b_rx_over; //���֡�Ÿոտ�ʼ��
			}
			else //��һ���ֽڲ���A5
				data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head; //��֡ͷ��־
		}
		else
		{ 
			data_buffer[rx_wr_index++]=res;
			RC_Flag&=~b_uart_head;
			if(rx_wr_index==data_buffer[0]) //�չ����ֽ���.
			{  
				rx_wr_index = 0;
				RC_Flag|=b_rx_over; //��λ ����������һ֡����
			}
		}

		if(rx_wr_index==RX_BUFFER_SIZE) //��ֹ���������
		rx_wr_index--;
	}


	if(RC_Flag&b_rx_over)
	{  //�Ѿ�������һ֡?

		RC_Flag&=~b_rx_over; //���־��
		if(Sum_check())
		{ 
			
			if(data_buffer[1]==0xA3) //REALL  ����ALL������
			{
//			AfxMessageBox(_T("Sum_check success!"));	
				
				temp=0;
				temp = data_buffer[2];//yaw
				temp <<= 8;
				temp |= data_buffer[3];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //ƫ����
				m_str_yaw.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[4];//pitch
				temp <<= 8;
				temp |= data_buffer[5];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //������
				m_str_pitch.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[6];//roll
				temp <<= 8;
				temp |= data_buffer[7];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //��ת��
				m_str_roll.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[8];//tempr
				temp <<= 8;
				temp |= data_buffer[9];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //�¶�
				m_str_tempr.Format(_T("%.1f"),f_temp);

				temp=0;
				temp = data_buffer[10];//press
				temp <<= 8;
				temp |= data_buffer[11];
				f_temp=(float)temp * 10.0f; //��ѹ
				m_str_press.Format(_T("%.0f"),f_temp);

				li_temp=0;
				li_temp = data_buffer[12];//longitude	
				li_temp <<= 8;
				li_temp |= data_buffer[13];
				li_temp <<= 8;
				li_temp |= data_buffer[14];
				li_temp <<= 8;
				li_temp |= data_buffer[15];
				f_temp=(float)li_temp / 100000.0f; //����
				m_str_lon.Format(_T("%.5f %1c   "),f_temp,data_buffer[16]);
				
				li_temp=0;
				li_temp = data_buffer[17];//latitude	
				li_temp <<= 8;
				li_temp |= data_buffer[18];
				li_temp <<= 8;
				li_temp |= data_buffer[19];
				li_temp <<= 8;
				li_temp |= data_buffer[20];
				f_temp=(float)li_temp / 100000.0f; //γ��
				m_str_lat.Format(_T("%.5f %1c   "),f_temp,data_buffer[21]);
				
				m_str_svnum.Format(_T("%d"),data_buffer[22]);//svnum		�ɼ�������

				temp=0;
				temp = data_buffer[23];//speed		
				temp <<= 8;
				temp |= data_buffer[24];
				f_temp=(float)temp / 1000.0f; //�ٶ�
				m_str_speed.Format(_T("%.3fkm/h"),f_temp);

				temp=0;
				temp = data_buffer[25];//altitude		
				temp <<= 8;
				temp |= data_buffer[26];
				if(temp&0x8000){
				temp = 0-(temp&0x7fff);
				}else temp = (temp&0x7fff);
				f_temp=(float)temp / 10.0f; //����
				m_str_altitud.Format(_T("%.1fm"),f_temp);

/*				str_rec_temp1.Format(_T("%d"),data_buffer[2]);
				str_rec_temp2.Format(_T("%d"),data_buffer[3]);
				str_rec_temp3.Format(_T("%d"),data_buffer[4]);
				str_rec_temp4.Format(_T("%d"),data_buffer[5]);
*/
/*				str_rec_temp1.Format(_T("%d"),data_buffer[2]*256+data_buffer[3]);//��ʾ�ĸ�int����
				str_rec_temp2.Format(_T("%d"),data_buffer[4]*256+data_buffer[5]);
				str_rec_temp3.Format(_T("%d"),data_buffer[6]*256+data_buffer[7]);
				str_rec_temp4.Format(_T("%d"),data_buffer[8]*256+data_buffer[9]);

*/	
				
			} 
		}//У���Ƿ�ͨ��?
		else
		{		
//			AfxMessageBox(_T("Sum_check Fail!"));	

		}
		return 1;
	}
	else 
		return 0;
}

