#include "MQTT.h"
#include "string.h"
#include "usart2.h"
#include "systick.h"


u8 txbuf[256]={0};
u8 rxbuf[256]={0};

//���ӳɹ���������Ӧ 20 02 00 00
//�ͻ��������Ͽ����� e0 00
const u8 parket_connetAck[] = {0x20,0x02,0x00,0x00};//�����ڽ����ַ�ʱ�����ҽ��յ�'\0'��ת���' '���洢�����Խ��յ��Ļ�Ӧ����0x20 02 0x20 0x20
const u8 parket_disconnet[] = {0xe0,0x00};
const u8 parket_subAck[] = {0x90,0x03};
const u8 parket_heart[] = {0xc0,0x00};
const u8 parket_heart_reply[] = {0xd0,0x00};//�����ڽ����ַ�ʱ�����ҽ��յ�'\0'��ת���' '���洢�����Խ��յ��Ļ�Ӧ����0xd0 0x20


static void Init(u8 *prx,u16 rxlen,u8 *ptx,u16 txlen);
static u8 Connect(char *ClientID,char *Username,char *Password);
static void Disconnect(void);
static u8 SubscribeTopic(char *topic,u8 qos,u8 whether);
static u8 PublishData(char *topic, char *message, u8 qos);
static void SentHeart(void);
static void SendData(u8* p,u16 len);


_typdef_mqtt _mqtt = 
{
	0,0,
	0,0,
	Init,
	Connect,
	Disconnect,
	SubscribeTopic,
	PublishData,
	SentHeart,
	SendData,
};


static u8 BYTE1(int num)
{
	return (u8)((num&0xFF00)>>8);
}
static u8 BYTE0(int num)
{
	return (u8)(num&0xFF);
}


// MQTT��ʼ��
static void Init(u8 *prx,u16 rxlen,u8 *ptx,u16 txlen)
{
	_mqtt.rxbuf = prx;_mqtt.rxlen = rxlen;
	_mqtt.txbuf = ptx;_mqtt.txlen = txlen;
	
//	memset(_mqtt.rxbuf,0,_mqtt.rxlen);
//	memset(_mqtt.txbuf,0,_mqtt.txlen);
//	
//	//�������������Ͽ�,����4�Σ���֤�ܶϿ�
//	_mqtt.Disconnect();Delayms(100);
//	_mqtt.Disconnect();Delayms(100);
//	_mqtt.Disconnect();Delayms(100);
//	_mqtt.Disconnect();Delayms(100);
}

/*
****************************************************************************************
* Function: Connect
* Description: ���ӷ������Ĵ������
* Input: ClientID���ͻ��˱�ʶ��
				 Username���û���
			   Password������
* Return: 1�ɹ� 0ʧ��
****************************************************************************************
*/
static uint8_t Connect(char *ClientID,char *Username,char *Password)
{
	int ClientIDLen = strlen(ClientID);
  int UsernameLen = strlen(Username);
  int PasswordLen = strlen(Password);
  int DataLen;
	_mqtt.txlen=0;
	//Variable Header(�ɱ䱨ͷ)+Payload(��Ч����)  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
  DataLen = 10 + (ClientIDLen+2) + (UsernameLen+2) + (PasswordLen+2);
	
	//�̶���ͷ
	//���Ʊ�������
  _mqtt.txbuf[_mqtt.txlen++] = 0x10;		//MQTT Message Type CONNECT
	//ʣ�೤��(�������̶�ͷ��)
	do
	{
		u8 encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		_mqtt.txbuf[_mqtt.txlen++] = encodedByte;
	}while ( DataLen > 0 );
    	
	//�ɱ䱨ͷ
	//Э����
    _mqtt.txbuf[_mqtt.txlen++] = 0;        		// Protocol Name Length MSB    
    _mqtt.txbuf[_mqtt.txlen++] = 4;        		// Protocol Name Length LSB    
		_mqtt.txbuf[_mqtt.txlen++] = 'M';        	// ASCII Code for M    
    _mqtt.txbuf[_mqtt.txlen++] = 'Q';        	// ASCII Code for Q    
    _mqtt.txbuf[_mqtt.txlen++] = 'T';        	// ASCII Code for T    
    _mqtt.txbuf[_mqtt.txlen++] = 'T';        	// ASCII Code for T    
	//Э�鼶��
    _mqtt.txbuf[_mqtt.txlen++] = 4;        		// MQTT Protocol version = 4    
	//���ӱ�־
    _mqtt.txbuf[_mqtt.txlen++] = 0xc2;        	// conn flags   
	//�������ʱ��
    _mqtt.txbuf[_mqtt.txlen++] = 0x01;        	// Keep-alive Time Length MSB    
    _mqtt.txbuf[_mqtt.txlen++] = 0xF4;        	// Keep-alive Time Length LSB  500S������  
	
    _mqtt.txbuf[_mqtt.txlen++] = BYTE1(ClientIDLen);// Client ID length MSB    
    _mqtt.txbuf[_mqtt.txlen++] = BYTE0(ClientIDLen);// Client ID length LSB  	
		memcpy(&_mqtt.txbuf[_mqtt.txlen],ClientID,ClientIDLen);
    _mqtt.txlen += ClientIDLen;
    
    if(UsernameLen > 0)
    {   
      _mqtt.txbuf[_mqtt.txlen++] = BYTE1(UsernameLen);		//username length MSB    
      _mqtt.txbuf[_mqtt.txlen++] = BYTE0(UsernameLen);    //username length LSB    
			memcpy(&_mqtt.txbuf[_mqtt.txlen],Username,UsernameLen);
      _mqtt.txlen += UsernameLen;
    }
    
    if(PasswordLen > 0)
    {    
      _mqtt.txbuf[_mqtt.txlen++] = BYTE1(PasswordLen);		//password length MSB    
      _mqtt.txbuf[_mqtt.txlen++] = BYTE0(PasswordLen);    	//password length LSB  
			memcpy(&_mqtt.txbuf[_mqtt.txlen],Password,PasswordLen);
      _mqtt.txlen += PasswordLen; 
    }  

		//printf("%s",_mqtt.txbuf);
		_mqtt.SendData(_mqtt.txbuf,_mqtt.txlen);
	
		u16 time=0;
		while(!ESP12rev.RevOver)//�ȴ�������ɱ�־
		{
			delay_ms(1);
			if(++time>500) //��ʱ
				break;
		}
		if(ESP12rev.RevOver)
		{
			ESP12rev.RevOver=0;//���־
			//if(rxbuf[0]==parket_connetAck[0] && rxbuf[1]==parket_connetAck[1]) //���ӳɹ�		
			if(ESP12rev.RevBuf[0]==parket_connetAck[0] && ESP12rev.RevBuf[1]==parket_connetAck[1]) //���ӳɹ�			  			
			{
				return 1;//���ӳɹ�
			}
		}		
		return 0;	
}

/*
****************************************************************************************
* Function: SubscribeTopic
* Description: MQTT����/ȡ���������ݴ��������
* Input: topic       ���� 
				 qos         ��Ϣ�ȼ� 
				 whether     1����/0ȡ�����������
* Return: 1�ɹ� 0ʧ��
****************************************************************************************
*/
static u8 SubscribeTopic(char *topic,u8 qos,u8 whether)
{    
	_mqtt.txlen=0;
    int topiclen = strlen(topic);
	
	int DataLen = 2 + (topiclen+2) + (whether?1:0);//�ɱ䱨ͷ�ĳ��ȣ�2�ֽڣ�������Ч�غɵĳ���
	//�̶���ͷ
	//���Ʊ�������
    if(whether) _mqtt.txbuf[_mqtt.txlen++] = 0x82; //��Ϣ���ͺͱ�־����
    else	_mqtt.txbuf[_mqtt.txlen++] = 0xA2;    //ȡ������
 
	//ʣ�೤��
	do
	{
		u8 encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		_mqtt.txbuf[_mqtt.txlen++] = encodedByte;
	}while ( DataLen > 0 );	
	
	//�ɱ䱨ͷ
    _mqtt.txbuf[_mqtt.txlen++] = 0;				//��Ϣ��ʶ�� MSB
    _mqtt.txbuf[_mqtt.txlen++] = 0x01;           //��Ϣ��ʶ�� LSB
	//��Ч�غ�
    _mqtt.txbuf[_mqtt.txlen++] = BYTE1(topiclen);//���ⳤ�� MSB
    _mqtt.txbuf[_mqtt.txlen++] = BYTE0(topiclen);//���ⳤ�� LSB   
		memcpy(&_mqtt.txbuf[_mqtt.txlen],topic,topiclen);
    _mqtt.txlen += topiclen;
    
    if(whether)
    {
        _mqtt.txbuf[_mqtt.txlen++] = qos;//QoS����
    }
	
		_mqtt.SendData(_mqtt.txbuf,_mqtt.txlen);
		
		u16 time=0;
		while(!ESP12rev.RevOver)//�ȴ�������ɱ�־
		{
			delay_ms(1);
			if(++time>500) //��ʱ
				break;
		}
		if(ESP12rev.RevOver)
		{
			ESP12rev.RevOver=0;//���־
			//if(_mqtt.rxbuf[0]==parket_subAck[0] && _mqtt.rxbuf[1]==parket_subAck[1]) //���ĳɹ�			
			if(ESP12rev.RevBuf[0]==parket_subAck[0] && ESP12rev.RevBuf[1]==parket_subAck[1]) //���ĳɹ�			  			
			{
				return 1;//���ĳɹ�
			}
		}
		
		return 0;
		
}

/*
****************************************************************************************
* Function: PublishData
* Description: MQTT�������ݴ��������
* Input: topic   ���� 
				 message ��Ϣ
				 qos     ��Ϣ�ȼ� 
* Return: ���ݰ�����
****************************************************************************************
*/
static uint8_t PublishData(char *topic, char *message, uint8_t qos)
{  
    int topicLength = strlen(topic);    
    int messageLength = strlen(message);     
    static u16 id=0;
	int DataLen;
	_mqtt.txlen=0;
	//��Ч�غɵĳ����������㣺�ù̶���ͷ�е�ʣ�೤���ֶε�ֵ��ȥ�ɱ䱨ͷ�ĳ���
	//QOSΪ0ʱû�б�ʶ��
	//���ݳ���             ������   ���ı�ʶ��   ��Ч�غ�
    if(qos)	DataLen = (2+topicLength) + 2 + messageLength;       
    else	DataLen = (2+topicLength) + messageLength;   
 
    //�̶���ͷ
	//���Ʊ�������
    _mqtt.txbuf[_mqtt.txlen++] = 0x30;    // MQTT Message Type PUBLISH  
 
	//ʣ�೤��
	do
	{
		u8 encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		_mqtt.txbuf[_mqtt.txlen++] = encodedByte;
	}while ( DataLen > 0 );	
	
    _mqtt.txbuf[_mqtt.txlen++] = BYTE1(topicLength);//���ⳤ��MSB
    _mqtt.txbuf[_mqtt.txlen++] = BYTE0(topicLength);//���ⳤ��LSB 
		memcpy(&_mqtt.txbuf[_mqtt.txlen],topic,topicLength);//��������
    _mqtt.txlen += topicLength;
        
	//���ı�ʶ��
    if(qos)
    {
        _mqtt.txbuf[_mqtt.txlen++] = BYTE1(id);
        _mqtt.txbuf[_mqtt.txlen++] = BYTE0(id);
        id++;
    }
		memcpy(&_mqtt.txbuf[_mqtt.txlen],message,messageLength);
    _mqtt.txlen += messageLength;
        
	_mqtt.SendData(_mqtt.txbuf,_mqtt.txlen);
    return _mqtt.txlen;
}

/*
****************************************************************************************
* Function: SentHeart
* Description: ��������
****************************************************************************************
*/
static void SentHeart(void)
{
	_mqtt.SendData((u8 *)parket_heart,sizeof(parket_heart));
}

/*
****************************************************************************************
* Function: Disconnect
* Description: ����Disconnect����
****************************************************************************************
*/
static void Disconnect(void)
{
	_mqtt.SendData((u8 *)parket_disconnet,sizeof(parket_disconnet));
}

/*
****************************************************************************************
* Function: SendData
* Description: �������ܣ�MQTT���ݰ����ͺ���
* Input: pָ������͵����ݰ������ݰ����ݳ���
****************************************************************************************
*/
static void SendData(u8* p,u16 len)
{
	while(len--)
	{
		while(USART_GetFlagStatus (USART2, USART_FLAG_TXE) == 0);  //�ȴ����ͻ�����Ϊ��
		USART_SendData(USART2, *p);
		p++;
	}
}
