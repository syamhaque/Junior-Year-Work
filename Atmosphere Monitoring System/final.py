#!/usr/bin/python
# Final Project #

###import python modules###
import threading
import RPi.GPIO as GPIO
import time
import datetime
import requests # used to make JSON requests
import pprint  # used to print JSON data in human readable format
import Freenove_DHT as DHT

from PCF8574 import PCF8574_GPIO
from Adafruit_LCD1602 import Adafruit_CharLCD



###Global data and logic variables###
LCD_occupied = False            #Some info displayed should not be interrupted. Set this to true if that is the case. Use mutex when changing.
hr_last_irrigated = 0           #hour of latest irrigation
hour_available = 0                              #hour of latest valid CIMIS  data
CIMIS_updated = False                   #CIMIS has data for the requested hour.
start = True
#24 by 6 2D array. Contains 6 data values (see index enum) for each of the 24 hours in a day
Data = [[0.00001] * 6 for i in range(24)]

###Global constants for indexes of Data 2D array###
# use these constants instead of direct values. e.g. Data[0][LTEMP] = 75 # 
LTEMP = 0       #Local Temperature
LHUM = 1        #Local Humidity
LET = 2         #Local ET
STEMP = 3       #Station Temperature
SHUM = 4        #Station Humidity
SET = 5         #Station ET 

###Global constants for pin values###
Relay = 11      #Relay that turns on motor (sprinklers)
DHT11 = 13      #Temperature and Humidity Sensor
PIR = 15        #Motion Sensor

###Set GPIO pins and setup###
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

GPIO.setup(PIR, GPIO.IN)                #Motion Sensor
GPIO.setup(Relay, GPIO.OUT)             #Relay that turns on motor

###Set up LCD
PCF8574_address = 0x27  # I2C address of the PCF8574 chip.
PCF8574A_address = 0x3F  # I2C address of the PCF8574A chip.
# Create PCF8574 GPIO adapter.
try:
        mcp = PCF8574_GPIO(PCF8574_address)
except:
        try:
                mcp = PCF8574_GPIO(PCF8574A_address)
        except:
                print ('I2C Address Error !')
                exit(1)
# Create LCD, passing in MCP GPIO adapter.
lcd = Adafruit_CharLCD(pin_rs=0, pin_e=2, pins_db=[4,5,6,7], GPIO=mcp)


#######Thread functions to implement#######

###Temp and Humidity Sensor###
##Senses and display temp and humidity every minute. Must be synced with real time. Use datetime module.##
def temp_hum_thread():
        global Data
        global LCD_occupied
        global hr_last_irrigated
        global start
        dht = DHT.DHT(DHT11)   #create a DHT class object
        temp = 0.0
        hum = 0.0
        count = 0
        hours_passed = 0
        now = datetime.datetime.today()
        #this makes it so you can start at any time
        if start:
                hr_last_irrigated = now.hour   #this is line that should be used. UNCOMMENT
                start = False
                print ('Starting Hour: ',hr_last_irrigated)
        next_hour = datetime.datetime(now.year, now.month, now.day, (now.hour + 1) % 24, 0, 0)
        if(next_hour.hour == 0):
                next_hour += datetime.timedelta(days = 1)
        
        while(hours_passed < 24):
                #read data from DHT. If error, repeat read until no errors
                check = dht.readDHT11()
                while (not(check is dht.DHTLIB_OK) or dht.humidity > 100): #or dht.humidity > 100 (<---- inserting this in while loop may fix first garbage value error
                        check = dht.readDHT11()
                if(LCD_occupied == False):      #if LCD is not occupied, display data. Else, move on without displaying
                        LCD_occupied = True
                        lcd_display(dht.temperature, dht.humidity)
                        LCD_occupied = False
                #print("Humidity : %.2f, \t Temperature : %.2f \n"%(dht.humidity,dht.temperature)) #just print for now
                #Keep a running sum of temp and humidity for an hour. Average it at the hour.
                temp += dht.temperature
                hum += dht.humidity
                count += 1
                #If an hour has passed (synced with real time) average the temp and humidity values and store in Data array
                now = datetime.datetime.today()
                if(now >= next_hour): 
                        hours_passed += 1
                        Data[now.hour][LTEMP] = temp / count #IMPORTANT: Depending on if hr ranges from (0 - 23) or (1 - 24) change how you access array.
                        Data[now.hour][LHUM] = hum / count 
                        #display(avg temp and hum)
                        print("Average Humidity : %.2f, \t Average Temperature : %.2f \n"%(Data[now.hour][LHUM],Data[now.hour][LTEMP]))
                        lcd_display (Data[now.hour][LTEMP],Data[now.hour][LHUM])
                        #update value of next_hour
                        next_hour = datetime.datetime(now.year, now.month, now.day, (now.hour + 1) % 24, 0, 0)
                        if(next_hour.hour == 0):
                                next_hour += datetime.timedelta(days = 1)
                        #Call the thread that gathers CIMIS data 
                        t = threading.Thread(target=get_CIMIS_data)
                        t.start()
                        #reset all 3 values to 0
                        temp = 0.0
                        hum = 0.0
                        count = 0
                        
                #sleep until next real time minute
                next_minute = datetime.datetime(now.year, now.month, now.day, now.hour, (now.minute + 1) % 60, 0)
                if(next_minute.minute == 0):
                        next_minute += datetime.timedelta(hours = 1)
                time.sleep((next_minute - now).seconds)
                
###Thread to access CIMIS###
#Access CIMIS data of specified hour
def get_CIMIS_data():
    global Data
    global LCD_occupied
    global hour_available
    # dict used to get the month number instead of the month name
    month = {'January': '01', 'February': '02', 'March': '03', 'April': '04', 'May': '05', 'June': '06',
             'July': '07', 'August': '08', 'September': '09', 'October': '10', 'November': '11', 'December': '12'}

    ## use datetime library to create a date of type string to be used in the JSON request
    m = month[datetime.datetime.now().strftime("%B")]
    d = datetime.datetime.now().strftime("%d")
    y = datetime.datetime.now().strftime("%Y")
    date = y + "-" + m + "-" + d

    ## Assemble the JSON request
    url = "http://et.water.ca.gov/api/data?"
    params = {"appKey" : "80b0f94e-c5e5-4734-8cc1-131e7c2f825e" , "dataItems" : "hly-air-tmp, hly-eto, hly-rel-hum" ,
              "targets" : "75" , "startDate" : date , "endDate" : date, "unitOfMeasure" : 'M'}
    headers =  {"accept" : "application/json"}

    ## Make the request and store it in 'json_data' variable
    try:
            r = requests.get(url, params = params, headers = headers)
            json_data = r.json()

            hour_index = 0
            for i in json_data['Data']['Providers'][0]['Records']:
                temp = i['HlyAirTmp']['Value']
                eto =  i['HlyEto']['Value']
                humidity = i['HlyRelHum']['Value']
                        
                        # if requested values are not available, fill the array with -1
                if eto is None or temp is None or humidity is None:
                        Data[hour_index][STEMP] = -1.0
                        Data[hour_index][SHUM] = -1.0
                        Data[hour_index][SET] = -1.0
                else:
                        Data[hour_index][STEMP] = float(temp)
                        Data[hour_index][SHUM] = float(humidity)
                        Data[hour_index][SET] = float(eto)
                        hour_available = hour_index #used to signal which latest hour contains valid data
                        #print('hr avialable', hour_available)
                #print('station temp' + str(temp))
                #print ('local temp' + str(Data[hour_available][LTEMP]))
                hour_index = hour_index + 1
                #Calculate local ET and display if new data gathered
            now = datetime.datetime.today()
            LET_TOTAL = derate_ET()
            if CIMIS_updated:
                #Display and update hr_last_irrigated global variable
                if LCD_occupied == False:
                        lcd.clear()
                        LCD_occupied = True
                        lcd_display_rolling(LET_TOTAL)
                        LCD_occupied = False
                if hour_available >=22:
                        print('\n'.join([''.join(['{:4}'.format(item) for item in row]) for row in Data]))#DEBUG: prints Data array
                ##Calculate how many gallons of water you need to irrigate
                gallons = irrigation_amount(LET_TOTAL)
                ##Call irrigate_field(gallons) function 
                irrigate_field(gallons)
                #t2 = threading.Thread(target=irrigate_field, args=(gallons))
    except:
                print('Something wrong with pulling CIMIS data')


###Function to Irrigate Field###
#Infrared motion sensor will be used as GPIO.input() to stop sprinklers
def irrigate_field(gallons):
        global LCD_occupied
        ##Based on the gallons calculate how long you have to run the motor for. Remeber the system delivers 1020 gallons per hour
        irrigation_time = gallons/1020  #Get time to irrigate
        ##Convert decimal time to real time
        irr_hr = int(irrigation_time)   
        irr_min = irrigation_time%1*60  
        irr_sec = irr_min%1*60
        ##Get current real time
        curr_time = datetime.datetime.now().strftime('%H:%M:%S')
        (h, m, s) = curr_time.split(':')
        ##calculate real time to stop
        hrToStop = int(h) + irr_hr
        if hrToStop >= 24:
                hrToStop -= 24
        minToStop = int(m) + int(irr_min)
        if minToStop >= 60:
                hrToStop += 1
                minToStop -= 60
        secToStop = int(s) + int(irr_sec)
        if secToStop >= 60:
                minToStop += 1
                secToStop -= 60
    ##Irrigate for specified time, but check for input from the infrared motion sensor.
        counter = 0
        while True:
##      keep calculating real time to see if irrigation time is up
                curr_time = datetime.datetime.now().strftime('%H:%M:%S')
                (h, m, s) = curr_time.split(':')
                if int(h) == hrToStop and int(m) == minToStop and int(s) == secToStop:
                        break
##      if motion detected start counting seconds
                if GPIO.input(PIR) == GPIO.HIGH and counter < 60:
                        GPIO.output(Relay, GPIO.LOW)    #Stop relay
                        if LCD_occupied == False:
                                LCD_occupied = True
                                lcd_display_irrigation("OFF")
                                LCD_occupied = False
                        lcd.clear()
                        while GPIO.input(PIR) == GPIO.HIGH and counter < 60:
                #Keep getting real amount of time stopped
                                counter += 1
                                curr_time = datetime.datetime.now().strftime('%H:%M:%S')
                                (hr, mi, se) = curr_time.split(':')
##              get time stopped and add to the real time to stop  
                                if hr >= h: 
                                        hrToStop += int(hr) - int(h)
                                        if hrToStop >= 24:
                                                hrToStop -= 24
                                else:
                                        hrToStop = 0
                                if mi >= m:
                                        minToStop += int(mi) - int(m)
                                        if minToStop >= 60:
                                                hrToStop += 1
                                                minToStop -= 60
                                else:
                                        minToStop = 0
                                if se >= s:
                                        secToStop += int(se) - int(s)
                                        if secToStop >= 60:
                                                minToStop += 1
                                                secToStop -= 60
                                else:
                                        secToStop = 0
                                time.sleep(1)
                #if counter = 60, one minute has passed, start relay
##      if no motion, relay goes on
                else:
                        GPIO.output(Relay, GPIO.HIGH)
                        time.sleep(1)
    #Remeber to display that sprinklers have turned off/on.

###Function for Derating ET
def derate_ET():
        global Data
        global hr_last_irrigated
        global CIMIS_updated
        global hour_available
        CIMIS_updated = False
        i = hour_available - hr_last_irrigated
        #Check if CIMIS has updated
        if Data[hr_last_irrigated + 1][SET] >= 0 :  #if [hour last irrigated + 1] station ET is negative, CIMIS not updated yet
                CIMIS_updated = True
        ET_TOTAL = 0
        k = 0
        #if cimis updated, then get local et by derating each station et with local values 
        #then sum up all local ets from hour alst updated to hour available in cimis
        if CIMIS_updated :
                while (k+1) <= i:
                        #print ('k',k)
                        DERATED_TEMP = Data[hr_last_irrigated + k + 1][LTEMP] / Data[hr_last_irrigated + k + 1][STEMP]
                        DERATED_HUM = Data[hr_last_irrigated + k + 1][SHUM] / Data[hr_last_irrigated + k + 1][LHUM]
                        local_et = Data[hr_last_irrigated + k + 1][SET] * DERATED_TEMP * DERATED_HUM
                        Data[hr_last_irrigated + k + 1][LET] = local_et
                        ET_TOTAL += local_et
                        k=k+1
                        #print ('hr:',k,'et:',local_et,'set:',Data[hr_last_irrigated+k+1][SET])
                #hr_last_irrigated = hour_available
                #print ('hr last irrigated',hr_last_irrigated)
        return ET_TOTAL

###Function for calculating amount of water to use/ time
def irrigation_amount(ET_TOTAL):
        PF = 1.0 #plant factor, currently set to grass
        SF = 200 #square feet
        IE = 0.8 #irrigation efficiency
        return (ET_TOTAL * PF * SF * 0.62 * IE)
        

###Function to Display###
 
def get_time_now():     # get system time
        return datetime.datetime.now().strftime('    %H:%M:%S')

def lcd_display(temp, hum):
        mcp.output(3,1)     # turn on LCD backlight
        lcd.begin(16,2)     # set number of LCD lines and columns
    
        #lcd.clear()
        lcd.clear()
        lcd.setCursor(0,0)  # set cursor position
        lcd.message(( 'loc hum: %.2f' %hum  +'\n'))# )   # display the current humid
        lcd.message( 'loc temp: %.2f'  %temp )# display current temp
        time.sleep(5) 

def lcd_display_irrigation (mode):
        mcp.output(3,1)     # turn on LCD backlight
        lcd.begin(16,2)     # set number of LCD lines and columns
    
        #lcd.clear()
        lcd.clear()
        lcd.setCursor(0,0)  # set cursor positiom
        lcd.message(( 'Irrigation \n'))# )   # display the current irrigation mode
        lcd.message( 'is currently '+  mode )# display mode
        time.sleep(2)

        
        

def lcd_display_rolling(LET_TOTAL):
        global Data
        global LCD_occupied
        global hr_last_irrigated
        global hour_available
        LCD_occupied = True
        hours = hour_available - hr_last_irrigated
        SET_TOTAL = 0
        #this finds the total station ET since last time irrigated
        for i in range(hours):
                        SET_TOTAL += Data[hr_last_irrigated + i + 1][SET]
        hr_last_irrigated = hour_available #sets new hr_last_irrigated
        CIMIS_water_usage = irrigation_amount(SET_TOTAL)
        LOCAL_water_usage = irrigation_amount(LET_TOTAL)

        #sets string parameters 
        CIMIS_TEMP = 'CIMIS Temperature: %.2f'  %Data[hr_last_irrigated][STEMP]
        CIMIS_HUM  = 'CIMIS Humidity: %.2f' %(Data[hr_last_irrigated][SHUM])
        CIMIS_ET = 'CIMIS ET: %.2f' %(SET_TOTAL)
        CIMIS_WATER = 'CIMIS Water Usage: %.2f' %(CIMIS_water_usage)
        LOCAL_TEMP = 'LOCAL Temperature: %.2f' %(Data[hr_last_irrigated][LTEMP])
        LOCAL_HUM  = 'LOCAL Humidity: %.2f' %(Data[hr_last_irrigated][LHUM])
        LOCAL_ET = 'LOCAL ET: %.2f' %(LET_TOTAL)
        LOCAL_WATER = 'LOCAL Water Usage: %.2f' %(LOCAL_water_usage)
        
        TOP_MSG = ( CIMIS_TEMP +' '+ CIMIS_HUM +' '+ CIMIS_ET +' '+ CIMIS_WATER )
        BOT_MSG = ( LOCAL_TEMP +' '+ LOCAL_HUM +' '+ LOCAL_ET +' '+ LOCAL_WATER )
        lcd.clear()
        lcd.setCursor(0,0)  # set cursor position
        lcd.message( 'Current Time:\n' )# display current temp
        lcd.message( get_time_now () )# )   # display the current humid
        time.sleep(3)
        length = len(TOP_MSG)
        print ('Hour Irrigating:',hr_last_irrigated)
        print (TOP_MSG)  #for debugging
        print (BOT_MSG) #for debugging
        i = 0
        LCD_occupied = True
        lcd.clear()
        #this displays the rolling message
        while (i+16<length):
                mcp.output(3,1)
                lcd.begin(16,2)
                lcd.setCursor(0,0)
                lcd.message( TOP_MSG[i:i+16] +'\n')# this displays the spliced string, will fit lcd display of 16 width
                lcd.message( BOT_MSG[i:i+16] )#
                time.sleep(0.2)
                i=i+1
        hr_last_irrigated = hour_available
        LCD_occupied = False
        

def lcd_destroy():
        lcd.clear()  
        mcp.output(3,0)





###Main Function###
if __name__ == '__main__':
        print ('Program is starting ... ')
        try:
                GPIO.output(Relay, GPIO.LOW)
                temp_hum_thread()
        except KeyboardInterrupt:
                GPIO.output(Relay, GPIO.LOW)
                lcd_destroy()
                GPIO.cleanup()
                exit()  
