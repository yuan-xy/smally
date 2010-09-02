netsh interface ip set address name="本地连接" source=static addr=222.205.125.176 mask=255.255.255.0 gateway=222.205.125.1 gwmetric=1
netsh interface ip set dns name="本地连接" source=static addr=10.10.0.21

