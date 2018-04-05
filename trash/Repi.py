from __future__ import print_function
import RPi.GPIO as GPIO
import time
import spidev


RST_PIN = 21  # BCM 21 - wPi 29
CVA_PIN = 18  # BCM 18 - wPi 1
CVB_PIN = 23  # BCM 23 - wPi 4
CS_PIN = 16   # BCM 16 - wPi 27
BUSY_PIN = 20 # BCM 20 - wPi 28


def AD7606_IOSet():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(BUSY_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(RST_PIN, GPIO.OUT)
    GPIO.setup(CVA_PIN, GPIO.OUT)
    GPIO.setup(CVB_PIN, GPIO.OUT)
    GPIO.setup(CS_PIN, GPIO.OUT)

    GPIO.output(RST_PIN, False)
    GPIO.output(CVA_PIN, True)
    GPIO.output(CVB_PIN, True)
    GPIO.output(CS_PIN, True)


def AD7606_Reset():
    GPIO.output(RST_PIN, False)
    GPIO.output(RST_PIN, True)
    time.sleep(0.5)
    GPIO.output(RST_PIN, False)
    


def AD7606_Convst():
    GPIO.output(CVA_PIN, False)
    GPIO.output(CVB_PIN, False)
    time.sleep(0.001)
    GPIO.output(CVA_PIN, True)
    GPIO.output(CVB_PIN, True)


def isBusy():
    return GPIO.input(BUSY_PIN)


def main():
    bus=0
    device=0
    
    spi = spidev.SpiDev()
    
    spi.open(bus, device)
    spi.mode=2
    spi.max_speed_hz=200000

    AD7606_IOSet()
    print(spi.cshigh)

    AD7606_Reset()

    try:
        while True:
            while isBusy():
                pass
            #GPIO.output(CS_PIN, True)
            #spi.cshigh=True
            AD7606_Convst()
            while isBusy():
                pass
            #spi.cshigh=False
            GPIO.output(CS_PIN, False)
            #GPIO.output(CS_PIN, GPIO.LOW)
           
            data = spi.readbytes(6)
            GPIO.output(CS_PIN, True)
            #time.sleep(1)
            #spi.cshigh=True
            d = [0, 0 ,0 ]
            for i in range(3):
                temp=(data[2*i]*256+data[2*i+1])*5/32768
                if temp>5:
                    d[i]=temp-10
                else:
                    d[i]=temp
            for x in d:
                print(x, end=" ")
            print(" ")
            time.sleep(1)
            #GPIO.output(CS_PIN, True)
##            channel = 0
##            #data = spi.xfer2([1, (8+channel)<<4, 0])
##            #data = spi.xfer2([0x00])
##            for x in data:
##                print(x, end=" ")
##            print("")
    finally:
        print("Cleaning Up!")
        GPIO.cleanup()


if __name__ == '__main__':
    main()
