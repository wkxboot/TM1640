#ifndef  __TM1640_H__
#define  __TM1640_H__

#include "stdint.h"


#ifdef  __cplusplus
#define TM1640_BEGIN  extern "C" {
#define TM1640_END    }
#else
#define TM1640_BEGIN  
#define TM1640_END   
#endif


TM1640_BEGIN


#define  TM1640_CONNECT_TYPE_ANODE    1
#define  TM1640_CONNECT_TYPE_CATHODE  2
#define  TM1640_CONNECT_TYPE               TM1640_CONNECT_TYPE_CATHODE
#define  CLK_DELAY_TIME               20

typedef struct
{
void (*clk_rise)(void);
void (*clk_down)(void);
void (*data_set)(void);
void (*data_clr)(void);
}tm1640_hal_driver_t;

/*注册硬件驱动*/
int tm1640_register_hal_driver(tm1640_hal_driver_t *hal_driver);
/*初始化*/
int tm1640_init();
/*本地显存清零*/
int tm1640_buffer_clean(void);
/*
*addr:0-15
*cnt :0-16
*/
int tm1640_buffer_update(uint8_t addr,uint8_t *update,uint8_t cnt);
/*刷新显示*/
int tm1640_display_refresh(void);

/*brightness: 0-8*/
int tm1640_brightness(uint8_t brightness );




TM1640_END



#endif


















