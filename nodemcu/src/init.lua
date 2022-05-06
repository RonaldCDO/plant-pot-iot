wifi = require('wifi')
net = require('net')

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
-- Ports: CLK -> GPIO14, MOSI -> GPIO13, MISO -> GPIO12
spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_HIGH, spi.DATABITS_8, 400, spi.FULLDUPLEX)
-- we won't be using the HSPI/CS line, so disable it again
gpio.mode(8, gpio.INPUT, gpio.PULLUP)

--TCP server Setup
server = net.createServer(net.TCP, 120)

function receiver(sck, data)
  sck:send("\r\nCOMANDO: ")
  sck:send(data)
  sck:send("\r\n")
  if string.find(data, "MEDIR")  then
   spi.send(1, 'M')
   -- Inicia comunicação
   spi.recv(1, 1)
   -- Receber os dados
   local temperature = spi.recv(1, 1)
   sck:send("Temperatura:\r\n")
   sck:send(string.byte(temperature))
   sck:send("\r\n")
   local luminosity = spi.recv(1, 1)
   sck:send("Luminosidade:\r\n")
   sck:send(string.byte(luminosity))
   sck:send("\r\n")
   local umidity = spi.recv(1, 1)
   sck:send("Umidade:\r\n")
   sck:send(string.byte(umidity))
   sck:send("\r\n")
  elseif string.find(data, "IRRIGAR")  then
   spi.send(1, 'I')
  elseif string.find(data, "SAIR")  then
   sck:close()
  else
   sck:send("\r\nCOMANDO DESCONHECIDO!")
  end
end

if server then
  server:listen(80, function(conn)
  conn:on("receive", receiver)
  conn:send("Ola!\r\n")
  conn:send("1. Envie \"MEDIR\" para receber as medidas de temperatura, luminosidade e umidade do solo\r\n")
  conn:send("2. Envie \"IRRIGAR\" para irrigar a planta agora\r\n")
  conn:send("3. Envie \"SAIR\" para finalizar a sessão\r\n")
  end)
end