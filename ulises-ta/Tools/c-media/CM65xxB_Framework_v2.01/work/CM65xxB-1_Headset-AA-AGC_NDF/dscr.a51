#include "config.h"
$SET (__NUCLEODF__)	  				;; Version de NUCLEO...

DSCR_DEVICE           	equ    1   ;; Descriptor type: Device
DSCR_CONFIG           	equ    2   ;; Descriptor type: Configuration
DSCR_STRING           	equ    3   ;; Descriptor type: String
DSCR_INTRFC           	equ    4   ;; Descriptor type: Interface
DSCR_ENDPNT           	equ    5   ;; Descriptor type: Endpoint

DSCR_CS_INTERFACE   	equ    24H
DSCR_CS_ENDPOINT    	equ    25H

AC_SUBTYPE_HEADER		equ    	1
AC_SUBTYPE_IT			equ    	2
AC_SUBTYPE_OT			equ    	3
AC_SUBTYPE_MIXER		equ    	4
AC_SUBTYPE_SELECTOR		equ    	5
AC_SUBTYPE_FEATURE		equ    	6

AS_SUBTYPE_GENERAL    	equ    1
AS_SUBTYPE_FORMAT     	equ    2
EP_SUBTYPE_GENERAL    	equ    1

DSCR_DEVICE_LEN       	equ		18
DSCR_CONFIG_LEN       	equ		9
DSCR_INTRFC_LEN       	equ		9
DSCR_ENDPNT_LEN       	equ		7
DSCR_HID_LEN			equ		9

DSCR_AC_IT_LEN   		equ    12
DSCR_AC_OT_LEN   		equ     9
DSCR_AS_GENERAL_LEN  	equ   	7
DSCR_EP_GENERAL_LEN  	equ   	7

ET_CONTROL            	equ    0   ;; Endpoint type: Control
ET_ISO                	equ    1   ;; Endpoint type: Isochronous
ET_BULK               	equ    2   ;; Endpoint type: Bulk
ET_INT                	equ    3   ;; Endpoint type: Interrupt

public DeviceDscr
public T1_ConfigDscr, T1_HidDscr, Tx_HidReportDscr
public MixerTable

$IF (__NUCLEODF__)
	public ProductStringDscr, LangIdStringDscr, ManufactureStringDscr, VersionStringDscr
	public ProductStringDscr_01, ProductStringDscr_02, ProductStringDscr_03, ProductStringDscr_04;
$ELSE
	public ProductStringDscr, LangIdStringDscr, ManufactureStringDscr, VersionStringDscr
$ENDIF
 
CSEG AT 0A290H

DeviceDscr:   
	db	DSCR_DEVICE_LEN   	;; Descriptor length
	db  DSCR_DEVICE       	;; Decriptor type
	dw  1001H             	;; Specification Version (BCD)
	db  00H               	;; Device class
	db  00H               	;; Device sub-class
	db  00H               	;; Device sub-sub-class
	db  16                	;; Maximum packet size
	dw  8C0DH             	;; Vendor ID
	dw  7001H             	;; Product ID
	dw  0000H             	;; Product version ID
	db  1                 	;; Manufacturer string index
	db  2                 	;; Product string index
	db  0                 	;; Serial number string index
	db  1                 	;; Number of configurations

org (($ / 2) +1) * 2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; T1 ==> Headset Topology
T1_ConfigDscr:
	db	DSCR_CONFIG_LEN		;; Descriptor length
	db	DSCR_CONFIG				;; Descriptor type
	db	(T1_ConfigDscrEnd - T1_ConfigDscr) mod 256	;; Total Length (LSB)
	db	(T1_ConfigDscrEnd - T1_ConfigDscr)  /  256	;; Total Length (MSB)
	db	4					;; Number of interfaces
	db	1					;; Configuration number
	db	0					;; Configuration string
	db	10000000b			;; Attributes (b7 - reserved, b6 - selfpwr, b5 - rwu)
	db	50						;; Power requirement (div 2 mA)

;; Audio Control Interface 0 Descriptor	
	db  DSCR_INTRFC_LEN   	;; Descriptor length
	db  DSCR_INTRFC       	;; Descriptor type
	db  0                 	;; Zero-based index of this interface
	db  0                 	;; Alternate setting
	db  0                 	;; Number of end points 
	db  01H               	;; Interface class
	db  01H               	;; Interface sub class
	db  00H               	;; Interface sub sub class
	db  0                 	;; Interface descriptor string index	

T1_ACIntrfcDscrHeader:
	db  (T1_ACIntrfcDscrHeaderEnd - T1_ACIntrfcDscrHeader)	;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_HEADER 	;; AC Descriptor subtype
	dw  0001H             	;; Audio class release number
	db  (T1_ACInterfaceDscrEnd - T1_ACIntrfcDscrHeader) mod 256	;; Total Length (LSB)
	db  (T1_ACInterfaceDscrEnd - T1_ACIntrfcDscrHeader)  /  256	;; Total Length (MSB)
	db  02H               	;; Number of AS interfaces
	db  01H               	;; AS interface number 1
	db  02H               	;; AS interface number 2
T1_ACIntrfcDscrHeaderEnd:

;; USB Audio10 Input Terimal 1 Descriptor
	db  DSCR_AC_IT_LEN    	;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_IT     	;; AC Descriptor subtype
	db  01H               	;; ID
	dw  0101H             	;; Terimal type: USB streaming
	db  0                 	;; Associate terminal
	db  02H               	;; Number of channels
	dw  0300H             	;; Config of logical channels
	db  0                 	;; String index of first logical channel
	db  0                 	;; String index of IT		
		
;; USB Audio10 Input Terimal 2 Descriptor
	db  DSCR_AC_IT_LEN    	;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_IT     	;; AC Descriptor subtype
	db  02H               	;; ID
	dw  0102H             	;; Terimal type: Microphone
	db  0                 	;; Associate terminal
	db  02H               	;; Number of channels
	dw  0300H             	;; Config of logical channels
	db  0                 	;; String index of first logical channel
	db  0                 	;; String index of IT		
	
;; USB Audio10 Output Terimal 3 Descriptor
	db  DSCR_AC_OT_LEN    	;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_OT     	;; AC Descriptor subtype
	db  03H               	;; ID
	dw  0203H             	;; Terimal type: Headphone
	db  0                 	;; Associate terminal
	db  05H               	;; Source ID
	db  0                		;; String index of OT	
	
;; USB Audio10 Output Terimal 4 Descriptor
	db  DSCR_AC_OT_LEN    	;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_OT     	;; AC Descriptor subtype
	db  04H               	;; ID
	dw  0101H             	;; Terimal type: USB streaming
	db  0                 	;; Associate terminal
	db  08H               	;; Source ID
	db  0                		;; String index of OT

;; USB Audio10 Feature Unit 5 Descriptor	
T1_FeatureUnit5Dscr:
	db  T1_FeatureUnit5DscrEnd - T1_FeatureUnit5Dscr ;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_FEATURE	;; AC Descriptor subtype
	db  05H               	;; ID
	db  09H               	;; Source ID
	db  01H               	;; Size of control byte
	db  01H               	;; Control byte of channel 0
	db  02H               	;; Control byte of channel 1
	db  02H               	;; Control byte of channel 2
	db  0                 	;; String index of Feature
T1_FeatureUnit5DscrEnd:
	
;; USB Audio10 Feature Unit 6 Descriptor	
T1_FeatureUnit6Dscr:
	db  T1_FeatureUnit6DscrEnd - T1_FeatureUnit6Dscr ;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_FEATURE	;; AC Descriptor subtype
	db  06H               	;; ID
	db  02H               	;; Source ID
	db  01H               	;; Size of control byte
	db  41H               	;; Control byte of channel 0
	db  02H               	;; Control byte of channel 1
	db  02H               	;; Control byte of channel 2
	db  0                 	;; String index of Feature
T1_FeatureUnit6DscrEnd:
	
;; USB Audio10 Feature Unit 7 Descriptor	
T1_FeatureUnit7Dscr:
	db  T1_FeatureUnit7DscrEnd - T1_FeatureUnit7Dscr ;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AC_SUBTYPE_FEATURE	;; AC Descriptor subtype
	db  07H               	;; ID
	db  02H               	;; Source ID
	db  01H               	;; Size of control byte
	db  01H               	;; Control byte of channel 0
	db  02H               	;; Control byte of channel 1
	db  02H               	;; Control byte of channel 2
	db  0                 	;; String index of Feature
T1_FeatureUnit7DscrEnd:

;; USB Audio10 Selector Unit 8 Descriptor	
T1_SelectorUnit8Dscr:
	db	T1_SelectorUnit8DscrEnd - T1_SelectorUnit8Dscr     ;; Descriptor length
	db  DSCR_CS_INTERFACE   ;; Descriptor type
	db  AC_SUBTYPE_SELECTOR ;; AC Descriptor subtype
	db  08H      			;; ID
	db  01H      			;; Number of input pins
	db  06H      			;; ID of source 0
	db  0        			;; String index of Selector
T1_SelectorUnit8DscrEnd:

;; USB Audio10 Mixer Unit 9 Descriptor	
T1_MixerUnit9Dscr:
	db	T1_MixerUnit9DscrEnd - T1_MixerUnit9Dscr ;; Descriptor length
	db  DSCR_CS_INTERFACE		;; Descriptor type
	db  AC_SUBTYPE_MIXER		;; AC Descriptor subtype
	db  09H               	;; ID
	db  02H               	;; input pin
	db  01H 								;; Source ID
	db  07H               	;; Source ID
	db  02H               	;; Number of logical output channel
	dw  0300H 							;; Config of logical channels
	db  00H                 ;; String index of Mixer
	db  00H									;; bmcontrol
	db  00H                 ;; String index of Mixer
T1_MixerUnit9DscrEnd:

T1_ACInterfaceDscrEnd:

;; USB Audio10 Audio Stream Interface 1 (IT 1) alternate 0 Descriptor
	db	DSCR_INTRFC_LEN   	;; Descriptor length
	db	DSCR_INTRFC       	;; Descriptor type
	db  1                 	;; Zero-based index of this interface
	db  0                 	;; Alternate setting
	db  0                 	;; Number of end points 
	db  01H               	;; Interface class
	db  02H               	;; Interface sub class
	db  00H               	;; Interface sub sub class
	db  0                 	;; Interface descriptor string index
      
;; USB Audio10 Audio Stream Interface 1 (IT 1) alternate 1 Descriptor
	db  DSCR_INTRFC_LEN   	;; Descriptor length
	db  DSCR_INTRFC       	;; Descriptor type
	db  1                 	;; Zero-based index of this interface
	db  1                 	;; Alternate setting
	db  1                 	;; Number of end points 
	db  01H               	;; Interface class
	db  02H               	;; Interface sub class
	db  00H               	;; Interface sub sub class
	db  0                 	;; Interface descriptor string index
      
;; USB Audio10 AS General Interface Descriptor
	db  DSCR_AS_GENERAL_LEN ;; Descriptor length
	db  DSCR_CS_INTERFACE   ;; Descriptor type
	db  AS_SUBTYPE_GENERAL  ;; AS Descriptor subtype
	db  01H                 ;; Terminal ID link to
	db  01H                 ;; Sync delay
	dw  0100H               ;; Format type: 16bit PCM
      
T1_AS1FormatDscr:
	db  (T1_AS1FormatDscrEnd - T1_AS1FormatDscr) ;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AS_SUBTYPE_FORMAT 	;; AS Descriptor subtype
	db  01H               	;; Format type
	db  02H               	;; Number of channels
	db  02H               	;; Byte per sample
	db  10H               	;; Bit per sample
	db  07H               	;; Number of sampling rate
	db	40H									;; sampling rate 8K	
	db	1FH									;;
	db	00H									;;
	db	11H									;; sampling rate 11.025K
	db	2BH									;;
	db	00H									;;
	db	80H									;; sampling rate 16K
	db	3EH									;;
	db	00H									;;
	db	22H									;; sampling rate 22.05K
	db	56H									;;
	db	00H									;;
	db	00H									;; sampling rate 32K
	db	7DH									;;
	db	00H									;;
	db  44H									;; sampling rate 44.1K
	db  0ACH								;;
	db  00H									;;
	db  80H									;; sampling rate 48K
	db  0BBH								;;
	db  00H									;;
T1_AS1FormatDscrEnd:

;; USB Audio10 Endpoint Descriptor
	db  DSCR_ENDPNT_LEN+2 	;; Descriptor length
	db  DSCR_ENDPNT       	;; Descriptor type
	db  01H               	;; Endpoint number, and direction
	db  0DH               	;; Endpoint type: ISO & Synchronous
	db  2*2*(48+1) mod 256  ;; Maximun packet size (LSB)
	db  2*2*(48+1)  /  256  ;; Maximun packet size (MSB)
	db  01H               	;; Polling interval
	db  00H               	;; Must be 0
	db  00H               	;; No sync pipe
	
;; USB Audio10 AS General Endpoint Descriptor
	db  DSCR_EP_GENERAL_LEN ;; Descriptor length
	db  DSCR_CS_ENDPOINT    ;; Descriptor type
	db  EP_SUBTYPE_GENERAL  ;; EP Descriptor subtype
	db  01H                 ;; Attribute
	db  00H                 ;; 
	dw  0                   ;; Format type: 16bit PCM

;; USB Audio10 Audio Stream Interface 2 (OT 4) alternate 0 Descriptor
	db	DSCR_INTRFC_LEN   	;; Descriptor length
	db	DSCR_INTRFC       	;; Descriptor type
	db  2                 	;; Zero-based index of this interface
	db  0                 	;; Alternate setting
	db  0                 	;; Number of end points 
	db  01H               	;; Interface class
	db  02H               	;; Interface sub class
	db  00H               	;; Interface sub sub class
	db  0                 	;; Interface descriptor string index
      
;; USB Audio10 Audio Stream Interface 2 (OT 4) alternate 1 Descriptor
	db  DSCR_INTRFC_LEN   	;; Descriptor length
	db  DSCR_INTRFC       	;; Descriptor type
	db  2                 	;; Zero-based index of this interface
	db  1                 	;; Alternate setting
	db  1                 	;; Number of end points 
	db  01H               	;; Interface class
	db  02H               	;; Interface sub class
	db  00H               	;; Interface sub sub class
	db  0                 	;; Interface descriptor string index    
      
;; USB Audio10 AS General Interface Descriptor
	db  DSCR_AS_GENERAL_LEN ;; Descriptor length
	db  DSCR_CS_INTERFACE   ;; Descriptor type
	db  AS_SUBTYPE_GENERAL  ;; AS Descriptor subtype
	db  04H                 ;; Terminal ID link to
	db  01H                 ;; Sync delay
	dw  0100H               ;; Format type: 16bit PCM        
      
T1_AS2FormatDscr:
	db  (T1_AS2FormatDscrEnd - T1_AS2FormatDscr) ;; Descriptor length
	db  DSCR_CS_INTERFACE 	;; Descriptor type
	db  AS_SUBTYPE_FORMAT 	;; AS Descriptor subtype
	db  01H               	;; Format type
	db  02H               	;; Number of channels
	db  02H               	;; Byte per sample
	db  10H               	;; Bit per sample
	db  07H               	;; Number of sampling rate
	db	40H									;; sampling rate 8K	
	db	1FH									;;
	db	00H									;;
	db	11H									;; sampling rate 11.025K
	db	2BH									;;
	db	00H									;;
	db	80H									;; sampling rate 16K
	db	3EH									;;
	db	00H									;;
	db	22H									;; sampling rate 22.05K
	db	56H									;;
	db	00H									;;
	db	00H									;; sampling rate 32K
	db	7DH									;;
	db	00H									;;
	db  44H									;; sampling rate 44.1K
	db  0ACH								;;
	db  00H									;;
	db  80H									;; sampling rate 48K
	db  0BBH								;;
	db  00H									;;
T1_AS2FormatDscrEnd:	

;; USB Audio10 Endpoint Descriptor
	db  DSCR_ENDPNT_LEN+2 	;; Descriptor length
	db  DSCR_ENDPNT       	;; Descriptor type
	db  82H               	;; Endpoint number, and direction
	db  0DH               	;; Endpoint type: ISO & Synchronous
	db  2*2*(48+1) mod 256  ;; Maximun packet size (LSB)
	db  2*2*(48+1)  /  256  ;; Maximun packet size (MSB)
	db  01H               	;; Polling interval
	db  00H               	;; Must be 0
	db  00H               	;; No sync pipe

;; USB Audio10 AS General Endpoint Descriptor
	db  DSCR_EP_GENERAL_LEN ;; Descriptor length
	db  DSCR_CS_ENDPOINT    ;; Descriptor type
	db  EP_SUBTYPE_GENERAL  ;; EP Descriptor subtype
	db  01H                 ;; Attribute
	db  00H                 ;; 
	dw  0                   ;; Format type: 16bit PCM

;; Full Speed HID Interface 3 Descriptor
	db	DSCR_INTRFC_LEN     ;; Descriptor length
	db  DSCR_INTRFC         ;; Descriptor type
	db  3										;; Zero-based index of this interface
	db  0                   ;; Alternate setting
	db  1                   ;; Number of end points 
	db  03H                 ;; Interface class
	db  00H                 ;; Interface sub class
	db  00H                 ;; Interface sub sub class
	db  0                   ;; Interface descriptor string index.

;; Full Speed HID Descriptor
T1_HidDscr:
	db	DSCR_HID_LEN        ;; Descriptor length
	db  21H               	;; Descriptor type
	dw  0001              	;; HID version
	db  0                 	;; Country code
	db  1                 	;; Number of descriptors
	db  22H               	;; Descriptor type: Report
	db  (Tx_HidReportDscrEnd-Tx_HidReportDscr) mod 256 ;; Length of descriptor (LSB)
	db  (Tx_HidReportDscrEnd-Tx_HidReportDscr)  /  256 ;; Length of descriptor (MSB)

;; Full Speed Endpoint Descriptor
	db 	DSCR_ENDPNT_LEN   	;; Descriptor length
	db  DSCR_ENDPNT       	;; Descriptor type
	db  87H              	;; Endpoint number, and direction
	db  ET_INT            	;; Endpoint type
	db  10H               	;; Maximun packet size (LSB) 
	db  00H               	;; Maximun packet size (MSB)
	db  01H               	;; Polling interval

T1_ConfigDscrEnd:

org (($ / 2) +1) * 2

LangIdStringDscr:
	db  LangIdStringDscrEnd-LangIdStringDscr ;; String descriptor length
	db  DSCR_STRING
	db	09H
	db	04H
LangIdStringDscrEnd:

$IF(__NUCLEODF__)
	ManufactureStringDscr:
		db  ManufactureStringDscrEnd-ManufactureStringDscr ;; String descriptor length
		db  DSCR_STRING
		db  'N',00
		db	'U',00
		db  'C',00
		db  'L',00
		db  'E',00
		db  'O',00
		db  ' ',00	
		db	'D',00
		db	'F',00
	ManufactureStringDscrEnd:
	
	ProductStringDscr:
		db  ProductStringDscrEnd-ProductStringDscr ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'W',00
		db  'P',00
		db  ' ',00
		db  'U',00
		db  'S',00
		db  'B',00
		db  ' ',00
		db  'D',00
		db  'e',00
		db  'v',00
		db  'i',00
		db	'c',00
		db	'e',00
		db	' ',00
		db	'?',00
		db	'?',00
	ProductStringDscrEnd:
	
	; AGL......
	ProductStringDscr_01:
		db  ProductStringDscr_01_End-ProductStringDscr_01 ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'W',00
		db  'P',00
		db  ' ',00
		db  'U',00
		db  'S',00
		db  'B',00
		db  ' ',00
		db  'D',00
		db  'e',00
		db  'v',00
		db  'i',00
		db	'c',00
		db	'e',00
		db	' ',00
		db	'#',00
		db	' ',00
		db	'0',00
		db	'1',00
	ProductStringDscr_01_End:
	
	ProductStringDscr_02:
		db  ProductStringDscr_02_End-ProductStringDscr_02 ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'W',00
		db  'P',00
		db  ' ',00
		db  'U',00
		db  'S',00
		db  'B',00
		db  ' ',00
		db  'D',00
		db  'e',00
		db  'v',00
		db  'i',00
		db	'c',00
		db	'e',00
		db	' ',00
		db	'#',00
		db	' ',00
		db	'0',00
		db	'2',00
	ProductStringDscr_02_End:
	
	ProductStringDscr_03:
		db  ProductStringDscr_03_End-ProductStringDscr_03 ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'W',00
		db  'P',00
		db  ' ',00
		db  'U',00
		db  'S',00
		db  'B',00
		db  ' ',00
		db  'D',00
		db  'e',00
		db  'v',00
		db  'i',00
		db	'c',00
		db	'e',00
		db	' ',00
		db	'#',00
		db	' ',00
		db	'0',00
		db	'3',00
	ProductStringDscr_03_End:
	
	ProductStringDscr_04:
		db  ProductStringDscr_04_End-ProductStringDscr_04 ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'W',00
		db  'P',00
		db  ' ',00
		db  'U',00
		db  'S',00
		db  'B',00
		db  ' ',00
		db  'D',00
		db  'e',00
		db  'v',00
		db  'i',00
		db	'c',00
		db	'e',00
		db	' ',00
		db	'#',00
		db	' ',00
		db	'0',00
		db	'4',00
	ProductStringDscr_04_End:
$ELSE
	ManufactureStringDscr:
		db  ManufactureStringDscrEnd-ManufactureStringDscr ;; String descriptor length
		db  DSCR_STRING
		db  'C',00
		db	'-',00
		db  'M',00
		db  'e',00
		db  'd',00
		db  'i',00
		db  'a',00	
		db	' ',00
		db	'E',00
		db	'l',00
		db	'e',00
		db	'c',00
		db	't',00
		db	'r',00
		db	'o',00
		db	'n',00
		db	'i',00
		db	'c',00
		db	's',00 
		db  ' ',00
		db	'I',00
		db	'n',00
		db	'c',00
		db	'.',00
	ManufactureStringDscrEnd:
	
	ProductStringDscr:
		db  ProductStringDscrEnd-ProductStringDscr ;; String descriptor length
		db  DSCR_STRING
		db  'U',00
		db	'S',00
		db  'B',00
		db  ' ',00
		db  'A',00
		db  'd',00
		db  'v',00
		db  'a',00
		db  'n',00
		db  'c',00
		db  'e',00
		db  'd',00
		db	' ',00
		db	'A',00
		db	'u',00
		db	'd',00
		db	'i',00
		db	'o',00
		db	' ',00
		db	'D',00
		db	'e',00
		db	'v',00
		db	'i',00
		db	'c',00
		db	'e',00 	
	ProductStringDscrEnd:
$ENDIF

VersionStringDscr:
	db	VersionStringDscrEnd-VersionStringDscr
	db	DSCR_STRING
	db	'5', 00
	db	'0', 00
	db	'2', 00
	db	'3', 00
	db	'C', 00
VersionStringDscrEnd:

org (($ / 2) +1) * 2

Tx_HidReportDscr:
      db   05H, 0CH						;;Usage Page(Consumer)
      db   09H, 01H						;;Usage(Consumer Control)
      db   0A1H, 01H					;;Collection(Application)
      db   85H, 01H						;;Report ID(1)
      db   15H, 00H						;;Logical Min.(0)
      db   25H, 01H						;;Logical Max.(1)
      db   09H, 00H						;;Usage(Undefined)         
      db   09H, 00H						;;Usage(Undefined)         
      db   75H, 01H						;;Report Size(1)
      db   95H, 02H						;;Report Count(2)
      db   81H, 42H						;;Input(Data, Variable, Absolute, Null state)
;	  db   09H, 0E2H					;;Usage(Mute)
	  db   09H, 00H					;;Usage(Mute)
      db   95H, 01H						;;Report Count(1)
      db   81H, 06H						;;Input(Data, Variable, Relative)      
      db   06H, 01H, 0FFH				;;Usage Page(Vendor Defined)
      db   09H, 01H						;;Usage(Vendor1 ??)
      db   95H, 09H						;;Report Count (9)
      db   81H, 06H						;;Input(Data, Variable, Relative)
      db   05H, 0CH						;;Usage Page(Consumer)
      db   09H, 0CDH					;;Usage(Play/Pause)
      db   09H, 0B7H					;;Usage(Stop)
      db   09H, 0B5H					;;Usage(Scan Next Track)
      db   09H, 0B6H					;;Usage(Scan Previous Track)
      db   95H, 04H						;;Report Count(4)
      db   81H, 06H						;;Input(Data, Variable, Relative)
      db   06H, 07H, 0FFH				;;Usage Page(Vendor Defined)
      db   09H, 01H						;;Usage(Vendor1 ??)
      db   75H, 08H						;;Report Size(8)
	  db   95H, 0DH						;;Report Count(13)
      db   81H, 06H						;;Input(Data, Variable, Relative)
      db   09H, 00H						;;Usage(Undefined)
      db   95H, 0FH						;;Report Count(15)
      db   91H, 02H						;;Output(Data, Variable, Absolute)
      db   0C0H							;;End Collection
Tx_HidReportDscrEnd:

org (($ / 2) +1) * 2

MixerTable:
		dw 0000H
		dw 0080H
		dw 0080H
		dw 0000H
		dw 0000H
		dw 0080H
		dw 0080H
		dw 0000H
		dw 0000H
		dw 0080H
		dw 0080H
		dw 0000H
MixerTableEnd:	

org (($ / 2) +1) * 2
	dw CHIP_ID							;; Chip ID
	dw VENDOR_ID						;; Vendor ID
	dw PRODUCT_ID						;; Product ID
	dw CHIP_VERSION					;; Chip version ID
	dw VERSION_ID						;; Firmware version ID
	dw EEPROM_PARAMETER
	
end