#ifndef _HID_DESCRIPTOR_
#define _HID_DESCRIPTOR_

/*
typedef class HIDDescriptor{
	uint8_t		bLength;
	uint8_t		bDescriptorType;
	uint16_t	bcdHID;
	uint8_t		bCountryCode;
	uint8_t		bNumDescriptor;
	uint8_t		bDescriptorTypeN;
	uint16_t	bDescriptorLengthN;
	uint8_t		*descriptors;
} hid_descritor_t;

#define COUNTRY_CODE_NotSupported	0
#define COUNTRY_CODE_Arabic 		1
#define COUNTRY_CODE_Belgian 		2
#define COUNTRY_CODE_CanadianBilingual 	3
#define COUNTRY_CODE_CanadianFrench 	4
#define COUNTRY_CODE_Czech Republic 	5
#define COUNTRY_CODE_Danish 		6
#define COUNTRY_CODE_Finnish 		7
#define COUNTRY_CODE_French 		8
#define COUNTRY_CODE_German 		9
#define COUNTRY_CODE_Greek 		10
#define COUNTRY_CODE_Hebrew 		11
#define COUNTRY_CODE_Hungary 		12
#define COUNTRY_CODE_International 	13
#define COUNTRY_CODE_Italian 		14
#define COUNTRY_CODE_Japan 		15
#define COUNTRY_CODE_Korean 		16
#define COUNTRY_CODE_Latin American 	17
#define COUNTRY_CODE_NetherlandsDutch 	18
#define COUNTRY_CODE_Norwegian 		19
#define COUNTRY_CODE_Persian		20
#define COUNTRY_CODE_Poland 		21
#define COUNTRY_CODE_Portuguese 	22
#define COUNTRY_CODE_Russia 		23
#define COUNTRY_CODE_Slovakia 		24
#define COUNTRY_CODE_Spanish 		25
#define COUNTRY_CODE_Swedish 		26
#define COUNTRY_CODE_SwissFrench 	27
#define COUNTRY_CODE_SwissGerman 	28
#define COUNTRY_CODE_Switzerland 		29
#define COUNTRY_CODE_Taiwan 		30
#define COUNTRY_CODE_TurkishQ 		31
#define COUNTRY_CODE_UK 			32
#define COUNTRY_CODE_US 			33
#define COUNTRY_CODE_Yugoslavia 		34
#define COUNTRY_CODE_TurkishF 		35
*/



/* for input,output,feature */
#define Data		0x00
#define Constant	0x01
#define Array		0x00
#define Variable	0x02
#define Absolute	0x00
#define Relative	0x04
#define NoWrap		0x00
#define Wrap		0x08
#define Linear		0x00
#define NonLinear	0x10
#define PreferedState	0x00
#define NoPrefered	0x20
#define NoNullPosition	0x00
#define NullState	0x40
#define BitField	0x00
#define BufferdBytes	0x100
#define NonVolatile	0x00
#define Volatile	0x80 

/* for collection */
#define Physical	0x00
#define Application	0x01
#define Logical		0x02
#define Report		0x03
#define NamedArray	0x04
#define UsageSwitch	0x05
#define UsageModifier	0x06



#define _Add0(b1,args...)	(b1+0)
#define _Add1(b1,args...)	_Add0(b1,args)
#define _Add2(b1,args...)	_Add1(b1,args)
#define _Add3(b1,args...)	_Add2(b1,args)
#define _Add4(b1,args...)	_Add3(b1,args)
#define _Add5(b1,args...)	_Add4(b1,args)
#define _Add6(b1,args...)	_Add5(b1,args)
#define _Add7(b1,args...)	_Add6(b1,args)
#define _Add8(b1,args...)	_Add7(b1,args)
#define _Add9(b1,args...)	_Add8(b1,args)
#define _Addn(b1,args...)	_Add9(b1,args)


/* Tags in Mian item */
#define Input(args...)			0x82,(_Addn(args))&0xff,(_Addn(args))>>8
#define Output(args...)			0x92,(_Addn(args))&0xff,(_Addn(args))>>8
#define Feature(args...)		0xb2,(_Addn(args))&0xff,(_Addn(args))>>8
#define Collection(args...)		0xa1,_Addn(args)
#define EndCollection(args...)		0xc0,_Addn(args)

/* Tags in Global item */
#define UsagePage(args...)		0x60,(_Addn(args))&0xff,(_Addn(args))>>8
#define LogicalMinimum(args...)		_Addn(args)
#define LogicalMaximum(args...)		_Addn(args)
#define PhysicalMinimum(args...)	_Addn(args)
#define PhysicalMaximum(args...)	_Addn(args)
#define UnitExponent(args...)		_Addn(args)
#define Unit(args...)			_Addn(args)
#define ReportSize(args...)		_Addn(args)
#define ReportID(args...)		_Addn(args)
#define ReportCount(args...)		_Addn(args)
#define Push(args...)			_Addn(args)
#define Pop(args...)			_Addn(args)
#define Reserved(args...)		_Addn(args)

/* Tags in Local item */
#define Usage(args...)			_Addn(args)
#define UsageMinimum(args...)		_Addn(args)
#define UsageMaximum(args...)		_Addn(args)
#define DesignatorIndex(args...)	_Addn(args)
#define DesignatorMinimum(args...)	_Addn(args)
#define DesignatorMaximum(args...)	_Addn(args)
#define StringIndex(args...)		_Addn(args)
#define StringMinimum(args...)		_Addn(args)
#define StringMaximum(args...)		_Addn(args)
#define Delimiter(args...)		_Addn(args)

#endif
