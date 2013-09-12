ROW-SCP
=======

ROW-SCP stands for roaming + wither + scp. ROW-SCP allows users to roam while transfering a file and uses the Wither congestion protocol developed by João Batalha and Ameesh Goyal. 

Background
----------

Nowadays, (apart from FTP) the standard program that is used to transfer files between two computers in a command line environment is [Secure Copy](http://en.wikipedia.org/wiki/Secure_copy) (scp). scp is based on the BSD RCP protocol, and it is built on top of [SSH](http://en.wikipedia.org/wiki/Secure_Shell). The way scp works is using the client and server model. In order to start a file transfer, it starts the scp process locally (in either source or sink mode, depending on whether the client is receiving or sending a file), then it initiates an SSH connection to the remote host, and starts the scp process in the remote host. Secure Copy uses [TCP](http://en.wikipedia.org/wiki/Transmission_Control_Protocol) to transfer files.
TCP was originally designed with the assumption that packet loss could be interpreted as a sign of congestion. This assumption was fine in its early years. Nevertheless, even though congestion can certainly be a cause of packet loss, it is not the only one. Modern networks often have high bandwidth links with non negligible packet loss rates due to cabling, glitches, etc. When TCP sees a packet loss in one of these links and it interprets it as congestion, its exponential backoff algorithm ends up causing the transfer throughput to collapse even though there is plenty of bandwidth in the left to be used.
This is one of the reasons why we decided to stay away of TCP. Another reason is that for file transfer we do not need TCP’s guarantee of in order delivery. UDP provides us with a lot of flexibility. For instance, we can deal with congestion control ourselves and we can set the size of UDP packets. In addition, UDP sockets are connectionless which is essential for the way we implemented roaming.

