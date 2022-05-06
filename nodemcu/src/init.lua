wifi = require('wifi')
net = require('net')
-- spi = require('spi')

--Wifi Setup
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

if wifi.ap.getip() ~= nil then
  print(wifi.ap.getip())
end

--SPI Setup
-- Mode: master
-- Data: 8 bits
-- CLOCK: 1MHz
-- HalfDuplex (3 pin SPI)
-- Ports: CLK -> GPIO14, MOSI -> GPIO13, MISO -> GPIO12
spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_HIGH, spi.DATABITS_8, 400, spi.FULLDUPLEX)
-- we won't be using the HSPI/CS line, so disable it again
gpio.mode(8, gpio.INPUT, gpio.PULLUP)

--TCP server Setup
server = net.createServer(net.TCP, 120)

function receiver(sck, data)
  if string.find(data, "MEDIR")  then
   spi.send(1, 'M')
   local temperature = spi.recv(1, 1)
   sck:send("Temperatura:")
   sck:send(tostring(temperature))
   local luminosity = spi.recv(1, 1)
   sck:send("Luminosidade:")
   sck:send(tostring(luminosity))
   local umidity = spi.recv(1, 1)
   sck:send("Umidade:")
   sck:send(tostring(umidity))
  elseif string.find(data, "EXIT")  then
   sck:close()
  else
   sck:send("\r\nCommand Not Found...!!!")
  end
end

if server then
  server:listen(80, function(conn)
  conn:on("receive", receiver)
  conn:send("Ola!\r\n")
  conn:send("1. Envie \"MEDIR\" para receber as medidas de temperatura, luminosidade e umidade do solo\r\n")
  end)
end