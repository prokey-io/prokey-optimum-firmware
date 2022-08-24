# Prokey Pptimum Firmware
## What is Prokey Optimum?
The Prokey Optimum is a secure, easy-to-use cryptocurrency hardware wallet that protects your crypto assets from online and offline attacks, while also supporting a wide range of cryptocurrencies without installing any application, letting you enjoy an all-in-one web-based wallet.
## Official websites
- [Prokey website](Prokey.io)
- [Shop](https://prokey.io/prokey-optimum)
- [Support](https://support.prokey.io)
- [Blog](https://prokey.io/blog)
## How to build (Ubuntu)
### Prerequisites
- Download [GNU ARM Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads) , unzip and add to PATH
```
wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
mkdir gcc-arm
tar -xf ./gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz -C ./gcc-arm/
mv ~/gcc-arm/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi ./gcc-arm/gcc-arm
nano .profile
```

Add this line to end of the file and save:
```
export PATH=$PATH:$HOME/gcc-arm/gcc-arm/bin
```

- Make sure you have installed Python3
```
python --version
```
If you get "Command 'python' not found" but you have python3 installed, run following command to make python3 as default:
```
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 10
```

- Install python3-pip
```
sudo apt install python3-pip
```

- Install [pipenv](https://pipenv.readthedocs.io/en/latest/install/)
```
pip3 install --user pipenv
```

- Install git
```
sudo apt install git
```

- Install protobuf [Protobuf](https://github.com/protocolbuffers/protobuf/releases/download/v3.19.4/protoc-3.19.4-linux-x86_64.zip)
```
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.19.4/protoc-3.19.4-linux-x86_64.zip
mkdir protoc
unzip protoc-3.19.4-linux-x86_64.zip -d protoc
nano .profile
```

Add following line to the end of file and save the changes
```
export PATH=$PATH:$HOME/protoc/bin
```
- Logout and login again or reboot the system

### Get the code and compile the code
- Get the code from github repository
```
git clone https://github.com/prokey-io/prokey-optimum-firmware.git
cd prokey-optimum-firmware
```

- Install environment
```
pipenv install
```

- Initialize the repository
```
pipenv run script/setup
```
- And finally build the code
```
pipenv run script/cibuild
```

## Run emulator
- To run the emulator, you need to install SDL2 first
```
sudo apt install libsdl2-dev
```

- Set the EMULATOR=1
```
export EMULATOR=1
```

- Build the code
```
pipenv run script/setup
pipenv run script/cibuild
```

- Run the emulator
```
./firmware/prokey.elf
```
* If you are using Ubuntu for windows, you need to install GWSL
