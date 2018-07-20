#include "tm1640.h"


static tm1640_hal_driver_t* driver;


#ifndef NULL
#define NULL      ((void*)0)
#endif



#ifndef  TRUE 
#define  TRUE     (1)
#endif


#ifndef  FALSE 
#define  FALSE    (0)
#endif


#ifndef  ASSERT_NULL_PTR
#define  ASSERT_NULL_PTR(x)     \
{                               \
	     if((x)== NULL){        \
		 return -1;             \
	     }                      \
}
#endif

/*
* 命令类别
*/
#define  MODE_CMD_ID                (1<<6)
#define  DIS_CTRL_CMD_ID            (1<<7)
#define  ADDR_CMD_ID                ((1<<6)|(1<<7))

/*
*模式控制命令
*/
#define  MODE_CMD_ADDR_INCREASE     (0)
#define  MODE_CMD_ADDR_FIXED        (1<<2)
#define  MODE_CMD_NORMAL_MODE       (0)
#define  MODE_CMD_FACTORY_MODE      (1<<3)

#define  MODE_CMD_MASK              (0x0C)
/*
*地址命令掩码
*/

#define  ADDR_CMD_MASK              (0x0F)
/*
*显示控制命令
*/
#define  DIS_CTRL_CMD_ON            (1<<3)       
#define  DIS_CTRL_CMD_OFF           (0)  
#define  DIS_CTRL_DUTY_1_16         (0)
#define  DIS_CTRL_DUTY_2_16         (1)
#define  DIS_CTRL_DUTY_4_16         (2)
#define  DIS_CTRL_DUTY_10_16        (3)
#define  DIS_CTRL_DUTY_11_16        (4)
#define  DIS_CTRL_DUTY_12_16        (5)
#define  DIS_CTRL_DUTY_13_16        (6)
#define  DIS_CTRL_DUTY_14_16        (7)

#define  DIS_CTRL_CMD_MASK          (0x0F)   








int tm1640_register_hal_driver(tm1640_hal_driver_t *hal_driver)
{
	ASSERT_NULL_PTR(hal_driver);
	ASSERT_NULL_PTR(hal_driver->clk_rise);
	ASSERT_NULL_PTR(hal_driver->clk_down);
	ASSERT_NULL_PTR(hal_driver->data_set);
	ASSERT_NULL_PTR(hal_driver->data_clr);
    driver=hal_driver;
	
    return 0;
}

static volatile uint16_t clk_delay=0;

static void delay_ns(uint16_t ns)
{
  clk_delay=ns;
  while(clk_delay--);
}

static void write_byte(uint8_t byte)
{
 uint8_t pos;
 for(pos =0; pos < 8;pos++){
  driver->clk_down();
  delay_ns(CLK_DELAY_TIME);
  if(byte & (1<<pos)){
  driver->data_set();
  }else{
  driver->data_clr();
  }
  delay_ns(CLK_DELAY_TIME);
  driver->clk_rise();
  delay_ns(CLK_DELAY_TIME);
}

}

static void start(void)
{
 driver->clk_rise();
 delay_ns(CLK_DELAY_TIME);
 driver->data_clr();
 delay_ns(CLK_DELAY_TIME);
}

static void end(void)
{
 driver->clk_down();
 delay_ns(CLK_DELAY_TIME);
 driver->data_clr();
 delay_ns(CLK_DELAY_TIME);
 driver->clk_rise();
 delay_ns(CLK_DELAY_TIME);
 driver->data_set();
 delay_ns(CLK_DELAY_TIME);  
}


static void set_addr(uint8_t addr)
{
 write_byte(ADDR_CMD_ID|(ADDR_CMD_MASK & addr));
}


static void set_mode(uint8_t mode)
{
start();
write_byte(MODE_CMD_ID|(MODE_CMD_MASK & mode));
end();
}

static void display_ctrl(uint8_t dis_ctrl)
{
start();
write_byte(DIS_CTRL_CMD_ID|(DIS_CTRL_CMD_MASK & dis_ctrl));
end();
}

static void display_data(uint8_t dis_data)
{
write_byte(dis_data);
}

/*指定地址显示，地址不自动增长*/
int tm1640_display_fixed(uint8_t addr,uint8_t dis_data)
{
start();
set_addr( addr);
display_data(dis_data);
end(); 
return 0;
}
/*地址自动增长*/
int tm1640_display(uint8_t addr,uint8_t *dis_data,uint8_t cnt)
{
uint8_t i;
start();
set_addr(addr);
for(i=0;i<cnt;i++){
display_data(*dis_data++);
}
end();
return 0;
}

/*brightness 0-8*/
int tm1640_brightness(uint8_t brightness )
{
if(brightness == 0){
display_ctrl(DIS_CTRL_CMD_OFF);	
}else{
display_ctrl(DIS_CTRL_CMD_ON|(brightness-1));
}
return 0;
}

int tm1640_init()
{
ASSERT_NULL_PTR(driver);
ASSERT_NULL_PTR(driver->clk_rise);
ASSERT_NULL_PTR(driver->clk_down);
ASSERT_NULL_PTR(driver->data_set);
ASSERT_NULL_PTR(driver->data_clr);

 set_mode(MODE_CMD_ADDR_FIXED|MODE_CMD_NORMAL_MODE);
 display_ctrl(DIS_CTRL_CMD_ON|DIS_CTRL_DUTY_10_16);

 return 0;
}


int tm1640_factory_mode()
{
	ASSERT_NULL_PTR(driver);
	ASSERT_NULL_PTR(driver->clk_rise);
	ASSERT_NULL_PTR(driver->clk_down);
	ASSERT_NULL_PTR(driver->data_set);
	ASSERT_NULL_PTR(driver->data_clr);
	set_mode(MODE_CMD_FACTORY_MODE);
	
	return 0;
}




















