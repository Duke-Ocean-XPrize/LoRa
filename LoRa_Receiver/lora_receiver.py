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



#wait untill data is available

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
            sats = int(tokens[0])
            hdop = int(tokens[1])
            timestamp = tokens[2]
            lat = int(tokens[3])
            lon = int(tokens[4])
            age = int(tokens[5])
            packet = int(tokens[6])
        except ValueError,e:
            print "unable to parse: '%s'" % data_str
            print e

        #print(tokens)
	
	if(age==-1):
            print("invalid data: '%s'" % (data_str))
        else:
            print "Got data: '%s'" % data_str
            print "sats=%d hdop=%d ts='%s' lat=%d lon=%d age=%d packet=%d" % (sats,hdop,timestamp,lat,lon,age,packet)
        print "Pkts: %s/%d = %.1f%%" % (p_received,packet,100.0*p_received/max(1,packet))
            
lora.set_mode_idle()



