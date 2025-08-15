
# BORIUM documentation

---

## COMPILER

### TOOLS

> Here we will use a Linux environment.

```sh
apt-get update && apt-get upgrade -y
sudo apt-get install nasm binutils build-essential qemu-system make -y
```

### COMPILE AND RUN

After installing the OS compilation tools,
You can compile and run the OS with these commands (QEMU is required for `make run`) :

```sh
make
make run
```

And then, you can delete the binary files by doing:

```sh
make clean
```
