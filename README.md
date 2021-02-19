[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Z8Z734F5Y)
# Self Organising LEDs

[![Demo Video](https://img.youtube.com/vi/Ueim2Ko8VWo/0.jpg)](https://www.youtube.com/watch?v=Ueim2Ko8VWo)

This project uses an esp32 board to drive a string of WS2118 LEDs. It uses some basic image processing to detect where the LEDs are so that you can display geometric patterns even if the LEDs are not arranged in a grid.

If you have an espcam then you can just follow the instructions on the Setup section. On first boot the espcam will try and calibrate the LED positions.

Once calibrated these are stored in nvram. If you need to recalibrate just navigate to the web interface: `http://espcam.local` and hit the calibrate button.

If you do not have an ESP-CAM and want to use your phone for calibration then you will need to jump through some hoops.

You cannot access the phone's camera unless you are using `https` - currently the webserver that I am using only supports `http`.

To get an `https` connection I would suggest using `ngrok` - https://ngrok.com/.

You can create an `https` tunnel on your computer to the esp32 device. I would recommend using the ip address.

Do something like:

```
ping espcam.local
```

And take a note of the ip-address. Then do:

```
ngrok http IP_ADDRESS
```

This will create an https endpoint in the cloud that you can use to access the esp32 securely.

You will need a steady hand or a tripod as the calibration takes about 20-30 seconds depending on how many LEDs you have.

# Setup

You will need python3, node and yarn installed.

Create a virtual environment, activate it and install the requirements using:

```
python3 -m venv venv
source ./venv/bin/activate
pip install -r requirements.txt
```

Create a file `WifiCredentials.h` in the `firmware/src` and populate it with your wifi SSID and PASSWORD:

```
#define SSID "<SSID>"
#define PASSWORD "<PASSWORD>"
```

In `main.cpp` update the defines for the number of LEDs you have and if you have an ESP-CAM or a normal ESP32 board.

```
#define LED_COUNT 100
#define CAMERA_ENABLED 1
```

You can now use the `Makefile` to build the firmware, frontend and upload them to your device.

```
make upload
```
