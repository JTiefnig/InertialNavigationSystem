#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include <vector>

using namespace std;

#include "Counter.h"
#include "MPU6050/MPU6050.h"
#include "Quaternion.h"


#include "SerialProtocol.h"
#include "Quaternion.h"

#include <math.h>

#include "myInit.h"

#define PI 3.14159265358979f

#define RADtoDEG 180/PI
#define DEGtoRAD PI/180

Ticker updater;

DigitalIn setToZero(PA_12);//D2

double dt =0;

void updateFunc()
{
    if(myled.read())
    myled = 0; 
    else
    myled = 1; 
}



int main() 
{
    // Initalize Serial Port
    SerialProtocol serialPc(USBTX, USBRX);
    
    // Oled Display // shows splash screen during calibration...
    I2CPreInit gI2C(PB_7,PB_6);
    Adafruit_SSD1306_I2c gOled2(gI2C,PB_5, 0x78, 64, 128);
    
    // Initialize MPU
    mpu.init();
    // Calibrate Sensor - Dont move senosr on startup!!
    mpu.Calibrate();

    gOled2.clearDisplay();
    gOled2.setTextSize(1);

    int counter =0;

    updater.attach(&updateFunc, 0.02);

    t.start();

    while(1) {
        
        
        

        // Possibility to Zero the Roation
        if(setToZero)
            q = Quaternion();
            

        //Read in Gyro Data via library function
        DataVector read = mpu.GetGyroData();

        // Normalize values accroding to datasheet
        // get the yaw rate in the local sensor frame
        double dalpha = read.X *250.0/ 32768.0;
        double dbeta = read.Y  *250.0/ 32768.0;
        double dgamma = read.Z *250.0/ 32768.0;

        // to get the timeincremet right
        dt = t.read();
        t.reset(); // timer t zurücksetzen

        // create a Quaternion that represenst 
        Quaternion D_Q = Quaternion::from_euler_rotation_approx(dalpha*DEGtoRAD, dbeta*DEGtoRAD, dgamma*DEGtoRAD);
        
        // Nummeric Integration - time increment
        q += (q*D_Q)*dt;
        // always normalize
        q = q.normalize();

        
        
        

        if(counter % 5 == 0)
        {
            // Get Euler angels from Quaternion
            EulerAngles AA = q.ToEulerAngles();
            // Clear Display 
            gOled2.clearDisplay();
            gOled2.setTextCursor(0, 2);
            gOled2.printf("Psi:    %.2f \n\r" , AA.yaw*RADtoDEG);
            gOled2.printf("Theta:  %.2f \n\r" , AA.pitch*RADtoDEG);
            gOled2.printf("Phi:    %.2f \n\r" , AA.roll*RADtoDEG);
            gOled2.printf("%u \n\r" , counter);

            gOled2.printf("dt: %f \n\r" , dt);


            // very basic compass animation for yaw
            int offx = 100, offy = 32;
            float r= 10;
            float a = AA.yaw;
            float c = cos(a);
            float s = sin(a);

            gOled2.drawLine(offx+r*s, offy+c*r, offx-s*r, offy-c*r, 1);
            gOled2.drawLine(offx+r*s+c*3, offy+c*r-s*3, offx+s*r-c*3, offy+c*r+s*3, 1);

            gOled2.display();

            // Send Data via Serial Port --- Sending Unit Quaternion directly
            serialPc.SendDouble(1, q.a);
            serialPc.SendDouble(2, q.b);
            serialPc.SendDouble(3, q.c);
            serialPc.SendDouble(4, q.d);
            
        }
        // Print to Oled Screen

        

        // increment
        counter++;
            

        // This calculation will lead to gimbal lock! 
        // Alternative calculation  (f = PI / 180°) + timeintegration Euler explicit (phi += dphi* dt )
        //double f = 3.14159265/180.0d;
        //phi += (dbeta*sin(phi*f)+dgamma*cos(phi*f))/cos(theta*f)*dt;
        //theta += (dbeta*cos(phi*f)-dgamma*sin(phi*f))*dt;
        //psi += (dalpha+(dbeta*sin(phi*f)+dgamma*cos(phi*f))*tan(theta*f))*dt;

    }
}
