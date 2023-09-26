## Build in WSL

1. Make sure that env PICO_SDK_PATH is set

```
export PICO_SDK_PATH=../../pico-sdk
```

2. Make directory build 

```
mkdir build
```

3. Run cmake from build directory

```
cd build
cmake ..
```

Example from WSL terminal:
```
/mnt/d/LocalRepo/ImbueLight/imbuelight-stair-controller/firmware/imbuelight_stair_controller_fw/build$ cmake ..
```

4. Build project

```
make
```


To update pico sdk
```
cd pico-sdk
$ git pull
$ git submodule update
```