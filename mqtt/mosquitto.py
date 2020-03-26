import paho.mqtt.client as mqtt
from pymongo import MongoClient
import pymongo
import time
import datetime


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("casa")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	print(msg.topic+" "+str(msg.payload))
	#pymongo para la bd 
	myclient = pymongo.MongoClient("mongodb://localhost:27017/")
	mydb = myclient["mosco"]
	mycol = mydb["mosquitto"]
	#fecha = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")
	fecha = datetime.datetime.now()
	fecha_string = fecha.strftime('%Y-%m-%d')
	tiempo_strin = fecha.strftime('%H:%M:%S.%f')
	mydict = {
		'status':msg.payload.decode('ascii'),
		'date':fecha_string,
		'time':tiempo_strin
	}
	print(mydict, "diccionario")
	x = mycol.insert_one(mydict)
	print(x,"respuesta")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)
    
#mongodb_db = 'foco'





# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
