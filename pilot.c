#include"pilot.h"

uint8_t ADC_Init(i2c_instance_t i2c_chx,uint8_t address){
	uint8_t status;
	uint8_t channel = 0;
	uint8_t return_value = 0;
	//Write max and min values to DATA HIGH and DATA LOW registers respectively for all channels
	uint8_t DATA_HIGH[] = {DATA_HIGH_REG(0),DATAHIGH_MAX_H,DATAHIGH_MAX_L};
	uint8_t DATA_LOW[] = {DATA_LOW_REG(0),DATA_LOW_MIN_H,DATA_LOW_MIN_L};
	for(;channel <= 3;channel++) {
		DATA_HIGH[0] = DATA_HIGH_REG(channel);
		DATA_LOW[0] = DATA_LOW_REG(channel);
		I2C_write(&i2c_chx,address,DATA_HIGH,sizeof(DATA_HIGH),I2C_RELEASE_BUS);
		status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
		I2C_write(&i2c_chx,address,DATA_LOW,sizeof(DATA_LOW),I2C_RELEASE_BUS);
		status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
		return_value |= (status << channel);
	}
	
	return return_value;
}

double get_ADC_value(i2c_instance_t i2c_chx,uint8_t address,uint8_t chx) {
	uint8_t adc_read_value[2];
	uint8_t ch_read[] = {chx};
	ch_read[0] |= 0x8;
	ch_read[0] = ch_read[0] << 4;
	uint8_t status;
	uint16_t conv_res,value;
	double voltage;
	I2C_write_read(&i2c_chx,address,ch_read,1,adc_read_value,2,I2C_RELEASE_BUS);
	status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
	if(status != 0) {
		voltage = -1;
	} else {
		conv_res = (adc_read_value[0] << 8 ) | adc_read_value[1];
		value = conv_res & 0x0FFF;
		value = value >> 2;
		voltage = value * 3.3/1024;
	}
	return voltage;
}

void I2C_Init() {
	MSS_I2C_init( &g_mss_i2c0, DUMMY_I2C_ADDR, MSS_I2C_PCLK_DIV_960 );
	MSS_I2C_init(&g_mss_i2c1,DUMMY_I2C_ADDR,MSS_I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c0, COREI2C_0_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c1, COREI2C_1_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c2, COREI2C_2_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c3, COREI2C_3_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c4, COREI2C_4_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c5, COREI2C_5_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);

}

void GPIO_Init() {
	MSS_GPIO_init();
	gpio1_irq_cnt = gpio2_irq_cnt = gpio3_irq_cnt = gpio4_irq_cnt = 0;
	MSS_GPIO_config(MSS_GPIO_19,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_19);
	MSS_GPIO_config(MSS_GPIO_20,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_20);
	MSS_GPIO_config(MSS_GPIO_4,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_4);
	MSS_GPIO_config(MSS_GPIO_26,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_26);
	MSS_GPIO_config(MSS_GPIO_13,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(GMC_EN_PIN,MSS_GPIO_OUTPUT_MODE);
}

void Uart_Init() {
	
	UART_init(&uart0,COREUARTAPB_0_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart1,COREUARTAPB_1_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart2,COREUARTAPB_2_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart3,COREUARTAPB_3_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart4,COREUARTAPB_4_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
}

void Pilot_Init() {
	GPIO_Init();
	I2C_Init();
	Uart_Init();
	SD_Init();
	MSS_TIM64_init(MSS_TIMER_ONE_SHOT_MODE);
}

//void get_CDH_HK() {
//
//}

void FabricIrq0_IRQHandler(void)
{
    I2C_isr(&g_core_i2c0);
}

void FabricIrq1_IRQHandler(void)
{
    I2C_isr(&g_core_i2c1);
}

void FabricIrq2_IRQHandler(void)
{
    I2C_isr(&g_core_i2c2);
}

void FabricIrq3_IRQHandler(void)
{
    I2C_isr(&g_core_i2c3);
}

void FabricIrq4_IRQHandler(void)
{
    I2C_isr(&g_core_i2c4);
}

void FabricIrq5_IRQHandler(void)
{
    I2C_isr(&g_core_i2c5);
}

//void Timer1_IRQHandler(void) {
//	timer_flag = 0;
//	MSS_TIM1_clear_irq();
//}

void GPIO19_IRQHandler(void) {
	gpio1_irq_cnt++;
	MSS_GPIO_clear_irq( MSS_GPIO_19 );
}

void GPIO20_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_20 );
	gpio2_irq_cnt++;
}

void GPIO4_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_4 );
	gpio3_irq_cnt++;
}

void GPIO26_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_26 );
	gpio4_irq_cnt++;
}
