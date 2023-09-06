/***********************************************************************//** 
 * \file  gpio.c
 * \brief  csi gpio driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2020-1- 10<td>V0.1  <td>WNN   <td> tiny change
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_


#include "csp.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_gpio_speed_e
 * \brief    GPIO speed define
 */
typedef enum {
    GPIO_SPEED_NORMAL 	= 0U,		//normal
    GPIO_SPEED_FAST,				//fast
} csi_gpio_speed_e;

/**
 * \enum     csi_gpio_drive_e
 * \brief    GPIO speed define
 */
typedef enum {
    GPIO_DRIVE_NORMAL 	= 0U,		//normal
    GPIO_DRIVE_STRONG,				//strong
} csi_gpio_drive_e;

/**
 * \enum     csi_gpio_dir_e
 * \brief    GPIO dir define
 */
typedef enum {
	GPIO_DIR_GPD		= 0,
    GPIO_DIR_INPUT,    			//GPIO as input
    GPIO_DIR_OUTPUT             //GPIO as output
} csi_gpio_dir_e;

/**
 * \enum     csi_gpio_pin_state_e
 * \brief    GPIO pin state define
 */
typedef enum {
    GPIO_PIN_LOW 		= 0,	//GPIO low level
    GPIO_PIN_HIGH,              //GPIO high level
} csi_gpio_pin_state_e;

/**
 * \enum     csi_gpio_pull_mode_e
 * \brief    GPIO pull mode define
 */
typedef enum {
    GPIO_PULLNONE		= 0,    //Pull none 
    GPIO_PULLUP,                //Pull up 
    GPIO_PULLDOWN,              //Pull down 
} csi_gpio_pull_mode_e;	

/**
 * \enum     csi_gpio_out_mode_e
 * \brief    GPIO output mode define
 */
typedef enum {
	GPIO_PUSH_PULL 		= 0,	//push-pull 
	GPIO_OPEN_DRAIN,			//open drain 
} csi_gpio_output_mode_e;

/**
 * \enum     csi_gpio_input_mode_e
 * \brief    GPIO input mode define
 */
typedef enum {
	GPIO_INPUT_CMOS 	= 0U,	//cmos
	GPIO_INPUT_TTL1,			//ttl1
	GPIO_INPUT_TTL2				//ttl2
}csi_gpio_input_mode_e;

/**
 * \enum     csi_gpio_irq_mode_e
 * \brief    GPIO irq triger type
 */
typedef enum {
    GPIO_IRQ_RISING_EDGE  = 0, //rising edge
    GPIO_IRQ_FALLING_EDGE,     //falling edge
    GPIO_IRQ_BOTH_EDGE,        //both edge
} csi_gpio_irq_mode_e;

/**
 * \enum     csi_gpio_irq_mode_e
 * \brief    GPIO irq triger type
 */
typedef enum {
	
    IOMAP0_I2C_SCL	=0U,	//IOMAP GROUP0
	IOMAP0_I2C_SDA,
	IOMAP0_USART0_TX,
	IOMAP0_USART0_RX,
	IOMAP0_SPI0_NSS,
	IOMAP0_SPI0_SCK,
	IOMAP0_SPI0_MISO,
	IOMAP0_SPI0_MOSI,
	
	IOMAP1_USART0_TX,		//IOMAP GROUP1
	IOMAP1_USART0_RX,
	IOMAP1_USART0_SCK,
	IOMAP1_UART2_TX,
	IOMAP1_UART2_RX,
	IOMAP1_GPTA0_CHA,
	IOMAP1_GPTA1_CHA,
	IOMAP1_CMP0_OUT,
} csi_gpio_iomap_e;

/**
 * \enum     csi_exi_grp_e
 * \brief    GPIO exi group
 */
typedef enum{
	EXI_GRP0 = 0,
	EXI_GRP1,
	EXI_GRP2,
	EXI_GRP3,
	EXI_GRP4,
	EXI_GRP5,
	EXI_GRP6,
	EXI_GRP7,
	EXI_GRP8,
	EXI_GRP9,
	EXI_GRP10,
	EXI_GRP11,
	EXI_GRP12,
	EXI_GRP13,
	EXI_GRP14,
	EXI_GRP15,
	EXI_GRP16,
	EXI_GRP17,
	EXI_GRP18,
	EXI_GRP19,
}csi_exi_grp_e;

/**
 * \enum     csi_exi_trgsrc_e
 * \brief    GPIO exi trigger source
 */
typedef enum {
	TRGSRC_EXI0 = 0,
	TRGSRC_EXI1,
	TRGSRC_EXI2,
	TRGSRC_EXI3,
	TRGSRC_EXI4,
	TRGSRC_EXI5,
	TRGSRC_EXI6,
	TRGSRC_EXI7,
	TRGSRC_EXI8,
	TRGSRC_EXI9,
	TRGSRC_EXI10,
	TRGSRC_EXI11,
	TRGSRC_EXI12,
	TRGSRC_EXI13,
	TRGSRC_EXI14,
	TRGSRC_EXI15,
	TRGSRC_EXI16,
	TRGSRC_EXI17,
	TRGSRC_EXI18,
	TRGSRC_EXI19, 
}csi_exi_trgsrc_e;

/**
 * \enum     csi_exi_trgout_e
 * \brief    GPIO exi trigger out
 */
typedef enum {
	EXI_TRGOUT0 = 0,
	EXI_TRGOUT1,
	EXI_TRGOUT2,
	EXI_TRGOUT3,
	EXI_TRGOUT4,
	EXI_TRGOUT5,
}csi_exi_trgout_e;

/**
 * \enum     csi_exi_status_e
 * \brief    GPIO exi status bit
 */
typedef enum {
	STATUS_EXI0 	= (0x01ul << 0),
    STATUS_EXI1		= (0x01ul << 1),      
	STATUS_EXI2 	= (0x01ul << 2),
    STATUS_EXI3		= (0x01ul << 3), 
	STATUS_EXI4 	= (0x01ul << 4),
    STATUS_EXI5		= (0x01ul << 5),      
	STATUS_EXI6 	= (0x01ul << 6),
    STATUS_EXI7		= (0x01ul << 7), 
	STATUS_EXI8 	= (0x01ul << 8),
    STATUS_EXI9		= (0x01ul << 9),      
	STATUS_EXI10 	= (0x01ul << 10),
    STATUS_EXI11	= (0x01ul << 11), 
	STATUS_EXI12 	= (0x01ul << 12),
    STATUS_EXI13	= (0x01ul << 13),      
	STATUS_EXI14 	= (0x01ul << 14),
    STATUS_EXI15	= (0x01ul << 15),
	STATUS_EXI16	= (0x01ul << 16),      
	STATUS_EXI17 	= (0x01ul << 17),
    STATUS_EXI18	= (0x01ul << 18),
	STATUS_EXI19	= (0x01ul << 19)
}csi_exi_status_e;


/**
 * \enum     csi_exi_flttm_e
 * \brief    GPIO exi flt time
 */
typedef enum{
	EXI_FLT_NONE = 0,
	EXI_FLT_3TM,
	EXI_FLT_6TM,
	EXI_FLT_9TM,
	EXI_FLT_12TM,
	EXI_FLT_18TM,
	EXI_FLT_24TM,
	EXI_FLT_36TM,
	EXI_FLT_48TM,
	EXI_FLT_60TM,
	EXI_FLT_72TM,
	EXI_FLT_84TM,
	EXI_FLT_96TM,
	EXI_FLT_120TM,
	EXI_FLT_168TM,
	EXI_FLT_216TM
}csi_exi_flttm_e;

/**
  \brief       set gpio mux function
  \param[in]   ptGpioBase  pointer of gpio register structure  
  \param[in]   ePinName    pin name, defined in soc.h
  \param[in]   ePinFunc    pin function, defined in soc.h
  \return      none
*/
void csi_gpio_set_mux(csp_gpio_t *ptGpioBase, pin_name_e ePinName, pin_func_e ePinFunc);

/** 
  \brief 	   set gpio iomap function
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name
  \param[in]   eIoMap			gpio pin remap function
  \return 	   error code \ref csi_error_t
 */  
csi_error_t csi_gpio_set_iomap(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_iomap_e eIoMap);

/**
  \brief       get gpio function
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName    pin name, defined in soc.h
  \return      pin function
*/
pin_func_e csi_gpio_get_mux(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/**
  \brief       set gpio pull mode
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName    pin name, defined in soc.h
  \param[in]   ePullMode   pull mode, pull none/pull up/pull down
  \return      \ref  csi_error_t
*/
csi_error_t csi_gpio_pull_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_pull_mode_e ePullMode);

/** 
  \brief 	   set gpio input mode
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name, defined in soc.h.
  \param[in]   eInputMode	INPUT_CMOS/INPUT_TTL
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_input_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_input_mode_e eInputMode);

/** 
  \brief       set gpio output mode
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name, defined in soc.h.
  \param[in]   eOutMode		push-pull/open drain
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_output_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_output_mode_e eOutMode);

/**
  \brief       set gpio speed
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName     pin name, defined in soc.h
  \param[in]   eSpeed       io speed
  \return      none
*/
void csi_gpio_speed(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_speed_e eSpeed);

/**
  \brief       set gpio drive
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName     pin name, defined in soc.h
  \param[in]   eDrive       io drive
  \return      \ref  csi_error_t
*/
void csi_gpio_drive(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_drive_e eDrive);

/** 
  \brief 	   enable gpio input filtering
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name, defined in soc.h.
  \param[in]   bEnable		ENABLE/DISABLE
  \return 	   none
 */ 
void csi_pin_gpio_filter(csp_gpio_t *ptGpioBase, pin_name_e ePinName, bool bEnable);

/** 
  \brief       get gpio num
  \param[in]   ePinName	 	gpio pin name, defined in soc.h.
  \return      pin num
 */
uint8_t csi_gpio_get_num(pin_name_e ePinName);

/** 
  \brief 	   get the value of selected gpio 
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name, defined in soc.h.
  \return 	   According to the bit mask, the corresponding pin status is obtained
*/
bool csi_gpio_read(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief  	   gpio set high(output = 1)
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name
  \return      none
 */
void csi_gpio_set_high(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief   	   gpio set low(output = 0)
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		gpio pin name
  \return      none
 */
void csi_gpio_set_low(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief  	   gpio toggle
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   pin_name		gpio pin name
  \return      none
 */
void csi_gpio_toggle(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief  	   write gpio port ouput value
  \param[in]   ptGpioBase	pointer of gpio register structure
  \param[in]   wPinMask		pin mask, mask of write pin
  \param[in]   hwVal		Value of write
  \return none
 */ 
void  csi_gpio_write_port(csp_gpio_t *ptGpioBase, uint16_t hwPinMask, uint16_t hwVal);

/** 
  \brief       config gpio irq mode(assign exi group)
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		pin name
  \param[in]   eExiGrp		exi group; EXI_GRP0 ~EXI_GRP19
  \param[in]   eTrgEdge		rising edge; falling edge;	both edge;
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_irq_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_exi_grp_e eExiGrp, csi_gpio_irq_mode_e eTrgEdge);

/** 
  \brief       gpio interrupt enable
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		pin mask,0x0001~0xffff
  \param[in]   bEnable		true or false
  \return      error code \ref csi_error_t
 */ 
void csi_gpio_irq_enable(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief       gpio interrupt disable
  \param[in]   ptGpioBase 	pointer of gpio register structure 
  \param[in]   ePinName		pin mask,0x0001~0xffff
  \param[in]   bEnable		true or false
  \return      error code \ref csi_error_t
 */ 
void csi_gpio_irq_disable(csp_gpio_t *ptGpioBase, pin_name_e ePinName);

/** 
  \brief 	   gpio vic irq enable
  \param[in]   eExiGrp: exi group(exi line); EXI_GRP0 ~EXI_GRP19
  \param[in]   bEnable: ENABLE OR DISABLE
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_vic_irq_enable(csi_exi_grp_e eExiGrp, bool bEnable);

/** \brief  set exi as trigger Event(EV0~5) 
  \param[in]   eTrgOut		output Event select(TRGOUT0~5)
  \param[in]   eExiTrgSrc 	event source (TRGSRC_EXI0~19)
  \param       byTrgPrd 	accumulated EXI events to output trigger 
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_exi_set_evtrg(csi_exi_trgout_e eTrgOut, csi_exi_trgsrc_e eExiTrgSrc, uint8_t byTrgPrd);

/** 
  \brief exi evtrg output enable
  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
  \return none
 */
void csi_exi_evtrg_enable(csi_exi_trgout_e eTrgOut);

/** 
  \brief exi evtrg output disable
  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
  \return none
 */
void csi_exi_evtrg_disable(csi_exi_trgout_e eTrgOut);

/** 
  \brief  exi software trigger event 
  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
  \return none
 */ 
void csi_exi_soft_evtrg(csi_exi_trgout_e eTrgOut);


#ifdef __cplusplus

}
#endif

#endif /* _DRV_GPIO_H_ */
