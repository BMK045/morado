from flask import Flask, jsonify, request
from flask.ext.pymongo import PyMongo
import datetime

app = Flask(__name__)

app.config['MONGO_DBNAME'] = 'datosard'
app.config['MONGO_URI'] = 'mongodb:127.0.0.1:27017/datosard'

mongo = PyMongo(app)
#Leer los datos de la bd
@app.route('/leer', methods=['GET'])
def leer_datos():
    framework = mongo.db.framework 

    output = []

    for q in framework.find():
        output.append({'hora' : q['hora'], 'estado' : q['estado']})

    return jsonify({'result' : output})
#Leer datos por nombre de dispositivo(incluido en json arduino)
@app.route('/framework/<nombre>', methods=['GET'])
def get_one_framework(nombre):
    framework = mongo.db.framework

    q = framework.find_one({'nombre' : nombre})

    if q:
        output = {'nombre' : q['nombre'], 'estado' : q['estado'], 'hora':q['hora']}
    else:
        output = 'No results found'

    return jsonify({'result' : output})
#insertar datos
@app.route('/framework', methods=['POST'])
def add_framework():
    framework = mongo.db.framework 
    #En esta parte se recaban los datos del arduino 
    nombre = request.json['nombre']
    estado = request.json['estado']
    #en esta parte se determina la hora
    chora = datetime.datetime.now()
    hora = chora.strftime('%H:%M:%S.%f')
    framework_id = framework.insert({'nombre' : nombre, 'estado' : estado, 'hora':hora})
    new_framework = framework.find_one({'_id' : framework_id})

    output = {'nombre' : new_framework['nombre'], 'estado' : new_framework['estado']}

    return jsonify({'result' : output})


if __name__ == '__main__':
    app.run(debug=True)