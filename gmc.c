//#include"gmc.h"
//
////gmc_packet_t gmc;
//i2c_status_t gmc_i2c_status;
//uint8_t GMCInit() {
//    MSS_GPIO_set_output( GMC_EN_PIN,1);
//    if(Ahan.AHAN_CDH.I2C_5 != 1) {
//        I2C_init(&g_core_i2c3, COREI2C_3_0, ADC_I2C_ADDR, I2C_PCLK_DIV_256);
//    }
//
//    return 0;
//}
//
//uint8_t gmc_count_init() {
//    uint8_t tx[] = {GMC_COUNT_INIT};
//    uint8_t i = 0;
//    while (1)
//    {
//        I2C_write(GMC_I2C,GMC_ADDR,tx,1,I2C_RELEASE_BUS);
//        gmc_i2c_status = I2C_wait_complete(GMC_I2C,I2C_NO_TIMEOUT);
//        if(gmc_i2c_status == I2C_SUCCESS) {
//            ACDH.I2C_5 = 2;
//            return 0;
//        } else {
//            ACDH.I2C_5 = 3;
//            i++;
//        }
//        if(i >=10) {
//            break;
//        }
//    }
//    return 1;
//}
//
//uint8_t gmc_get_count() {
//    uint8_t tx[] = {GMC_COUNT_STOP};
//    uint16_t count;
//    uint8_t i = 0;
//    while (1)
//    {
//        I2C_write(GMC_I2C,GMC_ADDR,tx,1,I2C_RELEASE_BUS);
//        gmc_i2c_status = I2C_wait_complete(GMC_I2C,I2C_NO_TIMEOUT);
//        if(gmc_i2c_status == I2C_SUCCESS) {
//            ACDH.I2C_5 = 2;
//            break;
//        } else {
//            ACDH.I2C_5 = 3;
//            i++;
//        }
//        if(i >=10) {
//            break;
//        }
//    }
//    if(i>=10) {
//        return 1;
//    }
//
//    while(1) {
//        I2C_read(GMC_I2C,GMC_ADDR,(uint8_t*)&count,2,I2C_RELEASE_BUS);
//        gmc_i2c_status = I2C_wait_complete(GMC_I2C,I2C_NO_TIMEOUT);
//        if(gmc_i2c_status == I2C_SUCCESS) {
//            ACDH.I2C_5 = 2;
//            break;
//        } else {
//            ACDH.I2C_5 = 3;
//            i++;
//        }
//        if(i >=10) {
//            break;
//        }
//    }
//    if(i>=10) {
//        return 1;
//    }
//    MSS_RTC_get_calendar_count(&now);
//    gmc.gmc_time = now;
//    gmc.rcount = count;
//    if(gmc_store_data((uint8_t*)&gmc)) {
//        return 1;
//    }
//
//    return 0;
//
//
//}
//uint8_t getGMCData() {
//    uint8_t tx[] = {GMC_Req_Data};
//    uint16_t rx;
//    uint16_t curr_sect,curr_offset;
//    uint8_t psector[512],gsector[512];
//    uint8_t i = 0;
//    while(1) {
//        I2C_write_read(&g_core_i2c3,GMC_ADDR,tx,sizeof(tx),(uint8_t*)&rx,sizeof(rx),I2C_RELEASE_BUS);
//        core_i2c_status = I2C_wait_complete(&g_core_i2c3,I2C_NO_TIMEOUT);
//
//        if(core_i2c_status == I2C_SUCCESS) {
//            ACDH.I2C_5 = 2;
//            break;
//        } else {
//            ACDH.I2C_5 = 3;
//            i++;
//        }
//        if(i >=10) {
//            break;
//        }
//    }
//    MSS_RTC_get_calendar_count(&now);
//    gmc.gmc_time = now;
//    gmc.rcount = rx;
//    SD_Read(P_SECTOR,psector);
//    curr_sect = psector[GMC_SECT] | (psector[GMC_SECT+1] << 8);
//    curr_offset = psector[GMC_OFFSET] | (psector[GMC_OFFSET+1] << 8);
//    SD_Read(curr_sect,gsector);
//    if((curr_offset + sizeof(gmc)) > 510 ) {
//        curr_sect+=512;
//        curr_offset = 0;
//    }
////    uint8_t i = 0;
//    for(i = 0;i<sizeof(gmc);i++) {
//        gsector[curr_offset+i] = *((uint8_t*)(&gmc + i));
//    }
//    SD_Write(curr_sect,gsector);
//    psector[GMC_SECT] = curr_sect & 0xff;
//    psector[GMC_SECT+1] = curr_sect >> 8;
//    psector[GMC_OFFSET] = curr_offset & 0xff;
//    psector[GMC_OFFSET+1] = curr_offset >> 8;
//    SD_Write(P_SECTOR,psector);
//    return 0;
//}
