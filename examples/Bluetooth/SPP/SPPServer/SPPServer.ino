/*
 Example sketch for the RFCOMM/SPP Server Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <SPPServer.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the class in two ways */
SPPServer SerialBT(&Btd); // This will set the name to the defaults: "Arduino" and the pin to "0000"
//SPPServer SerialBT(&Btd, "Lauszus's Arduino", "1234"); // You can also set the name and pin like so

boolean firstMessage = true;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nSPP Server Started"));
}

void loop() {
  Usb.Task(); // The SPP data is actually not send until this is called, one could call SerialBT.send() directly as well

  if (SerialBT.connected) {
    if (firstMessage) {
      firstMessage = false;
      SerialBT.println(F("Hello from Arduino SPP server")); // Send welcome message
    }
    while (Serial.available())
      SerialBT.write(Serial.read());
    while (SerialBT.available())
      Serial.write(SerialBT.read());
  } else
    firstMessage = true;
}