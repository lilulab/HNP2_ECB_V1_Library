# HNP2_ECB_V1_Library

## Installation instruction.

### 1. Install USB driver for the UART-USB chip (CP2102), select right VPD Driver for your OS.
    https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx

### 2. Install Arduino IDE Version 1.0.4, (Notice that some newer version does not work with my board. Version 1.0.4 is the best one.)
    Extract the zip file below to local, and run the arduino.exe with right click -> Run as Administrator. (This will make sure the IDE can access hardware)

    http://arduino.googlecode.com/files/arduino-1.0.4-windows.zip

### 2. Find Arduino Sketchbook location and open this folder.
    1). Find Path using: Aruino 1.0.4 -> File ->Preference -> Sketchbook location.
    2). Open a File Brower to this Sketchbook location.
    3). Then go to the subfolder called "libraries" under the Sketchbook location.

### 3. Checkout newest project dependant libraries on Github.
    1). You will need the HNP2_ECB_V1_Library (For setup ECB board) and CwruStim_beta (For talk to Stim board).
    Put it under the <Sketchbook location> / libraries/ folder.
    Option 1. Use Git command line tool, if you have one installed on your computer, if not jump to option 2. 
    Option 2. Down load the zip file from Github page. (Mid right side of the page, locate button "Download as zip")

       https://github.com/lilulab/HNP2_ECB_V1_Library
       
       https://github.com/lilulab/CwruStim_beta

    Option 3. Email Lu, and will send the file via attachment.

    2). After Git clone, or extract the zip to the libraries folder. 
    Now you should have two folders named "HNP2_ECB_V1_Library" and "CwruStim_beta" in the libraries folder.

### 4. Restart Adurino IDE and check if two libraries are installed correctly.
    1). Turn off all the Adurino IDE windows, and reopen the Arudino.exe under arduino-1.0.4 folder.
    2). Go to File -> example. If you can see two folders called "HNP2_ECB_V1_Library" and "CwruStim_beta", then all set!

### 5. Open the example code.
    Go to File -> example -> HNP2_ECB_V1_Library -> example -> HNP2_ECB_V1_Demo_PercStim
    (Noticed that under example, code is read-only. You can simply go to File -> Save as, and save it to your <Sketchbook location> and edit it)
    
### 6. Connect ECB board hardware.
    1). Connect the ECB board to USB port.
    2). Open Device Manager. Check under Ports (COM &LPT), Find the COM port correspond to "Silicon Labs CP210x USB to UART Bridge".

### 7. Upload the code after setup board and port in Aduino IDE.
    1). Go to Tools -> board, select Mega 2560 or Mega ADK
    2). Go to Tools -> SerialPort, select the COM port you find from Step 6.
    3). Hit Upload button. And it should compile and flash the code to the ECB in few seconds.
