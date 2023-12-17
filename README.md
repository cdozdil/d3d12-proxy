# d3d12-proxy
Graphics card spoofing libraries for DirectX

I have developed these libraries to bypass vendor locking (mainly for Streamline), currently proxy acts as NVIDIA RTX 3060 Ti.

## Installation
* To use it just place `dxgi.dll` next to executable, it should be enough.

Normally proxy dlls first check `dxgi.o.dll`, `d3d12.o.dll`, `d3d12core.o.dll`,`d3d11.o.dll` files in same folder, if can't locate files, load them from system folder.

*d3d11, d3d12 and d3d12core projects are here for logging/debugging purposes.*

#### TODO
* Adding ini support for some options 

## Official Discord Channel: https://discord.gg/2JDHx6kcXB
