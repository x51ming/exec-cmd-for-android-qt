# Intro

- Run Any Executable File on Android

- Golang
```bash
CGO_ENABLED=1 GOOS=android GOARCH=arm64 \
NDK_TOOLCHAIN=${NDK_ROOT}/toolchains/llvm/prebuilt/darwin-x86_64 \
CC=${NDK_ROOT}/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android33-clang \
	go build
```