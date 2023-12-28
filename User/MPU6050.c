#include "MPU6050.h"

#define q30 1073741824.0f

static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1};

/**
 *  功能：IIC写一个字节
 *  入口参数：RegAddress，寄存器地址；data，数据
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    I2C_Start();               // 打开IIC通信
    I2C_Send_Byte(0xD0);       // 选中MPU6050，最后一位为0为写操作
    I2C_Wait_Ack();            // 从机发送应答，主机要接收应答
    I2C_Send_Byte(RegAddress); // 主机继续发送要写的寄存器地址
    I2C_Wait_Ack();            // 主机接收从机的应答
    I2C_Send_Byte(Data);       // 主机发送要写的数据
    I2C_Wait_Ack();            // 主机接收从机的应答
    I2C_Stop();                // 停止IIC通信SDA与SCL都为高
}
/**
 *  功能：IIC读一个字节
 *  入口参数：RegAddress，寄存器地址
 *  返回值：数据
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress) // 读指定寄存器
{
    uint8_t Data; // 接收读出数据的变量
    I2C_Start();
    I2C_Send_Byte(0xD1);
    I2C_Wait_Ack();
    I2C_Send_Byte(RegAddress);
    I2C_Wait_Ack(); // 前面几步确定地址
    I2C_Start();
    I2C_Send_Byte(0xD1);     // 对指定地址进行读
    I2C_Wait_Ack();          // 从机要回应这个指令
    Data = I2C_Read_Byte(0); // 从机把指定地址的数据通过SDA线发出来
    I2C_NAck();              // 主机不给应答，从机便会结束发送
    I2C_Stop();              // 结束通信
    return Data;
}
/**
 *  功能：IIC连续写
 *  入口参数：addr，外设地址；reg，寄存器地址；len，写入长度；buf，数据
 *  返回值：0，正常；1，错误
 */
uint8_t MPU6050_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    uint8_t i;
    I2C_Start();
    I2C_Send_Byte(addr | 0);
    if (I2C_Wait_Ack())
    {
        I2C_Stop();
        return 1;
    }
    I2C_Send_Byte(reg);
    I2C_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        I2C_Send_Byte(buf[i]);
        if (I2C_Wait_Ack())
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_Stop();
    return 0;
}
/**
 *  功能：IIC连续读
 *  入口参数：addr，外设地址；reg，寄存器地址；len，读取长度；buf，储存数据
 *  返回值：0,正常；1，错误
 */
uint8_t MPU6050_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    I2C_Start();
    I2C_Send_Byte(addr | 0);
    if (I2C_Wait_Ack())
    {
        I2C_Stop();
        return 1;
    }
    I2C_Send_Byte(reg);
    I2C_Wait_Ack();
    I2C_Start();
    I2C_Send_Byte(addr | 1);
    I2C_Wait_Ack();
    while (len)
    {
        if (len == 1)
            *buf = I2C_Read_Byte(0);
        else
            *buf = I2C_Read_Byte(1);
        len--;
        buf++;
    }
    I2C_Stop();
    return 0;
}

// MPU6050自测试
// 返回值:0,正常
//     其他,失败
uint8_t run_self_test(void)
{
    int result;
    // char test_packet[4] = {0};
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3)
    {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);

        //accel_sens = 0; // 不校准加速计
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        return 0;
    }
    else
        return 1;
}

/**
 * @brief       函数inv_orientation_matrix_to_scalar()的辅助函数
 * @param       row: 输入
 * @retval      输出
 */
static inline unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7; // error

    return b;
}

/**
 * @brief       将方向矩阵转换为标量表示，以供DMP使用
 * @param       mtx: 方向矩阵
 * @retval      标量表示的方向参数
 */
static inline unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

uint8_t MPU6050_Init(void)
{
    uint8_t res = 0;
    struct int_param_s int_param;  // 这个没什么用，就是为了能给他实参调用起来
    I2C_Init();                    // 初始化IIC总线
    if (mpu_init(&int_param) == 0) // 初始化MPU6050
    {
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL); // 设置所需要的传感器
        if (res)
            return 1;
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL); // 设置FIFO
        if (res)
            return 2;
        res = mpu_set_sample_rate(100); // 设置采样率
        if (res)
            return 3;
        res = dmp_load_motion_driver_firmware(); // 加载dmp固件
        if (res)
            return 4;
        res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)); // 设置陀螺仪方向
        if (res)
            return 5;
        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | // 设置dmp功能
                                 DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                                 DMP_FEATURE_GYRO_CAL);
        if (res)
            return 6;
        res = dmp_set_fifo_rate(100); // 设置DMP输出速率(最大不超过200Hz)
        if (res)
            return 7;
        // res = run_self_test(); // 自检
        // if (res)
        //     return 8;
        res = mpu_set_dmp_state(1); // 使能DMP
        if (res)
            return 9;
    }
    return 0;
}

// 得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
// pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
// roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
// yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
// 返回值:0,正常
//     其他,失败
uint8_t MPU6050_DMP_get_data(float *pitch, float *roll, float *yaw)
{
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    unsigned long sensor_timestamp;
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
        return 1;
    /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
     * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
     **/
    /*if (sensors & INV_XYZ_GYRO )
    send_packet(PACKET_TYPE_GYRO, gyro);
    if (sensors & INV_XYZ_ACCEL)
    send_packet(PACKET_TYPE_ACCEL, accel); */
    /* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
     * The orientation is set by the scalar passed to dmp_set_orientation during initialization.
     **/
    if (sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / q30; // q30格式转换为浮点数
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        // 计算得到俯仰角/横滚角/航向角
        *pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                    // pitch
        *roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;     // roll
        *yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; // yaw
    }
    else
        return 2;
    return 0;
}

void fake_delay_ms(unsigned long num_ms){};
void fake_get_ms(unsigned long *count){};
