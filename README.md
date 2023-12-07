# d3d12-proxy
Graphics card spoofing libraries for DirectX

I have developed these libraries to bypass vendor locking (mainly for Streamline), currently proxy acts as NVIDIA RTX 3060 Ti.

For wrapper logic and identifying interfaces [RenderDoc](https://github.com/baldurk/renderdoc) source was super helpful.

## Installation
* To use it with DirectX 12 games just place `dxgi.dll` and `d3d12.dll` next to executable.
* To use it with DirectX 11 games just placing `dxgi.dll` next to executable should be enough.

Normally proxy dlls first check `dxgi.o.dll`, `d3d12.o.dll`, `d3d12core.o.dll`,`d3d11.o.dll` files in same folder, if can't locate files, load them from system folder.

*d3d11 and d3d12core projects are here for logging/debugging purposes.*

#### TODO
* Adding ini support for some options 
* Solving the DirectX11 performance regression

## Official Discord Channel: https://discord.gg/2JDHx6kcXB
