#ifndef  __TM1640_H__
#define  __TM1640_H__

#include "stdint.h"


#ifdef  cplusplus
#define TM1640_BEGIN  extern "C" {
#define TM1640_END    }
#else
#define TM1640_BEGIN  
#define TM1640_END   
#endif


TM1640_BEGIN

#define CLK_DELAY_TIME             20

typedef struct
{
void (*clk_rise)(void);
void (*clk_down)(void);
void (*data_set)(void);
void (*data_clr)(void);
}tm1640_hal_driver_t;


int tm1640_register_hal_driver(tm1640_hal_driver_t *hal_driver);
int tm1640_init();
int tm1640_factory_mode();

/*addr:0-15 地址自动增长*/
int tm1640_display(uint8_t addr,uint8_t *dis_data,uint8_t cnt);
/*指定地址显示，地址不自动增长*/
int tm1640_display_fixed(uint8_t addr,uint8_t dis_data);
/*brightness: 0-8*/
int tm1640_brightness(uint8_t brightness );









TM1640_END








#endif


















