#commande a lancer pour creer myos.bin
docker run --rm -v "$(pwd)":/root/env -w /root/env debian:latest sh -c "chmod +x build.sh && ./build.sh"
#lancer l'os
qemu-system-i386 -kernel myos.bin