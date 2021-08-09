'Messages' - simple client-server application

After running server one or two clients can connect to
it and automatically send random messages to each other.

When second client is unavailable the server 
writes received messages from first client into file.
When second client is connected server sends all saved
messages to this client and removes file.

Client takes one parameter (integer) which stands for
number of messages to send.

For compile and run 'Messages' open terminal in
'Messages' directory and type:
$chmod +x run.sh
$./run.sh

Default parameter in run.sh for first client is 9
messages and 5 for second client.

