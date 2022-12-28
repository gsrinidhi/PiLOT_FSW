///*
// * AHAN.c
// *
// *  Created on: 25-Jun-2022
// *      Author: S-SPACE
// */
//
//#include "AHAN.h"
//#include<CMSIS/m2sxxx.h>
//#include<CMSIS/system_m2sxxx.h>
//
////Varialbes decleration
//uint8_t timer_flag;
//
////rtc
////mss_rtc_calendar_t now;
//
//
//uint8_t g_slave_rx_buffer[10u];
//uint8_t g_slave_tx_buffer[10u];
//uint8_t databuff[10u];
//
////UART instances
////UART_instance_t uart0;
////UART_instance_t uart1;
////UART_instance_t uart2;
////UART_instance_t uart3;
////UART_instance_t uart4;
////
//////I2C instances
////i2c_instance_t g_core_i2c0;
////i2c_instance_t g_core_i2c1;
////i2c_instance_t g_core_i2c2;
////i2c_instance_t g_core_i2c3;
////i2c_instance_t g_core_i2c4;
////i2c_instance_t g_core_i2c5;
//
//
//
//
//
//
//
//void time_to_count(uint32_t ms,uint32_t *upper_count,uint32_t *lower_count) {
//    *lower_count = (ms%FULL_SCALE_TIME_MS) * TIMER_COUNT_PER_MS;
//    *upper_count = (ms/FULL_SCALE_TIME_MS) * TIMER_COUNT_PER_MS;
//}
//
//
//void delay_ms_64(uint16_t ms) {
//	uint32_t tl,th,tph,tpl,tsh,tsl;
//	MSS_TIM64_get_current_value(&tsl,&tsh);
//	time_to_count(ms,&tph,&tpl);
//	while(1) {
//		MSS_TIM64_get_current_value(&tl,&th);
//		if(tsh-th >= tph && tsl - tl >= tpl) {
//			break;
//		}
//	}
//}
//void delay_ms(uint16_t ms) {
//	timer_flag = 1;
//	MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
//	MSS_TIM1_load_immediate(TIMER_COUNT_PER_MS * ms);
//	MSS_TIM1_start();
//	MSS_TIM1_enable_irq();
//	while(timer_flag);
//}
//
//void delay_flag(uint16_t ms) {
//	timer_flag = 1;
//	MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
//	MSS_TIM1_load_immediate(TIMER_COUNT_PER_MS * ms);
//	MSS_TIM1_enable_irq();
//	MSS_TIM1_start();
//
//
//}
//
//void GPIO_Init() {
//	MSS_GPIO_init();
//
//	gpio1_irq_cnt = gpio2_irq_cnt = gpio3_irq_cnt = gpio4_irq_cnt = 0;
//	MSS_GPIO_config(MSS_GPIO_19,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
//	MSS_GPIO_enable_irq(MSS_GPIO_19);
//	MSS_GPIO_config(MSS_GPIO_20,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
//	MSS_GPIO_enable_irq(MSS_GPIO_20);
//	MSS_GPIO_config(MSS_GPIO_4,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
//	MSS_GPIO_enable_irq(MSS_GPIO_4);
//	MSS_GPIO_config(MSS_GPIO_26,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
//	MSS_GPIO_enable_irq(MSS_GPIO_26);
//	MSS_GPIO_config(MSS_GPIO_13,MSS_GPIO_OUTPUT_MODE);
//	MSS_GPIO_config(GMC_EN_PIN,MSS_GPIO_OUTPUT_MODE);
//}
//
//mss_i2c_slave_handler_ret_t mss_slave_write_handler
//    (
//        mss_i2c_instance_t * this_i2c,
//        uint8_t * p_rx_data,
//        uint16_t rx_size
//    )
//{
//	if(rx_size > BUFFER_SIZE) {
//		rx_size = BUFFER_SIZE;
//	}
//
//	uint8_t loop_count;
//	for(loop_count = 0;loop_count < rx_size;loop_count++){
//		databuff[loop_count] = g_slave_rx_buffer[loop_count];
//	}
//
//	return MSS_I2C_REENABLE_SLAVE_RX;
//}
//
//void I2C_Init() {
//	MSS_I2C_init( &g_mss_i2c0, DUMMY_I2C_ADDR, MSS_I2C_PCLK_DIV_960 );
//	ACDH.I2C_0 = I2C_INITIALISED;
//	MSS_I2C_init(&g_mss_i2c1,DUMMY_I2C_ADDR,MSS_I2C_PCLK_DIV_960);
//	ACDH.I2C_1 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c0, COREI2C_0_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_2 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c1, COREI2C_1_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_3 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c2, COREI2C_2_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_4 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c3, COREI2C_3_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_5 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c4, COREI2C_4_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_6 = I2C_INITIALISED;
//	I2C_init(&g_core_i2c5, COREI2C_5_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//	ACDH.I2C_7 = I2C_INITIALISED;
//
//	MSS_I2C_set_slave_rx_buffer( &g_mss_i2c1, g_slave_rx_buffer,
//										 sizeof(g_slave_rx_buffer) );
//	g_slave_tx_buffer[0] = 0x44;
//	g_slave_tx_buffer[1] = 0x48;
//	MSS_I2C_set_slave_tx_buffer(&g_mss_i2c1,g_slave_tx_buffer,sizeof(g_slave_tx_buffer));
//	MSS_I2C_register_write_handler( &g_mss_i2c1, mss_slave_write_handler );
//	MSS_I2C_enable_slave( &g_mss_i2c1 );
//
//}
//
//void Uart_Init() {
//
//	UART_init(&uart0,COREUARTAPB_0_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
//	UART_init(&uart1,COREUARTAPB_1_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
//	UART_init(&uart2,COREUARTAPB_2_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
//	UART_init(&uart3,COREUARTAPB_3_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
//	//UART_init(&uart4,COREUARTAPB_4_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
//}
//
//void AHAN_Init() {
//	GPIO_Init();
//	I2C_Init();
//	Uart_Init();
//	MSS_RTC_init(MSS_RTC_CALENDAR_MODE,RTC_PRESCALER);
//	SD_Init();
//	mss_rtc_calendar_t tst;
//	tst.day = 18;
//	tst.hour = 6;
//	tst.minute = 0;
//	tst.second = 0;
//	tst.month = 8;
//	tst.week = 1;
//	tst.weekday = MSS_RTC_SUNDAY;
//	MSS_RTC_set_calendar_count(&tst);
//	ACDH.A_RTC = 1;
//}
//
//uint8_t ADC_Init(){
//	uint8_t status;
//	uint8_t data_high_1 = DATA_HIGH_REG(0);
//	uint8_t DATA_HIGH_REG1[] = {data_high_1};
//	uint16_t conv_res1,conv_res2,value;
//	double voltage;
//	//Write max and min values to DATA HIGH and DATA LOW registers respectively for CH1
//	uint8_t DATA_HIGH[] = {data_high_1,DATAHIGH_MAX_H,DATAHIGH_MAX_L};
//	uint8_t DATA_LOW[] = {DATA_LOW_REG(0),DATA_LOW_MIN_H,DATA_LOW_MIN_L};
//	uint8_t test_buff[10] = {0xF1,0x1F};
//	I2C_write(&g_core_i2c4,ADC_I2C_ADDR,DATA_HIGH,3,I2C_RELEASE_BUS);
//	status = I2C_wait_complete(&g_core_i2c4, I2C_NO_TIMEOUT);
//	I2C_write(&g_core_i2c4,ADC_I2C_ADDR,DATA_LOW,sizeof(DATA_LOW),I2C_RELEASE_BUS);
//	status = I2C_wait_complete(&g_core_i2c4, I2C_NO_TIMEOUT);
//	return 0;
//}
//
//double get_ADC_value(uint8_t chx) {
//	uint8_t exp[2];
//	uint8_t ch_read[] = {chx};
//	uint8_t status;
//	uint16_t conv_res,value;
//	double voltage;
//	I2C_write_read(&g_core_i2c4,ADC_I2C_ADDR,ch_read,1,exp,2,I2C_RELEASE_BUS);
//	status = I2C_wait_complete(&g_core_i2c4, I2C_NO_TIMEOUT);
//	conv_res = (exp[0] << 8 ) | exp[1];
//	value = conv_res & 0x0FFF;
//	value = value >> 2;
//	voltage = value * 3.3/1024;
//	return voltage;
//}
//
//
//
//
//uint8_t IMU_Init() {
//	i2c_status_t status;
//	SYSREG->WDOG_CR = 0;
//
//	uint8_t rx_buffer[RX_LENGTH];
//
//	uint8_t rx_buffer_xL_a[RX_LENGTH];
//	uint8_t rx_buffer_xH_a[RX_LENGTH];
//	uint8_t rx_buffer_yL_a[RX_LENGTH];
//	uint8_t rx_buffer_yH_a[RX_LENGTH];
//	uint8_t rx_buffer_zL_a[RX_LENGTH];
//	uint8_t rx_buffer_zH_a[RX_LENGTH];
//
//	uint8_t read_length_a = RX_LENGTH;
//
//	uint8_t write_length_a = TX_LENGTH;
//
//	uint8_t IMU_slave_addr = 0x6b;
//
//	uint8_t read_who_am_I[TX_LENGTH] = {0x0F};
//	uint8_t readCTRL_REG1_G[TX_LENGTH] = {0x10};
//	uint8_t read_CTRL_REG6_XL[TX_LENGTH] = {0x20};
//
//	uint8_t write_CTRL_REG1_G[TX_LENGTH+1] = {0x10,0x60};
//	uint8_t write_CTRL_REG6_XL[TX_LENGTH+1] = {0x20,0x60};
//
//	uint8_t read_ACC_out_X_L[TX_LENGTH] = {0x28};
//	uint8_t read_ACC_out_Y_L[TX_LENGTH] = {0x2A};
//	uint8_t read_ACC_out_Z_L[TX_LENGTH] = {0x2C};
//	uint8_t read_ACC_out_X_H[TX_LENGTH] = {0x29};
//	uint8_t read_ACC_out_Y_H[TX_LENGTH] = {0x2B};
//	uint8_t read_ACC_out_Z_H[TX_LENGTH] = {0x2D};
//	NVIC_SetPriority(SysTick_IRQn, 0xFFu);
//	I2C_init(&g_core_i2c5, COREI2C_5_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
//
//	while(1) {
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,write_CTRL_REG6_XL,write_length_a+1,rx_buffer,
//						read_length_a,I2C_RELEASE_BUS);
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Z_L,write_length_a,rx_buffer_zL_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Z_H,write_length_a,rx_buffer_zH_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_X_L,write_length_a,rx_buffer_xL_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_X_H,write_length_a,rx_buffer_xH_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Y_L,write_length_a,rx_buffer_yL_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//		I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Y_H,write_length_a,rx_buffer_yH_a,
//								read_length_a,I2C_RELEASE_BUS);
//
//		status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
//
//
//		uint16_t i = 0x0000;
//		uint8_t j = 0x00;
//
//		while(i<0xFFFF) {
//			i+=0x000F;
//			j = 0x00;
//			while(j < 0xFF) {
//				j+=0x01;
//			}
//		}
//	}
//
//
//	while(timer_flag) {
//
//	}
//}
//uint8_t compare(uint8_t *first, uint8_t *second,uint8_t size) {
//	uint8_t i = 0;
//	for(;i<size;i++) {
//		if(first[i]!=second[i]){
//			return 1;
//		}
//	}
//	return 0;
//}
//uint8_t test_peripherals() {
//	//Testing the I2C peripherals
//	i2c_status_t status;
//	uint8_t tx_byte[] = {0x88,0x48},limit = 10,count_w = 0,count_r = 0,rx_byte[10];
//	uint8_t f_i2c = 0,f_uart = 0,f = 0;
//	i2c_instance_t* i2c_array[6] = {&g_core_i2c0,&g_core_i2c1,&g_core_i2c2,&g_core_i2c3};
//	while(1) {
//		//Testing the fabric i2c
//		uint8_t i2c_i = 0;
//		for(;i2c_i < 4;i2c_i++) {
//			count_w = 0;
//			count_r = 0;
//			while(count_w < limit) {
//				I2C_write( i2c_array[i2c_i], I2C_SLAVE_ADDRESS, tx_byte, 2,I2C_RELEASE_BUS );
//				status = I2C_wait_complete(i2c_array[i2c_i], I2C_NO_TIMEOUT);
//				if(status == I2C_SUCCESS) {
//					f = 1;
//					break;
//				}
//				count_w++;
//			}
//			while(count_r < limit) {
//				I2C_read( i2c_array[i2c_i], I2C_SLAVE_ADDRESS, rx_byte, 2,I2C_RELEASE_BUS );
//				status = I2C_wait_complete(i2c_array[i2c_i], I2C_NO_TIMEOUT);
//				if(status == I2C_SUCCESS) {
//					f = 1;
//					break;
//				}
//				count_r++;
//			}
//			if((compare(tx_byte,g_slave_rx_buffer,2) == 1) || (compare(rx_byte,g_slave_tx_buffer,2) == 1) || count_w >= limit || count_r >= limit) {
//				f_i2c |= (1 << i2c_i);
//			}
//		}
//		return f_i2c;
//		f_uart = 0;
//
//	}
//}
//// void FabricIrq0_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c0);
//// }
//
//// void FabricIrq1_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c1);
//// }
//
//// void FabricIrq2_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c2);
//// }
//
//// void FabricIrq3_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c3);
//// }
//
//// void FabricIrq4_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c4);
//// }
//
//// void FabricIrq5_IRQHandler(void)
//// {
////     I2C_isr(&g_core_i2c5);
//// }
//
//// void Timer1_IRQHandler(void) {
//// 	timer_flag = 0;
//// 	MSS_TIM1_clear_irq();
//// }
//
//// void GPIO19_IRQHandler(void) {
//
//// 	uart_rx_size = UART_get_rx(&uart1,uart_rx_buff,20);
//// 	gpio1_irq_cnt++;
//// 	MSS_GPIO_clear_irq( MSS_GPIO_19 );
//// }
//
//// void GPIO20_IRQHandler(void) {
//// 	MSS_GPIO_clear_irq( MSS_GPIO_20 );
//// 	gpio2_irq_cnt++;
//// }
//
//// void GPIO4_IRQHandler(void) {
//// 	MSS_GPIO_clear_irq( MSS_GPIO_4 );
//// 	gpio3_irq_cnt++;
//// }
//
//// void GPIO26_IRQHandler(void) {
//// 	MSS_GPIO_clear_irq( MSS_GPIO_26 );
//// 	gpio4_irq_cnt++;
//// }
