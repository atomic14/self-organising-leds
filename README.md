# Setup

You will need python3, node and yarn installed.

Create a virtual environment, activate it and install the requirements using:

```
python3 -m venv venv
source ./venv/bin/activate
pip install -r requirements.txt
```

You can now use the `Makefile` to build the firmware, frontend and upload them to your device.

```
make upload
```
