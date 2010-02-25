.PHONY: default xcode clean

default:
	(mkdir -p build; cd build; cmake ..; make)

xcode:
	(mkdir -p build.xcode; cd build.xcode; cmake .. -G"Xcode"; open EWS.xcodeproj)

clean:
	rm -rf build build.xcode