/*
 * nRF24L01.h
 *
 *  Created on: Mar 31, 2019
 *      Author: WINNGUYEN
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

/* Register Map */
#define RF24_CONFIG      0x00
#define RF24_EN_AA       0x01
#define RF24_EN_RXADDR   0x02
#define RF24_SETUP_AW    0x03
#define RF24_SETUP_RETR  0x04
#define RF24_RF_CH       0x05
#define RF24_RF_SETUP    0x06
#define RF24_STATUS      0x07
#define RF24_OBSERVE_TX  0x08
#define RF24_CD          0x09
#define RF24_RPD         0x09
//RX ADDRESS
#define RF24_RX_ADDR_P0  0x0A
#define RF24_RX_ADDR_P1  0x0B
#define RF24_RX_ADDR_P2  0x0C
#define RF24_RX_ADDR_P3  0x0D
#define RF24_RX_ADDR_P4  0x0E
#define RF24_RX_ADDR_P5  0x0F
//TX ADDRESS
#define RF24_TX_ADDR     0x10
// Number of bytes in RX payload in data pipe #
#define RF24_RX_PW_P0    0x11
#define RF24_RX_PW_P1    0x12
#define RF24_RX_PW_P2    0x13
#define RF24_RX_PW_P3    0x14
#define RF24_RX_PW_P4    0x15
#define RF24_RX_PW_P5    0x16

#define RF24_FIFO_STATUS 0x17
#define RF24_DYNPD       0x1C
#define RF24_FEATURE     0x1D

/* Register Bits */
//Address 00: CONFIGURATION register
#define RF24_MASK_RX_DR  BIT6
#define RF24_MASK_TX_DS  BIT5
#define RF24_MASK_MAX_RT BIT4
#define RF24_EN_CRC      BIT3
#define RF24_CRCO        BIT2
#define RF24_PWR_UP      BIT1
#define RF24_PRIM_RX     BIT0
//Address 01: Enhance ShockBurst
//Enable 'Auto Acknowledgment' Function Disable
//
#define RF24_ENAA_P5     BIT5
#define RF24_ENAA_P4     BIT4
#define RF24_ENAA_P3     BIT3
#define RF24_ENAA_P2     BIT2
#define RF24_ENAA_P1     BIT1
#define RF24_ENAA_P0     BIT0

//Address 02: EN_RXADDR
//Enable RX addresses
//
#define RF24_ERX_P5      BIT5 //pipe 5
#define RF24_ERX_P4      BIT4 //    4
#define RF24_ERX_P3      BIT3 //    3
#define RF24_ERX_P2      BIT2 //    2
#define RF24_ERX_P1      BIT1 //    1
#define RF24_ERX_P0      BIT0 //    0

//SETUP_AW
//Setup address width
#define RF24_AW          BIT0 // illegal
//SETUP_RETR
//Setup Automatic Retransmission
#define RF24_ARD         BIT4// delay 1250uS, see datasheet
#define RF24_ARC         BIT0// auto retransmit disable

//Address 06
//RF setup register
#define RF24_CONT_WAVE   BIT7
#define RF24_RF_DR       BIT3
#define RF24_RF_DR_LOW   BIT5
#define RF24_PLL_LOCK    BIT4
#define RF24_RF_DR_HIGH  BIT3
#define RF24_RF_PWR      BIT1// 12dBm
#define RF24_LNA_HCURR   BIT0

//Address 07
//Status register
#define RF24_RX_DR       BIT6 //Data ready RX FIFO interrupt
#define RF24_TX_DS       BIT5
#define RF24_MAX_RT      BIT4
#define RF24_RX_P_NO     BIT1
#define RF24_TX_FULL     BIT0

//Address 08
//Transmit observe register
#define RF24_PLOS_CNT    BIT4// Count lost packages
#define RF24_ARC_CNT     BIT0// Count retransmitted Packages

//Address 17 : FIFO status
#define RF24_TX_REUSE    BIT6
#define RF24_FIFO_FULL   BIT5
#define RF24_TX_EMPTY    BIT4
#define RF24_RX_FULL     BIT1
#define RF24_RX_EMPTY    BIT0
//Address 1D : Feature
#define RF24_EN_DPL      BIT2
#define RF24_EN_ACK_PAY  BIT1
#define RF24_EN_DYN_ACK  BIT0

/* Instructions */
#define RF24_R_REGISTER    0x00
#define RF24_W_REGISTER    0x20
#define RF24_REGISTER_MASK 0x1F
#define RF24_R_RX_PAYLOAD  0x61
#define RF24_W_TX_PAYLOAD  0xA0
#define RF24_FLUSH_TX      0xE1
#define RF24_FLUSH_RX      0xE2
#define RF24_REUSE_TX_PL   0xE3
#define RF24_R_RX_PL_WID   0x60
#define RF24_W_ACK_PAYLOAD 0xA8
#define RF24_W_TX_PAYLOAD_NOACK 0xB0
#define RF24_NOP           0xFF

#endif /* NRF24L01_H_ */
