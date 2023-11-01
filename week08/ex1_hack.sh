pid=$(cat /tmp/ex1.pid)
password=$(echo sudo xxd -c 32 -s sudo gdb -p $pid /proc/$pid/mem | grep 'pass:' | head -n 1 | awk '{print $2}')
echo "Password: $password"
kill -9 $pid