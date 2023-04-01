#include "main.h"

unsigned short BD_Add_OddEven(unsigned short byte)
{
    unsigned char i;
    unsigned char n;
    unsigned short r;
    n = 0;
    for (i = 0; i < 10; i++) {
        if (((byte >> i) & 0x01) == 0x01) {
            n++;
        }
    }
    if ((n & 0x01) == 0x01) {
        r = byte | 0x400;
    } else {
        r = byte | 0x00;
    }
    return r;
}
unsigned short BD_Check_OddEven(unsigned short byte)
{
    unsigned char i;
    unsigned char n;
    unsigned char r;
    n = 0;
    for (i = 0; i < 10; i++) {
        if (((byte >> i) & 0x01) == 0x01) {
            n++;
        }
    }
    if ((byte >> 10) == (n & 0x01)) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

// 上位机 接收 下位机 输出，时钟由上位机控制
void kl_i2c_write(unsigned int write_data)
{

    write_data = BD_Add_OddEven(write_data);
    for (int i = 10; i >= 0; i--) {

        while (READ_SCL == HIGH) {
        }

        if ((write_data >> i) & 0x01)
            SET_SDA = 1;
        else
            SET_SDA = 0;

        while (READ_SCL == LOW) {
        }
    }

    while (READ_SCL == HIGH) {
    }
    SET_SDA = 0;
}

void kl_i2c_read()
{

    unsigned short READ_DATA = 0;

    for (int i = 0; i <= 10; i++) {
        READ_DATA <<= 1;
        // scl高时接收数据
        while (READ_SCL == LOW) // 等scl高以后，检测SDA值（高为发送，低为接收）
        {
        }

        if (READ_SDA == HIGH)
            READ_DATA |= 1;
        //  else
        //    READ_DATA |= 0;

        // 直到scl拉低

        while (READ_SCL == HIGH) // 等scl高以后，检测SDA值（高为发送，低为接收）
        {
        }
    }

    if (BD_Check_OddEven(READ_DATA) == 0) {
        // Serial.println("CRC-ERR");
        // Serial.println(READ_DATA);
    }

    // Serial.println(READ_DATA);
}

//
// 检测开始帧后面一个bit的sda是高电平还是低电平，高电平为写，低电平为读
// 检测开始帧后面一个bit的sda是高电平还是低电平，高电平为写，低电平为读
int kl_i2c_chk_read_write()
{
    int read_sda = 0;
    while (READ_SCL == LOW) // 等scl高以后，检测SDA值（高为发送，低为接收）
    {
    }
    // time_wait = 0;
    if (READ_SDA == HIGH) // 主机读，从机发送
    {
        // Serial.println("send");
        read_sda = 1;
    } else // 主机发送，从机读
    {
        // Serial.println("read");
        read_sda = 0;
    }

    while (READ_SCL == HIGH) // 等scl拉低以后，完成开始读/写检测，再继续
    {
    }
    // time_wait = 0;
    return read_sda;
}

void wait_end_frame()
{
    while (READ_SDA == HIGH) // 等sda拉低以后再继续
    {
    }

    while (READ_SCL == LOW) {
    }

    while (READ_SDA == LOW) {
    }
    // Serial.println("end_frame");
}

int mode                         = 0;
int time_wait                    = 0;
//bool start_output_calibrate_data = false;

void sw_i2c_start()
{
    if (READ_SDA == HIGH && READ_SCL == HIGH) {

        while (READ_SDA == HIGH) // 等sda拉低以后再继续
        {
        }

        while (READ_SCL == HIGH) // 等scl拉低以后，完成开始帧检测，再继续
        {
        }
        //mode = 2;
        // USBSerial.println("start-frame");
        //time_wait = 0;

        int Read_sda = kl_i2c_chk_read_write();

        if (Read_sda == 0)
            kl_i2c_read();

        else if (Read_sda == 1) {

            // if (start_output_calibrate_data == true) {
            // } else {
            //     // Gcode_buf += "read_once\n";
            // }
        } else
            return;

        wait_end_frame();
    }
}
