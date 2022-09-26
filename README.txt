En este archivo de texto se encuentran los pasos a seguir para el correcto uso del programa de transferencia de datos.
¡Es IMPORTANTE mencionar que para el uso de este programa se necesita tener instalado cmake¡, ya que esta destinado a ser usado junto a este.
1-Se debera ir a la carpeta de servidor o cliente, dependiendo de que codigo que este tratando de utilizar.
NOTA: en caso de querer utilizar el cliente para transferir archivos, se debera crea o mover el archivo que se desea transferir a la carpeta de cliente.
2-Dentro de la carpeta se debera ejecutar la linea de codigo $ cmake CMake
3-Luego se debe ejecutar la linea de codigo $ make
4-En caso de estar utilizando el codigo de server se debe utilizar la linea de codigo $ ./server  y esperar a la respuesta del cliente.
5-En caso de estar utilizando el codigo de cliente, primero se debera utlizar la linea de codigo $ ./cliente  y luego ingresar el nombre del archivo que se quiere transferir.

