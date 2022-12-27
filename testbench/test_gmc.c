#include"test_gmc.h"
uint8_t g_slave_rx_buffer[10u];
uint8_t g_slave_tx_buffer[10u];
uint8_t databuff[10u];

uint8_t i2c_state;
gmc_i2c_state_t state;
uint16_t count;
uint16_t getcount() {
    return 0x4848;
}
mss_i2c_slave_handler_ret_t mss_slave_write_handler_1
    (
        mss_i2c_instance_t * this_i2c,
        uint8_t * p_rx_data,
        uint16_t rx_size
    )
{
	if(rx_size > BUFFER_SIZE) {
		rx_size = BUFFER_SIZE;
	}

    if(state == GMC_IDLE && g_slave_rx_buffer[0] == 0x88) {
        count = 0;
        state = GMC_COUNTING;
    }

    if(state == GMC_COUNTING && g_slave_rx_buffer[0] == 0x44) {
        count = getcount();
        g_slave_tx_buffer[0] = count & 0x00FF;
        g_slave_tx_buffer[1] = (count >> 8) & 0x00FF;
        state = GMC_IDLE;
    }

	return MSS_I2C_REENABLE_SLAVE_RX;
}

void i2c_slave_init() {
    TEST_GMC_INIT(test_gmc_i2c,test_gmc_addr,MSS_I2C_PCLK_DIV_960);

    MSS_I2C_set_slave_rx_buffer( test_gmc_i2c, g_slave_rx_buffer,
										 sizeof(g_slave_rx_buffer) );
    MSS_I2C_set_slave_tx_buffer(test_gmc_i2c,g_slave_tx_buffer,sizeof(g_slave_tx_buffer));
	MSS_I2C_register_write_handler( test_gmc_i2c, mss_slave_write_handler_1 );
	MSS_I2C_enable_slave( test_gmc_i2c );
    state = GMC_IDLE;
    count = 0;
}
