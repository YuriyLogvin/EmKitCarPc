/* 
* ProtocolHost.h
*
* Created: 15.07.2013 20:06:39
* Author: Yuriy
*/


#ifndef __PROTOCOLHOST_H__
#define __PROTOCOLHOST_H__

#include "SendMetodHost.h"
#include "ReceiveMetodHost.h"

class ProtocolHost
{
    static const unsigned char _BroadcastAddress = 0;
    unsigned char _Address;
    static const unsigned char _CharStart = 0xff;
    static const unsigned char _CharEsc = 0xfe;
   
    typedef enum 
    {
	    None,
	    Address,
	    DataLength,
	    Data,
	    CheckSum0,
	    CheckSum1,
	    Finished
    } TransferingState;
	
    TransferingState _ReceiveState;
    
	unsigned char* _DataBuffer;
	short _DataBufferLength;
	unsigned char _DataBufferPosition;
	unsigned char _DataPacketLength;
	bool _EscSymvol;
	unsigned short _CheckSum;
    
	TransferingState _SendingState;
	unsigned char* _RefPointSendedDataBuff;
	unsigned char _RefPointSendedDataBuffLength;
	unsigned char _RefPointSendedDataBuffPosition;
	unsigned char _SendEscChar;
	unsigned short _RefPointSendedDataBuffCrc;

	void _SendData(unsigned char inData, unsigned char& data);
public:
	ProtocolHost(unsigned char address);
	~ProtocolHost();
	
	/// if packet receive - return received data
	unsigned char* ReceiveData(unsigned char data, unsigned char& dataLength);
	
	void InitSendData(unsigned char* data, unsigned char dataLength);
	
	void InitSendData(SendMetodHost& smHost);
	 
	bool SendData(unsigned char& data);
	
protected:
private:
	ProtocolHost( const ProtocolHost &c );
	ProtocolHost& operator=( const ProtocolHost &c );

}; //ProtocolHost

#endif //__PROTOCOLHOST_H__
