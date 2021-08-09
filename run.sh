cd Messages/Server
make
gnome-terminal -- ./server

cd ../Client
make

gnome-terminal -- ./client 9
gnome-terminal -- ./client 5

