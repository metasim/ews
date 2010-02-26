.PHONY: xcode clean

build:
	(mkdir -p build; cd build; cmake ..; make)

test: build
	(cd build; make test)

xcode:
	(mkdir -p build.xcode; cd build.xcode; cmake .. -G"Xcode"; open EWS.xcodeproj)

clean:
	rm -rf build build.xcode