# What should we do？

1. we need realization the server
   1. listen a port on tcp that allow our client to connect,this connect will use manager all client and collection client metric.
   2. listen a tcp port and a kcp port which use proxy intranet data.
2. we need realization the client
   1. connect server manager port
   2. report client stat on time
   3. accept command from server and exec


