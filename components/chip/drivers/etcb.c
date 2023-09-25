
/***********************************************************************//** 
 * \file  etcb.c
 * \brief  ETCB(event trigger control block) driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-10-8 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>SW trigger
 * <tr><td> 2023-9-21 <td>V0.3  <td>ZJY   <td>code normalization 
 * </table>
 * *********************************************************************
*/
#include <drv/etcb.h>

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/

#define ETCB_BUF_LEN	((ETCB_CH_MAX_NUM-1)/ETCB_CH_ALLOC_LEN +1)
static uint32_t s_wEtbAllocStatus[ETCB_BUF_LEN] = {0};


/** \brief etcb channel[0->31] check
 * 
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \return none
 */ 
static int32_t check_is_alloced(csi_etcb_ch_e eEtbCh)
{
    uint8_t byChOffset, byChGroup;
	uint32_t wStatus = 0U;
    int32_t ret = 0;
    byChGroup =   (uint8_t)(eEtbCh / ETCB_CH_ALLOC_LEN);
    byChOffset =  (uint8_t)(eEtbCh % ETCB_CH_ALLOC_LEN);

    wStatus = s_wEtbAllocStatus[byChGroup];
    wStatus &= (uint32_t)(1U << byChOffset);

    if (wStatus != 0U)
        ret = -1;

    return ret;
}
/** \brief etcb channel[0:31] status 
 * 
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \param[in] status: status
 *  \return none
 */ 
static void set_ch_alloc_status(csi_etcb_ch_e eEtbCh, uint32_t wStatus)
{
    uint8_t byChOffset, byChGroup;

    byChGroup =  (uint8_t)(eEtbCh / ETCB_CH_ALLOC_LEN);
    byChOffset = (uint8_t)(eEtbCh % ETCB_CH_ALLOC_LEN);

    if (wStatus == 1U) 
        s_wEtbAllocStatus[byChGroup] |= (uint32_t)(1U << byChOffset);
	else if (wStatus == 0U) 
        s_wEtbAllocStatus[byChGroup] &= ~(uint32_t)(1U << byChOffset);
		
}
/** \brief etcb channel[0->31] enable/disable 
 * 
 *  \param[in] ptEtbBase: pionter of ETCB reg structure.
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \param[in] bEnable: enable/disable
 *  \return none
 */ 
static void apt_etcb_ch_enable(csp_etcb_t *ptEtbBase, csi_etcb_ch_e eEtbCh, bool bEnable)
{
	switch(eEtbCh)
	{
		case ETCB_CH0:		
			ptEtbBase->CFG1_CH0 =((ptEtbBase->CFG1_CH0 & (~ETCB_CH_EN_MSK)) | (uint8_t)bEnable);
			break;
		case ETCB_CH1:
			ptEtbBase->CH1_2[0].CFG1 =((ptEtbBase->CH1_2[0].CFG1 & (~ETCB_CH_EN_MSK)) | (uint8_t)bEnable);
			break;
		case ETCB_CH2:
			ptEtbBase->CH1_2[1].CFG1 =((ptEtbBase->CH1_2[1].CFG1 & (~ETCB_CH_EN_MSK)) | (uint8_t)bEnable);
			break;
		default:
			if(eEtbCh <= ETCB_CH31)
				ptEtbBase->CFG_CHX[eEtbCh-3] = ((ptEtbBase->CFG_CHX[eEtbCh-3] & (~ETCB_CH_EN_MSK)) | (uint8_t)bEnable);
			break;
	}
}
/** \brief etcb more source trigger one destination
 * 
 *  \param[in] ptEtbBase: pionter of etcb reg structure.
 *  \param[in] bySrc0: trigger source 0
 *  \param[in] bySrc1: trigger source 1
 *  \param[in] bySrc2: trigger source 2
 *  \param[in] byDst: trigger destination
 *  \param[in] eTrgMode: hard/soft trigger
 *  \return none
 */ 
//static void etcb_set_more_trg_one(csp_etcb_t *ptEtbBase, uint8_t bySrc0, uint8_t bySrc1, uint8_t bySrc2,uint8_t byDst, etcb_ch_trg_mode_e eTrgMode)
//{
//	ptEtbBase->CFG0_CH0 = (ETCB_CH0_TRG_SRC0(bySrc0) | ETCB_CH0_TRG_SRC1(bySrc1) | ETCB_CH0_TRG_SRC2(bySrc2)); 
//	if(bySrc0 != ETCB_SRC_NOT_USE)
//		ptEtbBase->CFG0_CH0 |= ETCB_CH0_SRC0_EN;
//	else
//		ptEtbBase->CFG0_CH0 &= ~ETCB_CH0_SRC0_EN_MSK;
//	
//	if(bySrc1 != ETCB_SRC_NOT_USE)
//		ptEtbBase->CFG0_CH0 |= (ETCB_CH0_SRC1_EN << ETCB_CH0_SRC1_EN_POS);
//	else
//		ptEtbBase->CFG0_CH0 &= ~ETCB_CH0_SRC1_EN_MSK;
//		
//	if(bySrc1 != ETCB_SRC_NOT_USE)
//		ptEtbBase->CFG0_CH0 |= (ETCB_CH0_SRC2_EN << ETCB_CH0_SRC2_EN_POS);
//	else
//		ptEtbBase->CFG0_CH0 &= ~ETCB_CH0_SRC2_EN_MSK;
//		
//	ptEtbBase->CFG1_CH0 = (eTrgMode << ETCB_CH_TRG_MODE_POS) | ETCB_CH0_TRG_DST(byDst); 
//	ptEtbBase->CFG1_CH0  |= ETCB_CH_EN;		//enable etcb channel
//}
/** \brief etcb one source trigger more destination
 * 
 *  \param[in] ptEtbBase: pionter of etcb reg structure.
 *  \param[in] byChNum: channel number= [1:2]
 *  \param[in] bySrc: trigger source
 *  \param[in] byDst0: trigger destination 0
 *  \param[in] byDst1: trigger destination 1
 *  \param[in] byDst2: trigger destination 2
 *  \param[in] eTrgMode: trigger mode(hard/soft), \ref csi_etcb_ch_e
 *  \return none
 */ 
static void etcb_set_one_trg_more(csp_etcb_t *ptEtbBase, uint8_t byChNum, uint8_t bySrc, uint8_t byDst0, uint8_t byDst1, uint8_t byDst2, etcb_ch_trg_mode_e eTrgMode)
{
	ptEtbBase->CH1_2[byChNum-1].CFG0 = (ETCB_CH1_2_TRG_DST0(byDst0) | ETCB_CH1_2_TRG_DST1(byDst1) | ETCB_CH1_2_TRG_DST2(byDst2)); 
	if(byDst0 != ETCB_DST_NOT_USE)
		ptEtbBase->CH1_2[byChNum-1].CFG0 |= (ETCB_CH1_2_DST0_EN << ETCB_CH1_2_DST0_EN_POS);
	else
		ptEtbBase->CH1_2[byChNum-1].CFG0 &= ~ETCB_CH1_2_DST0_EN_MSK;
	
	if(byDst1 != ETCB_DST_NOT_USE)
		ptEtbBase->CH1_2[byChNum-1].CFG0 |= (ETCB_CH1_2_DST1_EN << ETCB_CH1_2_DST1_EN_POS);
	else
		ptEtbBase->CH1_2[byChNum-1].CFG0 &= ~ETCB_CH1_2_DST1_EN_MSK;
	
	if(byDst2 != ETCB_DST_NOT_USE)
		ptEtbBase->CH1_2[byChNum-1].CFG0 |= (ETCB_CH1_2_DST2_EN << ETCB_CH1_2_DST2_EN_POS);
	else
		ptEtbBase->CH1_2[byChNum-1].CFG0 &= ~ETCB_CH1_2_DST2_EN_MSK;
		
	ptEtbBase->CH1_2[byChNum-1].CFG1 = (eTrgMode << ETCB_CH_TRG_MODE_POS) | ETCB_CH1_2_TRG_SRC(bySrc);
	ptEtbBase->CH1_2[byChNum-1].CFG1 |= ETCB_CH_EN;	//enable etcb channel
}
/** \brief alloc an etcb channel
 * 
 *  \param[in] eChType: etcb channel woke mode, \ref csi_etcb_ch_type_e
 *  \return channel id or CSI_ERROR
*/
int32_t csi_etcb_ch_alloc(csi_etcb_ch_type_e eChType)
{
    int32_t ret_ch = 0;
    uint32_t result = csi_irq_save();

    switch (eChType) 
	{
        case ETCB_ONE_TRG_MORE:
            for (ret_ch = 0; ret_ch < 3; ret_ch++) 
			{
                if (check_is_alloced(ret_ch) != -1) 
                    break;
            }

            if (ret_ch >= 3) 
                ret_ch = CSI_ERROR;
				
            break;
        case ETCB_ONE_TRG_ONE:
            for (ret_ch = 3; ret_ch < ETCB_CH_DMA_STAR; ret_ch++) 
			{
                if (check_is_alloced(ret_ch) != -1)
                    break;
            }

            if (ret_ch >= ETCB_CH_DMA_STAR) 
                ret_ch = CSI_ERROR;
				
            break;
		case ETCB_ONE_TRG_ONE_DMA:
			for (ret_ch = ETCB_CH_DMA_STAR; ret_ch < ETCB_CH_MAX_NUM; ret_ch++) 
			{
                if (check_is_alloced(ret_ch) != -1)
                    break;
            }

            if (ret_ch >= ETCB_CH_MAX_NUM) 
               ret_ch = CSI_ERROR;
			
			break;
        default:
            ret_ch = CSI_ERROR;
    }

    if (ret_ch != CSI_ERROR) 
        set_ch_alloc_status(ret_ch, 1U);

    csi_irq_restore(result);
    return ret_ch;
}
/** \brief free an etcb channel
 * 
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \return none
*/
void csi_etcb_ch_free(csi_etcb_ch_e eEtbCh)
{
    uint32_t result = csi_irq_save();
    set_ch_alloc_status(eEtbCh, 0U);
    csi_irq_restore(result);
}
/** \brief config etcb channel
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \param[in] ptConfig: the config structure pointer for etcb channel
 * 				- eSrcIp: trigger source0, \ref csi_etcb_src_e
 * 				- eSrcIp1: trigger source1, \ref csi_etcb_src_e
 * 				- eSrcIp2: trigger source2, \ref csi_etcb_src_e
 * 				- eDstIp: trigger destination0, \ref csi_etcb_dst_e
 * 				- eDstIp1: trigger destination1, \ref csi_etcb_dst_e
 * 				- eDstIp2: trigger destination2, \ref csi_etcb_dst_e
 * 				- eTrgMode: trigger mode, \ref csi_etcb_trg_mode_e
 * 				- byChType: trigger source0, \ref csi_etcb_ch_type_e
 * 
 *  \return csi error code
*/
csi_error_t csi_etcb_ch_init(csi_etcb_ch_e eEtbCh, csi_etcb_config_t *ptConfig)
{
	csi_error_t ret = CSI_OK;
	
	soc_clk_enable(ETCB_SYS_CLK);			//enable peripheral clk
    csp_etcb_enable(ETCB);					//enable etcb module
	
	switch(ptConfig->eChType)
	{
		case ETCB_ONE_TRG_ONE:						//channel num = [3:32]
			if(eEtbCh > ETCB_CH2)
			{
				csp_etcb_set_one_trg_one(ETCB, eEtbCh, ptConfig->eSrcIp, ptConfig->eDstIp, (etcb_ch_trg_mode_e)ptConfig->eTrgMode);
				csp_etcb_chx_enable(ETCB, eEtbCh);	//enable etcb channel 
			}
			else
				ret = CSI_ERROR;

			break;
		case ETCB_ONE_TRG_MORE:						//channel num = [0:2]		
			if(eEtbCh < ETCB_CH3)
				etcb_set_one_trg_more(ETCB, eEtbCh, ptConfig->eSrcIp, ptConfig->eDstIp, ptConfig->eDstIp1, ptConfig->eDstIp2, (etcb_ch_trg_mode_e)ptConfig->eTrgMode);
			else
				ret = CSI_ERROR;
				
			break;
		case ETCB_ONE_TRG_ONE_DMA:					//channel num = [20:31]
			if((eEtbCh >= ETCB_CH_DMA_STAR) && (eEtbCh < ETCB_CH_MAX_NUM))
			{
				csp_etcb_set_one_trg_one(ETCB, eEtbCh, ptConfig->eSrcIp, ptConfig->eDstIp, (etcb_ch_trg_mode_e)ptConfig->eTrgMode);
				csp_etcb_dma_enable(ETCB, eEtbCh);	//enable etcb dma
				csp_etcb_chx_enable(ETCB, eEtbCh);	//enable etcb channel 
			}
			else
				ret = CSI_ERROR;
				
			break;
		default:
			ret = CSI_ERROR;
			break;
	}

    return ret;
}
/** \brief etcb channel sw force triger
 * 
 *  \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 *  \return none
*/
void csi_etcb_ch_sw_trg(csi_etcb_ch_e eEtbCh)
{
	csp_etcb_sw_trg(ETCB, eEtbCh);
}
/**
 * \brief start an etcb channel
 * \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 * \return none
*/
void csi_etcb_ch_start(csi_etcb_ch_e eEtbCh)
{
	apt_etcb_ch_enable(ETCB,eEtbCh, ENABLE);
}
/**
 * \brief stop an etcb channel
 * \param[in] eEtbCh: channel number, \ref csi_etcb_ch_e
 * \return none
*/
void csi_etcb_ch_stop(csi_etcb_ch_e eEtbCh)
{
    apt_etcb_ch_enable(ETCB, eEtbCh, DISABLE);
}
