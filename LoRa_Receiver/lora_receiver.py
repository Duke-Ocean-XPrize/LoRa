#!/usr/bin/python

import rf95
#create rf95 object with CS0 and external interrupt on pin 25
lora=rf95.RF95(0,25)
p_received=0

if not lora.init(): #returns True if found
    print("RF95 not found")
    quit(1)
else:
    print("RF95 LoRa mode ok")

#set frequency, power and mode
lora.set_frequency(915.0)
lora.set_tx_power(23)

lora.set_modem_config(rf95.Bw125Cr45Sf128)



#wait until data is available

# Receive

while True:
    while not lora.available():
            pass
    p_received+=1
    data=lora.recv()
    try:
        data_str = ''.join(chr(x) for x in data)
        data_str = data_str[4:]
        tokens = data_str.split()
        id = int(tokens[0])
        lat = int(tokens[1])
        lon = int(tokens[2])
        age = int(tokens[3])
        sats = int(tokens[4])
        hdop = int(tokens[5])
        timestamp=tokens[6]
               
    except ValueError as e:
        print ("unable to parse: '%s'" % (data_str))
        print(e)

        #print(tokens)
    
    if(age==-1):
        print("invalid data: '%s'" % (data_str))
    else:
        print("Got data: '%s'" % (data_str))
        #print "id=%d lat=%d lon='%s' age=%d sats=%d hdop=%d timestamp=%d " % (id,lat,lon,age,sats,hdop,timestamp)
   
            
lora.set_mode_idle()



