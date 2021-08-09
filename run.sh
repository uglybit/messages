cd Messages/Server
make
gnome-terminal -- ./server

cd ../Client
make

gnome-terminal -- ./client 6
gnome-terminal -- ./client 4

