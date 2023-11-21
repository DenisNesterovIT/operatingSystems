dd if=/dev/zero of=lofs.img bs=50M count=1 #Using the dd command, we create a logs.img file filled with zeros (copy zeros from the /dev/zero file). We specify the number of bytes to be written at a time (50 MiB) and the number of times these bytes are written (1 time).
device=$(losetup -f) #We find the first available device node and write it to the device variable.
sudo losetup $device lofs.img #We create a loop device.
sudo mkfs -t ext4 $device #We create a file system with loop device (in device variable). As an parameter -t we specify the ext4 file system type.
mkdir ./lofsdisk #Create empty directory (mount point) lofsdisk.
sudo mount -t ext4 $device ./lofsdisk #Mount filesystem.
cd lofsdisk #Open lofsdisk directory.
sudo nano file1 #Create and open file1.txt to write first name.
sudo nano file2 #Create and open file2.txt to write last name.
#Defenition of function get_libs for finding all shared libraries of binary file.
get_libs() {     
    sudo ldd "$1" | awk 'BEGIN{ORS=" "}$1~/^\/lib/{print $1}$2~/=>/ && $3~/^\/lib/{print $3}' 
}
#Making directories for linraries
sudo mkdir ./lofsdisk/bin/bash
sudo mkdir ./lofsdisk/bin/ls
sudo mkdir ./lofsdisk/bin/cat
sudo mkdir ./lofsdisk/bin/echo
#Using function get_libs we get shared libraries of commands bash, cat, echo, ls and add them with their shared libraries to the LOFS.
for i in bash cat echo ls; do
    for j in $(get_libs "/bin/$i"); do
        sudo mkdir -p "./lofsdisk$(dirname $j)"
        sudo cp "$j" "./lofsdisk$j"
    done
    sudo cp "$i" "./lofsdisk$i"
done
#Compile ex1.c file
sudo gcc -o ex1 ex1.c
#Change the root directory of the process to the mount point of the created LOFS and run the program ex1.
sudo chroot ./lofsdisk ./ex1 >> ex1.txt
#Run ex1 without changing root
echo "Run without changing root" >> ex1.txt
./ex1 >> ex1.txt


