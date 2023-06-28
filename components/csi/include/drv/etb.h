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
#ifndef _DRV_ETB_H_
#define _DRV_ETB_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif
#define ETB_CH_ALLOC_LEN		32
/**
 * \enum     csi_etb_trig_mode_e
 * \brief    ETB channel trigger mode
 */
typedef enum {
    ETB_HARDWARE_TRG  	= 0,	//etb channel inout is hardware trigger.
    ETB_SOFTWARE_TRG            //etb channel inout is software trigger.
} csi_etb_trg_mode_e;

/**
 * \enum     csi_etb_ch_type_e
 * \brief    ETB channel trigger type
 */
typedef enum {
    ETB_ONE_TRG_ONE	= 0,      //one device trig one deivce, 3~31channel
    ETB_ONE_TRG_MORE,         //one device trig two for more device, 0~2channel
	ETB_ONE_TRG_ONE_DMA       //one device trig one deivce of dma, 20->31channel
} csi_etb_ch_type_e;

/**
 * \enum     csi_etb_ch_e
 * \brief    ETB channel number(number 0~31)
 */
typedef enum {
    ETB_CH0			= 0,		//etb channel 0 id number
	ETB_CH1,					//etb channel 1 id number
	ETB_CH2,					//etb channel 2 id number
	ETB_CH3,					//etb channel 3 id number
	ETB_CH4,
	ETB_CH5,
	ETB_CH6,
	ETB_CH7,
	ETB_CH8,			
	ETB_CH9,						
	ETB_CH10,						
	ETB_CH11,						
	ETB_CH12,
	ETB_CH13,
	ETB_CH14,
	ETB_CH15,
	ETB_CH16,
	ETB_CH17,
	ETB_CH18,						
	ETB_CH19,						
	ETB_CH20,					//etb channel 20, dma start channel						
	ETB_CH21,
	ETB_CH22,
	ETB_CH23,
	ETB_CH24,
	ETB_CH25,			
	ETB_CH26,						
	ETB_CH27,						
	ETB_CH28,						
	ETB_CH29,
	ETB_CH30,
	ETB_CH31
} csi_etb_ch_e;

/**
 * \enum     csi_etb_src_e
 * \brief    ETB event source
 */
typedef enum{
	ETB_LPT_TRGOUT0 	= 0,
	ETB_EXI_TRGOUT0 	= 4,
	ETB_EXI_TRGOUT1,
	ETB_EXI_TRGOUT2,	
	ETB_EXI_TRGOUT3,

	ETB_RTC_TRGOUT0		= 8	,
	ETB_RTC_TRGOUT1,
	ETB_BT0_TRGOUT,
	ETB_BT1_TRGOUT,
	ETB_BT2_TRGOUT,
	ETB_BT3_TRGOUT,

	ETB_EXI_TRGOUT4     = 19,	
	ETB_EXI_TRGOUT5,	
	
	ETB_ADC0_TRGOUT0	= 30,
	ETB_ADC0_TRGOUT1,
	ETB_ADC1_TRGOUT0,
	ETB_ADC1_TRGOUT1,
	
	ETB_CMP0_TRGOUT		= 38,
	ETB_CMP1_TRGOUT,
	ETB_CMP2_TRGOUT,
	
	ETB_GPTA0_TRGOUT0 	= 47,
	ETB_GPTA0_TRGOUT1,
	ETB_GPTA1_TRGOUT0,
	ETB_GPTA1_TRGOUT1,
	ETB_GPTA2_TRGOUT0,
	ETB_GPTA2_TRGOUT1,
	ETB_GPTA3_TRGOUT0,
	ETB_GPTA3_TRGOUT1,
	
	ETB_GPTB0_TRGOUT0 	= 63,
	ETB_GPTB0_TRGOUT1,
	ETB_GPTB1_TRGOUT0,
	ETB_GPTB1_TRGOUT1,
	ETB_GPTB2_TRGOUT0,
	ETB_GPTB2_TRGOUT1,
	ETB_GPTB3_TRGOUT0,
	ETB_GPTB3_TRGOUT1,
	ETB_GPTB4_TRGOUT0,
	ETB_GPTB4_TRGOUT1,
	ETB_GPTB5_TRGOUT0,
	ETB_GPTB5_TRGOUT1,
	ETB_I2C0_TXSRC,
	ETB_I2C0_RXSRC,
	
	ETB_SPI0_TXSRC 		= 81,
	ETB_SPI0_RXSRC,
	ETB_SPI1_TXSRC,
	ETB_SPI1_RXSRC,
	
	ETB_SIO0_TXSRC 		= 87,
	ETB_SIO0_RXSRC,
	ETB_SIO1_TXSRC,
	ETB_SIO1_RXSRC,
	
	ETB_USART0_TXSRC 	= 93,
	ETB_USART0_RXSRC,
	ETB_USART1_TXSRC,
	ETB_USART1_RXSRC,
	ETB_UART0_TXSRC		= 97,
	ETB_UART0_RXSRC,
	ETB_UART1_TXSRC,
	ETB_UART1_RXSRC,
	ETB_UART2_TXSRC,
	ETB_UART2_RXSRC,
	
	ETB_CAN_TXSRC       = 107,
	ETB_CAN_RXSRC,
	
	ETB_DAC0_TRGOUT 	= 111,
	
	ETB_SRC_NOT_USE     = 0xff
}csi_etb_src_e;

/**
 * \enum     csi_etb_dst_e
 * \brief    ETB description event 
 */
typedef enum{
	ETB_LPT_SYNCIN 		= 0,
	ETB_BT0_SYNCIN0,
	ETB_BT0_SYNCIN1,
	ETB_BT0_SYNCIN2,
	ETB_BT1_SYNCIN0,
	ETB_BT1_SYNCIN1,
	ETB_BT1_SYNCIN2,
	ETB_BT2_SYNCIN0,
	ETB_BT2_SYNCIN1,
	ETB_BT2_SYNCIN2,
	ETB_BT3_SYNCIN0,
	ETB_BT3_SYNCIN1,
	ETB_BT3_SYNCIN2,
	
	ETB_ADC0_SYNCIN0	= 13,
	ETB_ADC0_SYNCIN1,
	ETB_ADC0_SYNCIN2,
	ETB_ADC0_SYNCIN3,
	ETB_ADC0_SYNCIN4,
	ETB_ADC0_SYNCIN5,
	ETB_ADC0_SYNCIN6,
	ETB_ADC0_SYNCIN7,
	ETB_ADC0_SYNCIN8,
	ETB_ADC0_SYNCIN9,
	ETB_ADC0_SYNCIN10,
	ETB_ADC0_SYNCIN11,
	ETB_ADC0_SYNCIN12,
	ETB_ADC0_SYNCIN13,
	ETB_ADC0_SYNCIN14,
	
	ETB_ADC1_SYNCIN0,
	ETB_ADC1_SYNCIN1,
	ETB_ADC1_SYNCIN2,
	ETB_ADC1_SYNCIN3,
	ETB_ADC1_SYNCIN4,
	ETB_ADC1_SYNCIN5,
	ETB_ADC1_SYNCIN6,
	ETB_ADC1_SYNCIN7,
	ETB_ADC1_SYNCIN8,
	ETB_ADC1_SYNCIN9,
	ETB_ADC1_SYNCIN10,
	ETB_ADC1_SYNCIN11,
	ETB_ADC1_SYNCIN12,
	ETB_ADC1_SYNCIN13,
	ETB_ADC1_SYNCIN14,	
	
	ETB_DAC0_SYNCIN0     = 43,
	ETB_DAC0_SYNCIN1,
	ETB_DAC0_SYNCIN2,
	ETB_CMP0_SYNCIN,
	ETB_CMP1_SYNCIN,
	ETB_CMP2_SYNCIN,
	
	//GPTA
	ETB_GPTA0_SYNCIN0 	= 49,
	ETB_GPTA0_SYNCIN1,
	ETB_GPTA0_SYNCIN2,
	ETB_GPTA0_SYNCIN3,
	ETB_GPTA0_SYNCIN4,
	ETB_GPTA0_SYNCIN5,
	ETB_GPTA0_SYNCIN6,
	
	ETB_GPTA1_SYNCIN0,
	ETB_GPTA1_SYNCIN1,
	ETB_GPTA1_SYNCIN2,
	ETB_GPTA1_SYNCIN3,
	ETB_GPTA1_SYNCIN4,
	ETB_GPTA1_SYNCIN5,
	ETB_GPTA1_SYNCIN6,
	
	ETB_GPTA2_SYNCIN0,
	ETB_GPTA2_SYNCIN1,
	ETB_GPTA2_SYNCIN2,
	ETB_GPTA2_SYNCIN3,
	ETB_GPTA2_SYNCIN4,
	ETB_GPTA2_SYNCIN5,
	ETB_GPTA2_SYNCIN6,
	
	ETB_GPTA3_SYNCIN0,
	ETB_GPTA3_SYNCIN1,
	ETB_GPTA3_SYNCIN2,
	ETB_GPTA3_SYNCIN3,
	ETB_GPTA3_SYNCIN4,
	ETB_GPTA3_SYNCIN5,
	ETB_GPTA3_SYNCIN6,
	//GPTB
	ETB_GPTB0_SYNCIN0  = 77,
	ETB_GPTB0_SYNCIN1,
	ETB_GPTB0_SYNCIN2,
	ETB_GPTB0_SYNCIN3,
	ETB_GPTB0_SYNCIN4,
	ETB_GPTB0_SYNCIN5,
	ETB_GPTB0_SYNCIN6,
	ETB_GPTB1_SYNCIN0,
	ETB_GPTB1_SYNCIN1,
	ETB_GPTB1_SYNCIN2,
	ETB_GPTB1_SYNCIN3,
	ETB_GPTB1_SYNCIN4,
	ETB_GPTB1_SYNCIN5,
	ETB_GPTB1_SYNCIN6,
	ETB_GPTB2_SYNCIN0,
	ETB_GPTB2_SYNCIN1,
	ETB_GPTB2_SYNCIN2,
	ETB_GPTB2_SYNCIN3,
	ETB_GPTB2_SYNCIN4,
	ETB_GPTB2_SYNCIN5,
	ETB_GPTB2_SYNCIN6,
	ETB_GPTB3_SYNCIN0,
	ETB_GPTB3_SYNCIN1,
	ETB_GPTB3_SYNCIN2,
	ETB_GPTB3_SYNCIN3,
	ETB_GPTB3_SYNCIN4,
	ETB_GPTB3_SYNCIN5,
	ETB_GPTB3_SYNCIN6,
	ETB_GPTB4_SYNCIN0,
	ETB_GPTB4_SYNCIN1,
	ETB_GPTB4_SYNCIN2,
	ETB_GPTB4_SYNCIN3,
	ETB_GPTB4_SYNCIN4,
	ETB_GPTB4_SYNCIN5,
	ETB_GPTB4_SYNCIN6,
	ETB_GPTB5_SYNCIN0,
	ETB_GPTB5_SYNCIN1,
	ETB_GPTB5_SYNCIN2,
	ETB_GPTB5_SYNCIN3,
	ETB_GPTB5_SYNCIN4,
	ETB_GPTB5_SYNCIN5,
	ETB_GPTB5_SYNCIN6,
	
	ETB_DMA0_CH0 		= 116,
	ETB_DMA0_CH1,
	ETB_DMA0_CH2,
	ETB_DMA0_CH3,
	ETB_DMA0_CH4,
	ETB_DMA0_CH5,
	ETB_DMA1_CH0 		= 122,
	ETB_DMA1_CH1,
	ETB_DMA1_CH2,
	ETB_DMA1_CH3,
	ETB_DMA1_CH4,
	ETB_DMA1_CH5,
	
	ETB_DST_NOT_USE    = 0xff
}csi_etb_dst_e;

typedef struct {
    uint8_t			bySrcIp;			//a specific number represent a location in an source trigger location map to trigger other ip(s).
	uint8_t         bySrcIp1; 
	uint8_t         bySrcIp2; 
    uint8_t         byDstIp;     		//a specific number represent an location in an dest trigger map to wait signal(s) from source ip(s) or location(s).
	uint8_t         byDstIp1;
	uint8_t         byDstIp2;  
    uint8_t    		byTrgMode;			//the input source is hardware trigger or software trigger.
    uint8_t     	byChType;    		//channel type
} csi_etb_config_t;

/**
  \brief       Init the etb device
  \return      none
*/
void csi_etb_init(void);

/**
  \brief       alloc an  etb channel
  \param[in]   eChType		etb channel work mode
  \return      channel id or CSI_ERROR
*/
int32_t csi_etb_ch_alloc(csi_etb_ch_type_e eChType);

/**
  \brief       free an etb channel
  \param[in]   eEtbCh		etb channel number 
  \return      none
*/
void csi_etb_ch_free(csi_etb_ch_e eEtbCh);

/**
  \brief       config etb channel
  \param[in]   eEtbCh		etb channel number
  \param[in]   ptConfig     the config structure for etb channel
  \return      csi error code
*/
csi_error_t csi_etb_ch_config(csi_etb_ch_e eEtbCh, csi_etb_config_t *ptConfig);

/** 
  \brief 	   etb channel sw force triger
  \param[in]   eEtbCh		etb channel number
  \return 	   none
*/
void csi_etb_ch_swtrg(csi_etb_ch_e eEtbCh);

/**
  \brief       open(start) an etb channel
  \param[in]   eEtbCh      	etb channel number
  \return      none
*/
void csi_etb_ch_start(csi_etb_ch_e eEtbCh);

/**
  \brief       close(stop) an etb channel
  \param[in]   etb        etb channel number
  \return      none
*/
void csi_etb_ch_stop(csi_etb_ch_e eEtbCh);

#endif /* _CSI_ETB_H_ */
