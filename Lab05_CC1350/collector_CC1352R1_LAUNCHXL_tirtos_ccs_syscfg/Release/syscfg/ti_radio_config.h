/*
 *  ======== ti_radio_config.h ========
 *  Configured RadioConfig module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC1352R1F3RGZ
 *  by the SysConfig tool.
 *  
 *  Radio Config module version : 1.3
 *  SmartRF Studio data version : 2.15.0
 */
#ifndef _TI_RADIO_CONFIG_H_
#define _TI_RADIO_CONFIG_H_

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)
#include <ti/drivers/rf/RF.h>


//*********************************************************************************
//  RF Setting:   50 kbps, 2-GFSK, 25 kHz deviation, IEEE 802.15.4g MR-FSK PHY mode
//
//  PHY:          2gfsk50kbps154g     
//  Setting file: setting_tc106_154g.json
//*********************************************************************************

// TX Power table size definition
#define TX_POWER_TABLE_SIZE_2gfsk50kbps154g 20

// TX Power Table Object
extern RF_TxPowerTable_Entry txPowerTable_2gfsk50kbps154g[];

// TI-RTOS RF Mode Object
extern RF_Mode RF_prop_2gfsk50kbps154g;

// RF Core API commands
extern const rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup_2gfsk50kbps154g;
extern const rfc_CMD_FS_t RF_cmdFs_2gfsk50kbps154g;
extern const rfc_CMD_PROP_TX_ADV_t RF_cmdPropTxAdv_2gfsk50kbps154g;
extern const rfc_CMD_PROP_RX_ADV_t RF_cmdPropRxAdv_2gfsk50kbps154g;
extern const rfc_CMD_PROP_CS_t RF_cmdPropCs_2gfsk50kbps154g;

// RF Core API Overrides
extern uint32_t pOverrides_2gfsk50kbps154g[];

#endif // _TI_RADIO_CONFIG_H_
