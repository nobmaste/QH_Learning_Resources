#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // 提供 atoi()


// 定义支持的最大数据长度（最多255字节）
#define MAX_DATA_LEN 255
// 全局变量：保存当前帧的解析信息
extern uint8_t frame_type;               // 帧类型（0x00 = 数据帧，0x01 = 按钮帧）
extern uint8_t subtype;                  // 数据帧子类型（例如频率、幅度）
extern uint8_t info1;                    // 数据长度、跳转标志
extern uint8_t info2;                    // 按钮编号 或 跳转页编号
extern uint8_t data_len;                 // 数据长度
extern uint8_t data_buf[MAX_DATA_LEN + 1];   // 数据缓冲区（+1用于字符串终止符 '\0'）
extern uint8_t data_index;               // 数据当前接收索引

extern int fre,amp,ADCNUM;
extern uint16_t FLAG;


void handle_data_frame(uint8_t type, uint8_t *data, uint8_t len);
void handle_button_frame(uint8_t jump_flag, uint8_t page_or_btn);
void parse_byte(uint8_t byte);


#endif // __PROTOCOL_H__

