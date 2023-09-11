cd
mkdir root
date
sleep 3
ls -t / > ~/root/root.txt
date
sleep 3
mkdir home
date
sleep 3
ls -t ~ > ~/home/home.txt
date
sleep 3
ls ~/root
cd root
cat root.txt
cd
ls ~/home
cd home
cat home.txt
