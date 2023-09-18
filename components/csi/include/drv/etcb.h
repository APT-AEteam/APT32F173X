/***********************************************************************//** 
 * \file  lpt.c
 * \brief  csi lpt driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2017-10-27<td>V0.0 <td>Alibaba <td> initial
 * <tr><td> 2021-1-8  <td>V0.1 <td>WNN     <td> SWTRG
 * <tr><td> 2021-5-14 <td>V0.1 <td>ZJY     <td> initial
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_ETCB_H_
#define _DRV_ETCB_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif
#define ETCB_CH_ALLOC_LEN		32
/**
 * \enum     csi_etcb_trig_mode_e
 * \brief    ETCB channel trigger mode
 */
typedef enum {
    ETCB_HARDWARE_TRG  	= 0,	//etcb channel inout is hardware trigger.
    ETCB_SOFTWARE_TRG            //etcb channel inout is software trigger.
} csi_etcb_trg_mode_e;

/**
 * \enum     csi_etcb_ch_type_e
 * \brief    ETCB channel trigger type
 */
typedef enum {
    ETCB_ONE_TRG_ONE	= 0,      //one device trig one deivce, 3~31channel
    ETCB_ONE_TRG_MORE,         //one device trig two for more device, 0~2channel
	ETCB_ONE_TRG_ONE_DMA       //one device trig one deivce of dma, 20->31channel
} csi_etcb_ch_type_e;

/**
 * \enum     csi_etcb_ch_e
 * \brief    ETCB channel number(number 0~31)
 */
typedef enum {
    ETCB_CH0			= 0,		//etcb channel 0 id number
	ETCB_CH1,					//etcb channel 1 id number
	ETCB_CH2,					//etcb channel 2 id number
	ETCB_CH3,					//etcb channel 3 id number
	ETCB_CH4,
	ETCB_CH5,
	ETCB_CH6,
	ETCB_CH7,
	ETCB_CH8,			
	ETCB_CH9,						
	ETCB_CH10,						
	ETCB_CH11,						
	ETCB_CH12,
	ETCB_CH13,
	ETCB_CH14,
	ETCB_CH15,
	ETCB_CH16,
	ETCB_CH17,
	ETCB_CH18,						
	ETCB_CH19,						
	ETCB_CH20,					//etcb channel 20, dma start channel						
	ETCB_CH21,
	ETCB_CH22,
	ETCB_CH23,
	ETCB_CH24,
	ETCB_CH25,			
	ETCB_CH26,						
	ETCB_CH27,						
	ETCB_CH28,						
	ETCB_CH29,
	ETCB_CH30,
	ETCB_CH31
} csi_etcb_ch_e;

/**
 * \enum     csi_etcb_src_e
 * \brief    ETCB event source
 */
typedef enum{
	ETCB_LPT_TRGOUT0 	= 0,
	ETCB_EXI_TRGOUT0 	= 4,
	ETCB_EXI_TRGOUT1,
	ETCB_EXI_TRGOUT2,	
	ETCB_EXI_TRGOUT3,

	ETCB_RTC_TRGOUT0		= 8	,
	ETCB_RTC_TRGOUT1,
	ETCB_BT0_TRGOUT,
	ETCB_BT1_TRGOUT,
	ETCB_BT2_TRGOUT,
	ETCB_BT3_TRGOUT,

	ETCB_EXI_TRGOUT4     = 19,	
	ETCB_EXI_TRGOUT5,	
	
	ETCB_ADC0_TRGOUT0	= 30,
	ETCB_ADC0_TRGOUT1,
	ETCB_ADC1_TRGOUT0,
	ETCB_ADC1_TRGOUT1,
	
	ETCB_CMP0_TRGOUT		= 38,
	ETCB_CMP1_TRGOUT,
	ETCB_CMP2_TRGOUT,
	
	ETCB_GPTA0_TRGOUT0 	= 47,
	ETCB_GPTA0_TRGOUT1,
	ETCB_GPTA1_TRGOUT0,
	ETCB_GPTA1_TRGOUT1,
	ETCB_GPTA2_TRGOUT0,
	ETCB_GPTA2_TRGOUT1,
	ETCB_GPTA3_TRGOUT0,
	ETCB_GPTA3_TRGOUT1,
	
	ETCB_GPTB0_TRGOUT0 	= 63,
	ETCB_GPTB0_TRGOUT1,
	ETCB_GPTB1_TRGOUT0,
	ETCB_GPTB1_TRGOUT1,
	ETCB_GPTB2_TRGOUT0,
	ETCB_GPTB2_TRGOUT1,
	ETCB_GPTB3_TRGOUT0,
	ETCB_GPTB3_TRGOUT1,
	ETCB_GPTB4_TRGOUT0,
	ETCB_GPTB4_TRGOUT1,
	ETCB_GPTB5_TRGOUT0,
	ETCB_GPTB5_TRGOUT1,
	ETCB_IIC0_TXSRC,
	ETCB_IIC0_RXSRC,
	
	ETCB_SPI0_TXSRC 		= 81,
	ETCB_SPI0_RXSRC,
	ETCB_SPI1_TXSRC,
	ETCB_SPI1_RXSRC,
	
	ETCB_SIO0_TXSRC 		= 87,
	ETCB_SIO0_RXSRC,
	ETCB_SIO1_TXSRC,
	ETCB_SIO1_RXSRC,
	
	ETCB_USART0_TXSRC 	= 93,
	ETCB_USART0_RXSRC,
	ETCB_USART1_TXSRC,
	ETCB_USART1_RXSRC,
	ETCB_UART0_TXSRC		= 97,
	ETCB_UART0_RXSRC,
	ETCB_UART1_TXSRC,
	ETCB_UART1_RXSRC,
	ETCB_UART2_TXSRC,
	ETCB_UART2_RXSRC,
	
	ETCB_CAN_TXSRC       = 107,
	ETCB_CAN_RXSRC,
	
	ETCB_DAC0_TRGOUT 	= 111,
	
	ETCB_SRC_NOT_USE     = 0xff
}csi_etcb_src_e;

/**
 * \enum     csi_etcb_dst_e
 * \brief    ETCB description event 
 */
typedef enum{
	ETCB_LPT_SYNCIN 		= 0,
	ETCB_BT0_SYNCIN0,
	ETCB_BT0_SYNCIN1,
	ETCB_BT0_SYNCIN2,
	ETCB_BT1_SYNCIN0,
	ETCB_BT1_SYNCIN1,
	ETCB_BT1_SYNCIN2,
	ETCB_BT2_SYNCIN0,
	ETCB_BT2_SYNCIN1,
	ETCB_BT2_SYNCIN2,
	ETCB_BT3_SYNCIN0,
	ETCB_BT3_SYNCIN1,
	ETCB_BT3_SYNCIN2,
	
	ETCB_ADC0_SYNCIN0	= 13,
	ETCB_ADC0_SYNCIN1,
	ETCB_ADC0_SYNCIN2,
	ETCB_ADC0_SYNCIN3,
	ETCB_ADC0_SYNCIN4,
	ETCB_ADC0_SYNCIN5,
	ETCB_ADC0_SYNCIN6,
	ETCB_ADC0_SYNCIN7,
	ETCB_ADC0_SYNCIN8,
	ETCB_ADC0_SYNCIN9,
	ETCB_ADC0_SYNCIN10,
	ETCB_ADC0_SYNCIN11,
	ETCB_ADC0_SYNCIN12,
	ETCB_ADC0_SYNCIN13,
	ETCB_ADC0_SYNCIN14,
	
	ETCB_ADC1_SYNCIN0,
	ETCB_ADC1_SYNCIN1,
	ETCB_ADC1_SYNCIN2,
	ETCB_ADC1_SYNCIN3,
	ETCB_ADC1_SYNCIN4,
	ETCB_ADC1_SYNCIN5,
	ETCB_ADC1_SYNCIN6,
	ETCB_ADC1_SYNCIN7,
	ETCB_ADC1_SYNCIN8,
	ETCB_ADC1_SYNCIN9,
	ETCB_ADC1_SYNCIN10,
	ETCB_ADC1_SYNCIN11,
	ETCB_ADC1_SYNCIN12,
	ETCB_ADC1_SYNCIN13,
	ETCB_ADC1_SYNCIN14,	
	
	ETCB_DAC0_SYNCIN0     = 43,
	ETCB_DAC0_SYNCIN1,
	ETCB_DAC0_SYNCIN2,
	ETCB_CMP0_SYNCIN,
	ETCB_CMP1_SYNCIN,
	ETCB_CMP2_SYNCIN,
	
	//GPTA
	ETCB_GPTA0_SYNCIN0 	= 49,
	ETCB_GPTA0_SYNCIN1,
	ETCB_GPTA0_SYNCIN2,
	ETCB_GPTA0_SYNCIN3,
	ETCB_GPTA0_SYNCIN4,
	ETCB_GPTA0_SYNCIN5,
	ETCB_GPTA0_SYNCIN6,
	
	ETCB_GPTA1_SYNCIN0,
	ETCB_GPTA1_SYNCIN1,
	ETCB_GPTA1_SYNCIN2,
	ETCB_GPTA1_SYNCIN3,
	ETCB_GPTA1_SYNCIN4,
	ETCB_GPTA1_SYNCIN5,
	ETCB_GPTA1_SYNCIN6,
	
	ETCB_GPTA2_SYNCIN0,
	ETCB_GPTA2_SYNCIN1,
	ETCB_GPTA2_SYNCIN2,
	ETCB_GPTA2_SYNCIN3,
	ETCB_GPTA2_SYNCIN4,
	ETCB_GPTA2_SYNCIN5,
	ETCB_GPTA2_SYNCIN6,
	
	ETCB_GPTA3_SYNCIN0,
	ETCB_GPTA3_SYNCIN1,
	ETCB_GPTA3_SYNCIN2,
	ETCB_GPTA3_SYNCIN3,
	ETCB_GPTA3_SYNCIN4,
	ETCB_GPTA3_SYNCIN5,
	ETCB_GPTA3_SYNCIN6,
	//GPTB
	ETCB_GPTB0_SYNCIN0  = 77,
	ETCB_GPTB0_SYNCIN1,
	ETCB_GPTB0_SYNCIN2,
	ETCB_GPTB0_SYNCIN3,
	ETCB_GPTB0_SYNCIN4,
	ETCB_GPTB0_SYNCIN5,
	ETCB_GPTB0_SYNCIN6,
	ETCB_GPTB1_SYNCIN0,
	ETCB_GPTB1_SYNCIN1,
	ETCB_GPTB1_SYNCIN2,
	ETCB_GPTB1_SYNCIN3,
	ETCB_GPTB1_SYNCIN4,
	ETCB_GPTB1_SYNCIN5,
	ETCB_GPTB1_SYNCIN6,
	ETCB_GPTB2_SYNCIN0,
	ETCB_GPTB2_SYNCIN1,
	ETCB_GPTB2_SYNCIN2,
	ETCB_GPTB2_SYNCIN3,
	ETCB_GPTB2_SYNCIN4,
	ETCB_GPTB2_SYNCIN5,
	ETCB_GPTB2_SYNCIN6,
	ETCB_GPTB3_SYNCIN0,
	ETCB_GPTB3_SYNCIN1,
	ETCB_GPTB3_SYNCIN2,
	ETCB_GPTB3_SYNCIN3,
	ETCB_GPTB3_SYNCIN4,
	ETCB_GPTB3_SYNCIN5,
	ETCB_GPTB3_SYNCIN6,
	ETCB_GPTB4_SYNCIN0,
	ETCB_GPTB4_SYNCIN1,
	ETCB_GPTB4_SYNCIN2,
	ETCB_GPTB4_SYNCIN3,
	ETCB_GPTB4_SYNCIN4,
	ETCB_GPTB4_SYNCIN5,
	ETCB_GPTB4_SYNCIN6,
	ETCB_GPTB5_SYNCIN0,
	ETCB_GPTB5_SYNCIN1,
	ETCB_GPTB5_SYNCIN2,
	ETCB_GPTB5_SYNCIN3,
	ETCB_GPTB5_SYNCIN4,
	ETCB_GPTB5_SYNCIN5,
	ETCB_GPTB5_SYNCIN6,
	
	ETCB_DMA0_CH0 		= 116,
	ETCB_DMA0_CH1,
	ETCB_DMA0_CH2,
	ETCB_DMA0_CH3,
	ETCB_DMA0_CH4,
	ETCB_DMA0_CH5,
	ETCB_DMA1_CH0 		= 122,
	ETCB_DMA1_CH1,
	ETCB_DMA1_CH2,
	ETCB_DMA1_CH3,
	ETCB_DMA1_CH4,
	ETCB_DMA1_CH5,
	
	ETCB_DST_NOT_USE    = 0xff
}csi_etcb_dst_e;

typedef struct {
    csi_etcb_src_e		 eSrcIp;		//a specific number represent a location in an source trigger location map to trigger other ip(s).
	csi_etcb_src_e       eSrcIp1; 
	csi_etcb_src_e       eSrcIp2; 
    csi_etcb_dst_e       eDstIp;     	//a specific number represent an location in an dest trigger map to wait signal(s) from source ip(s) or location(s).
	csi_etcb_dst_e       eDstIp1;
	csi_etcb_dst_e       eDstIp2;  
    csi_etcb_trg_mode_e  eTrgMode;	 	//the input source is hardware trigger or software trigger.
    csi_etcb_ch_type_e	 eChType;    	//channel type
} csi_etcb_config_t;

/**
  \brief       alloc an  etcb channel
  \param[in]   eChType		etcb channel work mode, \ref csi_etcb_ch_type_e
  \return      channel id or CSI_ERROR
*/
int32_t csi_etcb_ch_alloc(csi_etcb_ch_type_e eChType);

/**
  \brief       free an etcb channel
  \param[in]   eEtbCh		etcb channel number, \ref csi_etcb_ch_e 
  \return      none
*/
void csi_etcb_ch_free(csi_etcb_ch_e eEtbCh);

/**
  \brief       config etcb channel
  \param[in]   eEtbCh		etcb channel number, \ref csi_etcb_ch_e 
  \param[in]   ptConfig     the config structure for etcb channel
  \return      csi error code
*/
/** 
  \brief 	  config etcb channel
  \param[in]  eEtbCh			channel number, \ref csi_etcb_ch_e
  \param[in]  ptConfig		the config structure pointer for etcb channel
  				- eSrcIp: trigger source0, \ref csi_etcb_src_e
  				- eSrcIp1: trigger source1, \ref csi_etcb_src_e
  				- eSrcIp2: trigger source2, \ref csi_etcb_src_e
  				- eDstIp: trigger destination0, \ref csi_etcb_dst_e
  				- eDstIp1: trigger destination1, \ref csi_etcb_dst_e
  				- eDstIp2: trigger destination2, \ref csi_etcb_dst_e
  				- eTrgMode: trigger mode, \ref csi_etcb_trg_mode_e
  				- byChType: trigger source0, \ref csi_etcb_ch_type_e
 *  \return csi error code
*/
csi_error_t csi_etcb_ch_init(csi_etcb_ch_e eEtbCh, csi_etcb_config_t *ptConfig);

/** 
  \brief 	   etcb channel sw force triger
  \param[in]   eEtbCh		etcb channel number, \ref csi_etcb_ch_e 
  \return 	   none
*/
void csi_etcb_ch_sw_trg(csi_etcb_ch_e eEtbCh);

/**
  \brief       open(start) an etcb channel
  \param[in]   eEtbCh      	etcb channel number, \ref csi_etcb_ch_e 
  \return      none
*/
void csi_etcb_ch_start(csi_etcb_ch_e eEtbCh);

/**
  \brief       close(stop) an etcb channel
  \param[in]   etcb        etcb channel number, \ref csi_etcb_ch_e 
  \return      none
*/
void csi_etcb_ch_stop(csi_etcb_ch_e eEtbCh);

#endif /* _CSI_ETCB_H_ */
