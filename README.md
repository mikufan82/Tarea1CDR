Como usar

Para hacer uso de este codigo en nuestro caso utilizamos maquinas virtuales, las imagenes para hacer las conexiones fueron las siguientes:

https://drive.google.com/file/d/1dM2Exqbew0XuQmViBpn4dtXVrap_n7Se/view?usp=sharing

Credenciales Cyberops 
Usuario analyst
Contraseña cyberops

https://drive.google.com/file/d/1rrFSgwqERiEODpRGujBpECz95H8AzVxv/view?usp=sharing

Credenciales kali linux
Usuario kali
Contraseña kali

Estas imagenes se utilizaron para simular una red NAT configurada a traves de VirtualBox.

Dentro de VirtualBox para configurar una nueva red NAT hay que ir a archivo, luego a parametros, dentro de la seccion de red podemos crear una nueva red NAT.

Luego para cada maquina virtual tenemos que configurar para que estas tengan adaptadores de red NAT, para esto seguimos unos pasos similares a los anteriores, vamos a la maquina virtual, seleccionamos configuracion, luego red,
en este apartado configuramos las opciones, el campo "Conectado a:" debe ir en Red Nat, y en el campo "Nombre" sera la red NAT que creamos anteriormente. Deberia verse similar a esta imagen:

![image](https://github.com/mikufan82/Tarea1CDR/assets/147022362/d05ae929-180d-4142-947f-4836b7978b94)

Despues de hacer esto con ambas maquinas las iniciamos.

Iniciar Servidor

Para este caso solo se debe compilar el Makefile que esta adjunto en el github. Luego de esto en la terminal usar ./server (port) en el cual se puede terminar el uso con Ctrl + C

Iniciar Cliente

Para que el cliente pueda iniciar en un servidor debe dirigirse a la terminal y usar el comando: telnet (ip del servidor) (port).

Como ejemplo nosotros en el servidor iniciamos en el puerto 5555, por tanto el cliente se debe conectar como 

telnet 10.0.2.15 5555

despues de este iniciamos el juego y se le pide al jugador ingresar columnas.
