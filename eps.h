//#ifndef EPS_H
//#define EPS_H
//
//#include<stdint.h>
//#include"AHAN.h"
//#include"global.h"
//#define VC1 0x40
//#define VC2 0x41
//#define VC3 0x43
//
//#define FG 0x36
//
//#define VC_CONFIG_REG 0x00
//
//#define VC_BUSV_CHx(x)                  ((0x2 * x))
//#define VC_SHUNTV_CHx(x)                ((0x2 * x - 1))
//
//#define VC_AVG_1                        0x0
//#define VC_AVG_4                        0x1
//#define VC_AVG_16                       0x2
//#define VC_AVG_64                       0x3
//#define VC_AVG_128                      0x4
//#define VC_AVG_256                      0x5
//#define VC_AVG_512                      0x6
//#define VC_AVG_1024                     0x7
//
//#define VC_CT_140u                      0x0
//#define VC_CT_204u                      0x1
//#define VC_CT_332u                      0x2
//#define VC_CT_588u                      0x3
//#define VC_CT_1100u                     0x4
//#define VC_CT_2116u                     0x5
//#define VC_CT_4156u                     0x6
//#define VC_CT_8244u                     0x7
//
//#define VC_MODE_BS_SS                   0x3
//
//#define VC_BUS_CT_SHIFT                 6
//#define VC_SHUNT_CT_SHIFT               3
//#define VC_AVG_SHIFT                    9
//
//#define VC_INIT_CMD                     0x7000
//
//#define FG_MODE_REG 0x06
//
//#define FG_CONFIG_REG 0x0C
//
//#define FG_SOC_REG                      0x04
//
//#define MAX_TX_SIZE 256
//
//#define VC_SENSOR_I2C &g_mss_i2c0
//
//#define FG_SENSOR_I2C &g_core_i2c1
//
//#define SOC_TH1             30
//
//#define SOC_TH2             70
//
//
//typedef struct {
//    double vc1v1;
//    double vc1v2;
//    double vc1v3;
//    double vc1i1;
//    double vc1i2;
//    double vc1i3;
//    double vc2v1;
//    double vc2v2;
//    double vc2v3;
//    double vc2i1;
//    double vc2i2;
//    double vc2i3;
//    double vc3v1;
//    double vc3v2;
//    double vc3v3;
//    double vc3i1;
//    double vc3i2;
//    double vc3i3;
//    double soc;
//}eps_data_t;
//
//uint8_t vc_write(uint8_t addr, uint8_t *tx, uint8_t tx_size);
//
//uint8_t vc_read(uint8_t addr, uint8_t *rx, uint8_t rx_size);
//
//uint8_t vc_init(uint8_t addr);
//
//uint8_t eps_init();
//
//uint8_t get_voltages(void);
//
//void eps_init_test();
//
//double get_soc();
//
//#endif
