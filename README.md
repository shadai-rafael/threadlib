# multithreadedworkqueue

# Códigos ejemplo correspondientes a los proyectos que involucran
 - WorkQueue
 - Pthreads
 - Curl
 - Multiple Client handling

# Versión 2
Se escucha el puerto 8888 en el localhost, el server se encarga de crear childsockets para atender a cada usuario y otorgar un identificador específico, las respuestas a cada petición se acumulan a la workpool que son constantemente checadas por 4 threads.

# Pendientes
Encontrar límitantes de función select para establecer número máximo de clientes
