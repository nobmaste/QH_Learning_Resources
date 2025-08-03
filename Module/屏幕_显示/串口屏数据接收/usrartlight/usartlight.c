#include "usartlight.h"

//待更新
typedef enum {
    WAIT_HEADER,          // 等待帧头 0x55
    WAIT_TYPE,            // 帧类型：0x00=数据帧，0x01=按钮帧
    WAIT_SUBTYPE,         // 数据帧子类型：如 0x00=频率，0x01=幅度（只对数据帧有效）
    WAIT_LEN_OR_JUMP,     // 数据帧：等待数据长度；按钮帧：等待跳转标志
    WAIT_INFO2,           // 数据帧无此字段；按钮帧：等待跳转目标页编号 或 按钮编号
    WAIT_DATA,            // 接收数据帧的有效数据
    WAIT_FF1,             // 帧尾校验字节1（必须为 0xFF）
    WAIT_FF2,             // 帧尾校验字节2
    WAIT_FF3              // 帧尾校验字节3，之后触发解析处理
} ParseState;

uint8_t frame_type = 0;               // 帧类型（0x00 = 数据帧，0x01 = 按钮帧）
uint8_t subtype = 0;                  // 数据帧子类型（例如频率、幅度）
uint8_t info1 = 0;                    // 数据长度、跳转标志
uint8_t info2 = 0;                    // 按钮编号 或 跳转页编号
uint8_t data_len = 0;                 // 数据长度
uint8_t data_buf[MAX_DATA_LEN + 1];   // 数据缓冲区（+1用于字符串终止符 '\0'）
uint8_t data_index = 0;               // 数据当前接收索引

int fre,amp,ADCNUM;
uint16_t FLAG=0x0000;

void handle_data_frame(uint8_t type, uint8_t *data, uint8_t len);       // 数据帧处理函数
void handle_button_frame(uint8_t jump_flag, uint8_t page_or_btn);       // 按钮帧处理函数


void parse_byte(uint8_t byte) {
    static ParseState state = WAIT_HEADER;   // 当前解析状态（初始为等待帧头）

    switch (state) {

        // ---- 帧头识别 ----
        case WAIT_HEADER:
            if (byte == 0x55)
                state = WAIT_TYPE;
            break;

        // ---- 帧类型 ----
        case WAIT_TYPE:
            frame_type = byte;
            if (frame_type == 0x00) {
                state = WAIT_SUBTYPE;         // 数据帧 -> 等待子类型（频率/幅度）
            } else if (frame_type == 0x01) {
                state = WAIT_LEN_OR_JUMP;     // 按钮帧 -> 直接跳到跳转标志判断
            } else {
                state = WAIT_HEADER;          // 非法帧类型，重置
            }
            break;

        // ---- 数据帧的子类型 ----
        case WAIT_SUBTYPE:
            subtype = byte;                  // 保存频率/幅度类型
            state = WAIT_LEN_OR_JUMP;        // 接下来是长度
            break;

        // ---- 数据长度 或 按钮跳转标志 ----
        case WAIT_LEN_OR_JUMP:
            info1 = byte;
            if (frame_type == 0x00) {
                // 对于数据帧，info1 是字符串长度
                data_len = info1;
                data_index = 0;
                if (data_len > 0 && data_len <= MAX_DATA_LEN) {
                    state = WAIT_DATA;
                } else {
                    state = WAIT_HEADER;  // 非法长度
                }
            } else if (frame_type == 0x01) {
                // 按钮帧：info1 是跳转标志（0=不跳转，1=跳转）
                state = WAIT_INFO2;
            } else {
                state = WAIT_HEADER;
            }
            break;

        // ---- 接收按钮编号 或 跳转页面编号 ----
        case WAIT_INFO2:
            info2 = byte;
            state = WAIT_FF1;
            break;

        // ---- 数据接收 ----
        case WAIT_DATA:
            data_buf[data_index++] = byte;
            if (data_index >= data_len) {
                data_buf[data_len] = '\0';    // 补充字符串结束符
                state = WAIT_FF1;
            }
            break;

        // ---- 帧尾识别 ----
        case WAIT_FF1:
            if (byte == 0xFF)
                state = WAIT_FF2;
            else
                state = WAIT_HEADER;
            break;

        case WAIT_FF2:
            if (byte == 0xFF)
                state = WAIT_FF3;
            else
                state = WAIT_HEADER;
            break;

        case WAIT_FF3:
            if (byte == 0xFF) {
                // 完整一帧数据接收完成，触发处理
                if (frame_type == 0x00) {
                    handle_data_frame(subtype, data_buf, data_len);
                } else if (frame_type == 0x01) {
                    handle_button_frame(info1, info2);
                }
            }
            // 无论成功与否，都回到等待帧头
            state = WAIT_HEADER;
            break;
    }
}

/*
				flag
				0x0000 未接受到数据/复位
				0x1001 频率需要更新
				0x1010 幅度需要更新
				0x1011 频率幅度都需要更新
				0x0001 未发生界面跳转，0界面1按钮更新
				0x0021 未发生界面跳转，2界面1按钮更新
				0x0101 发生界面跳转。跳转至界面1
				*/

// 处理数据帧
// subtype: 00=频率，01=幅度，02=ADC数据
void handle_data_frame(uint8_t type, uint8_t *data, uint8_t len) 
	{
		FLAG|=0x1000;
    if (type == 0x00) 
		{
        // 如果子类型是 0x00，表示设置频率
//        printf("【频率设置】接收到频率字符串: %.*s\n", len, data);
				char temp[256] = {0};
				memcpy(temp, data, len);  // 复制为 null 结尾字符串
				fre = atoi(temp);    // 转为整数
				FLAG|=0x0001;
    } 
		if (type == 0x01) 
		{
        // 如果子类型是 0x01，表示设置幅度
//        printf("【幅度设置】接收到幅度字符串: %.*s\n", len, data);
				char temp[256] = {0};
				memcpy(temp, data, len);  // 复制为 null 结尾字符串
				amp = atoi(temp);    // 转为整数
				FLAG|=0x0010;
    } 
				if (type == 0x02) 
		{
        // 如果子类型是 0x02，表示设置ADC数据
//        printf("【幅度设置】接收到幅度字符串: %.*s\n", len, data);
				char temp[256] = {0};
				memcpy(temp, data, len);  // 复制为 null 结尾字符串
				ADCNUM = atoi(temp);    // 转为整数
				FLAG|=0x0100;
    } 
}

// 处理按钮帧
// jump_flag: 是否跳转界面（1=跳转，0=当前界面按钮）
// page_or_btn: 跳转的页面号或按钮编号
void handle_button_frame(uint8_t jump_flag, uint8_t page_or_btn) 
{
    if (jump_flag == 1) 
		{
        // 如果 jump_flag 为 1，表示跳转界面
//        printf("【跳转请求】跳转到界面 %d\n", page_or_btn);
			FLAG&=~0xF;
			FLAG|=0x0100;
			FLAG|=page_or_btn&0x000F;
    } 
		else 
		{
        // 如果 jump_flag 为 0，表示当前界面上的按钮被按下
//        printf("【按钮事件】按钮 %d 被按下（当前界面）\n", page_or_btn);
		    FLAG|=page_or_btn;
    }
}

