## LLVM IR (BC) Production

The first step of running SGXRay is to obtain a *single* LLVM IR file for an SGX
enclave application.We provide two Docker images for Intel SGX SDK and Open Enclave SDK, respectively.


### Intel SGX SDK Frontend Image

This image contains Intel SDK SGX version 2.12 and Clang version 11.

To obtain the LLVM IR for your application, please run the Docker container interactively with your application directory mounted,

```shell
cd <project> # go to the root your SGX project
docker run -it --rm -v $(pwd):/sgx -w /sgx --user $UID:$(id -g) baiduxlab/sgx-ray-frontend-intel:latest
```

Inside the container, please run,

```shell
source /opt/sgxsdk/environment
source /opt/smack-wllvm/default.environment
make # your compilation command
extract-bc -l llvm-link-11 <enclave.so> # this should produce enclave.so.bc
```


### Open Enclave Frontend Image

This image contains openenclave version 0.16.1 and Clang version 8.

To obtain the LLVM IR for your application, please run the Docker container interactively with your application directory mounted,

```shell
cd <project> # go to the root your SGX project
docker run -it --rm -v $(pwd):/sgx -w /sgx --user $UID:$(id -g) baiduxlab/sgx-ray-frontend-intel:latest
```

Inside the container, please run,

```shell
source /opt/openenclave/share/openenclave/openenclaverc
source /opt/smack-wllvm/oe.environment
make # your compilation command
extract-bc -l llvm-link-8 <enclave.so> # this should produce enclave.so.bc
```
