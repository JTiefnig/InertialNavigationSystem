


class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        start();
    };
};

// 
Quaternion q;

// 
MPU6050 mpu(I2C(PB_4,PA_7));

// just for Debugging reasons
DigitalOut myled(LED1);

Timer t;


