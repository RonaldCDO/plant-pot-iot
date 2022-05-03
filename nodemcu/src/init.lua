require('wifi')
net = require('net')

wifi.setmode(wifi.SOFTAP)
wifi.ap.config({
    ssid = "batata",
    pwd = "batata123"
})

wifi.ap.setip({
    ip="10.0.0.1",
    netmask="255.255.255.0",
    gateway="10.0.0.1"
})

server = net.createServer(net.TCP, 120)

function receiver(sck, data)
  if string.find(data, "LED ON")  then
   sck:send("\r\nLED ON")
  elseif string.find(data, "LED OFF")  then
   sck:send("\r\nLED OFF")
  elseif string.find(data, "EXIT")  then
   sck:close()
  else
   sck:send("\r\nCommand Not Found...!!!")
  end
end

if server then
  server:listen(80, function(conn)
  conn:on("receive", receiver)
  conn:send("Hello Client\r\n")
  conn:send("1. Send 'LED ON' command to ON LED\r\n")
  conn:send("2. Send 'LED OFF' command to OFF LED\r\n")
  conn:send("3. Send 'EXIT' command to Exit\r\n")
  end)
end