### latino-json
Librería dinámica para manejo de json en [latino](https://github.com/primitivorm/latino)

##### Prerequisitos
Tener instalado [latino](https://github.com/primitivorm/latino)

### Instalación
##### linux
```
git clone https://github.com/primitivorm/latino-json
cd latino-json
git submodule update --init --recursive
chmod +x instalar.sh
sudo bash instalar.sh
```

#### Dependecias
[jansson](https://github.com/akheron/jansson)

##### Uso de esta librería en código latino
```
//necesario para agregar la librería dinamica
incluir("json")

escribir("-----------------------------------------------------")
escribir("convertimos de una cadena json a un objeto de latino:")
escribir("-----------------------------------------------------")
cad = '{"ok":true,"result":[{"update_id":558904697, "message":{"message_id":4507,"from":{"id":189041244,"first_name":"Bruno Ric (K)","username":"Jarriz"},"chat":{"id":189041244,"first_name":"Bruno Ric (K)","username":"Jarriz","type":"private"},"date":1475275449,"text":"hola latinos!!!"}}]}'
d = json.decodificar(cad)
escribir(d)

escribir("-----------------------------------------------------")
escribir("\nconvertimos un diccionario de latino a cadena json:")
escribir("-----------------------------------------------------")
dlat = { "uno" : 1, "dos" : 2, "tres" : ["esta" , "es", "una", "lista"]}
cad = json.codificar(dlat)
escribir(cad)

```

##### Cualquier aportación o sugerencia es bienvenida
